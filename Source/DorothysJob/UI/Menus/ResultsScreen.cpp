#include "ResultsScreen.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Spacer.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/UI/UMGPalette/Button/ReloadLevelButton.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Data/DataTables/ScorePointsData.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include "DorothysJob/Data/DataAssets/Procedural/EndlessScoreDataAsset.h"

#pragma region | Virtual Methods
void UResultsScreen::NativeConstruct()
{
  Super::NativeConstruct();

  m_eState = EBaseGameState::WIN;

  if (IsValid(m_pContinueButton))
  {
    m_pContinueButton->OnClicked.AddUniqueDynamic(this, &UResultsScreen::OnClickContinue);
  }
  if (GetWorld())
  {
    if (UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) m_pSaveGameManager = pBaseGameInstance->GetSubsystem<USaveGameManager>();
  }
}

void UResultsScreen::Show()
{
  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Visible);
    if (IsValid(m_pFadeInAnimation))
    {
      PlayAnimation(m_pFadeInAnimation);
    }
  }
}

void UResultsScreen::Hide()
{
  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
  }
}

void UResultsScreen::SetupFocus()
{
  Super::SetupFocus();

  m_lFocusableElements.Empty();
  m_pSpecificFocusManager.Empty();

  m_lFocusableElements.Add(Cast<UObject>(m_pContinueButton));

  if (m_eState == EBaseGameState::WIN)
  {
    m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
    m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { 1 }, EFocusNavigationType::HORIZONTAL, true, false);
  }
  else if (m_eState == EBaseGameState::GAMEOVER)
  {
    m_lFocusableElements.Insert(Cast<UObject>(m_pRetryButton), 0);
    m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
    m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { 2 }, EFocusNavigationType::HORIZONTAL, true, false);
  }
}

void UResultsScreen::SetWidgetState(EBaseGameState _eState)
{
  m_eState = _eState;
  m_pRetryButton->SetVisibility(ESlateVisibility::Collapsed);

  if (m_eState == EBaseGameState::WIN)
  {
    m_pTitle->SetText(m_sWinTitle);
    m_pDorothyImage->SetBrushFromTexture(m_pDorothyWin);
    m_pButtonSpacer->SetSize(FVector2D::ZeroVector);
    m_pContinueButton->SetButtonSelected(true);
  }
  else if (m_eState == EBaseGameState::GAMEOVER)
  {
    m_pTitle->SetText(m_sGameOverTitle);
    m_pDorothyImage->SetBrushFromTexture(m_pDorothyGameOver);
    m_pButtonSpacer->SetSize(FVector2D(125., 1.));
    m_pRetryButton->SetVisibility(ESlateVisibility::Visible);
    m_pContinueButton->SetButtonSelected(false);
  }

  ShowResults();
  SetupFocus();
}
#pragma endregion

#pragma region | OnClick Methods
void UResultsScreen::OnClickContinue()
{
  // Play Killer Sound Event.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
    }
  }

  if (m_eState == EBaseGameState::WIN)
  {
    ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
    if (IsValid(pLevelManager))
    {
      pLevelManager->OnLevelFinish();
    }
  }
  else if (m_eState == EBaseGameState::GAMEOVER)
  {
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), m_pHubMap);
  }
}
#pragma endregion

void UResultsScreen::ShowResults()
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  EScore eScore = EScore::F;
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  bool bStatsTextNoValid = !IsValid(m_pResultDetailsText1) || !IsValid(m_pResultDetailsText2) || !IsValid(m_pResultDetailsText3);
  bool bImagesNoValid = !IsValid(m_pFirstStarImage) || !IsValid(m_pSecondStarImage) || !IsValid(m_pScoreImage);
  if (!IsValid(pLevelManager) || !IsValid(pGameInstance) || bStatsTextNoValid || bImagesNoValid)
  {
    return;
  }

  if (pGameInstance->m_iCurrentLevel <= 10)
  {
    FFinalScore* pFinalScore = pLevelManager->GetFinalScore();
    if (pFinalScore != nullptr)
    {
      m_pFirstStarImage->SetBrushFromTexture(pFinalScore->m_bHasBeenHit ? m_pStarEmptyImage : m_pStarFullImage);
      m_pSecondStarImage->SetBrushFromTexture(pFinalScore->m_bIsTimerExtra ? m_pStarFullImage : m_pStarEmptyImage);
      FText sUnavailableText = FText::FromString("???????? pts");

      if (pFinalScore->m_iTimePoints >= 0)
      {
        m_pResultDetailsText1->SetText(FText::Format(m_sFirstStatText, FText::AsNumber(pFinalScore->m_iTimePoints)));
      }
      else
      {
        m_pResultDetailsText1->SetText(sUnavailableText);
      }

      if (pFinalScore->m_iConsumablesPoints >= 0)
      {
        m_pResultDetailsText2->SetText(FText::Format(m_sSecondStatText, FText::AsNumber(pFinalScore->m_iConsumablesPoints)));
      }
      else
      {
        m_pResultDetailsText2->SetText(sUnavailableText);
      }

      if (pFinalScore->m_iCleaningStationPoints >= 0)
      {
        m_pResultDetailsText3->SetText(FText::Format(m_sThirdStatText, FText::AsNumber(pFinalScore->m_iCleaningStationPoints)));
      }
      else
      {
        m_pResultDetailsText3->SetText(sUnavailableText);
      }

      TArray<FScorePointsData*> lScores;
      bool bInRange = false;
      bool bHasAllStars = !pFinalScore->m_bHasBeenHit && pFinalScore->m_bIsTimerExtra;
      pGameInstance->m_pScorePointsData->GetAllRows(TEXT("Score"), lScores);
      for (FScorePointsData* pScore : lScores)
      {
        bInRange = (pScore->m_iMinLimit <= pFinalScore->m_iTotalScore) && (pFinalScore->m_iTotalScore <= pScore->m_iMaxLimit);
        if (bInRange && IsValid(pScore->m_pScoreImage) && (pScore->m_sScoreLetter != EScore::SPLUS || bHasAllStars))
        {
          m_pScoreImage->SetBrushFromTexture(pScore->m_pScoreImage);
          eScore = pScore->m_sScoreLetter;
          break;
        }
      }
      if (IsValid(m_pSaveGameManager) && pFinalScore)
      {
        m_pSaveGameManager->SaveBestFinalScore(eScore, pGameInstance->m_iCurrentLevel, pFinalScore->m_bHasBeenHit, pFinalScore->m_bIsTimerExtra);
      }
    }
  }
  else if (IsValid(pGameInstance->m_pEndlessScoreData))
  {
    int32 iEndlessLevel = pGameInstance->m_iCurrentEndlessLevel;

    // Title and Dorothy image
    if (IsValid(m_pTitle))
    {
      m_pTitle->SetText(m_sEndlessTitle);
    }
    if (IsValid(m_pButtonSpacer) && IsValid(m_pRetryButton) && IsValid(m_pContinueButton))
    {
      m_pButtonSpacer->SetSize(FVector2D(125., 1.));
      m_pRetryButton->SetVisibility(ESlateVisibility::Visible);
      m_pContinueButton->SetButtonSelected(false);
    }
    if (IsValid(m_pDorothyImage) && (pGameInstance->m_pEndlessScoreData->Scores.Num() > 0))
    {
      int32 iScoreFDaysIndex = pGameInstance->m_pEndlessScoreData->Scores.Num() - 2;
      if ((pGameInstance->m_pEndlessScoreData->Scores.IsValidIndex(iScoreFDaysIndex)) && (iEndlessLevel < pGameInstance->m_pEndlessScoreData->Scores[iScoreFDaysIndex].Days))
      {
        m_pDorothyImage->SetBrushFromTexture(m_pDorothyGameOver);
      }
      else
      {
        m_pDorothyImage->SetBrushFromTexture(m_pDorothyWin);
      }

      if ((pGameInstance->m_pEndlessScoreData->Scores.IsValidIndex(0)) && (iEndlessLevel < pGameInstance->m_pEndlessScoreData->Scores[0].Days))
      {
        m_pFirstStarImage->SetBrushFromTexture(m_pStarEmptyImage);
        m_pSecondStarImage->SetBrushFromTexture(m_pStarEmptyImage);
      }
      else
      {
        m_pFirstStarImage->SetBrushFromTexture(m_pStarFullImage);
        m_pSecondStarImage->SetBrushFromTexture(m_pStarFullImage);
      }
    }

    // Stats
    m_pResultDetailsText1->SetText(FText::Format(m_sEndlessFirstStatText, FText::AsNumber(iEndlessLevel)));
    m_pResultDetailsText2->SetText(FText::Format(m_sEndlessSecondStatText, FText::AsNumber(pGameInstance->SecondsLastedInEndless)));
    m_pResultDetailsText3->SetText(FText::Format(m_sEndlessThirdStatText, FText::AsNumber(pGameInstance->EnemiesKilledInEndless)));

    pGameInstance->m_iCurrentEndlessLevel = -1;
    for (const FDaysScore DayScore : pGameInstance->m_pEndlessScoreData->Scores)
    {
      if (iEndlessLevel >= DayScore.Days && IsValid(DayScore.Score))
      {
        m_pScoreImage->SetBrushFromTexture(DayScore.Score);
        eScore = DayScore.m_sScoreLetter;
        break;
      }
    }
    if (IsValid(m_pSaveGameManager))
    {
      m_pSaveGameManager->SaveBestFinalScore(eScore, pGameInstance->m_iCurrentLevel);
    }
  }
}