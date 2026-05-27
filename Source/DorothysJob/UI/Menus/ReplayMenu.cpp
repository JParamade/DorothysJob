#include "ReplayMenu.h"
#include "Components/CanvasPanel.h"
#include "Components/GridPanel.h"
#include "Components/GridSlot.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/UI/Utils/InputIcon.h"
#include "DorothysJob/Controller/BasePlayerController.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include "DorothysJob/UI/Menus/ConfirmationWindow.h"
#include "DorothysJob/UI/Utils/ReplayResultsWidget.h"

void UReplayMenu::NativeConstruct()
{
  Super::NativeConstruct();

  m_bIsVisible = false;

  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
  }

  int32 iIndex = 1;
  TArray<UWidget*> lWidget = m_pLevelPanel->GetAllChildren();
  TObjectPtr<UBaseButton> pLevelButton = nullptr;
  TObjectPtr<UReplayResultsWidget> pScoreWidget = nullptr;
  for (UWidget* pWidget : lWidget)
  {
    // Buttons
    pLevelButton = Cast<UBaseButton>(pWidget);
    if (IsValid(pLevelButton))
    {
      TArray<UWidget*> lButtonChildren = pLevelButton->GetAllChildren();
      for (UWidget* pChildWidget : lButtonChildren)
      {
        UTextBlock* pTextBlock = Cast<UTextBlock>(pChildWidget);
        if (IsValid(pTextBlock))
        {
          pTextBlock->SetText(FText::Format(m_sLevelsTitle, FText::AsNumber(iIndex)));
          break;
        }
      }

      pLevelButton->OnClickedAction.BindLambda([this, iIndex]() { OnLevelClicked(iIndex); });
      m_lLevelButton.Add(pLevelButton);
      ++iIndex;
    }
    // Scores
    pScoreWidget = Cast<UReplayResultsWidget>(pWidget);
    if (IsValid(pScoreWidget))
    {
      m_lLevelScore.Add(pScoreWidget);
    }
  }
}

void UReplayMenu::NativeDestruct()
{
  for (TObjectPtr<UBaseButton> pLevelButton : m_lLevelButton)
  {
    pLevelButton->OnClickedAction.Unbind();
  }

  m_lLevelButton.Empty();
  m_lLevelScore.Empty();

  Super::NativeDestruct();
}

void UReplayMenu::Show()
{
  SetupFocus();

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (IsValid(pGameInstance))
  {
    int32 iIndex = pGameInstance->m_iHighestLevelAchieved - 1;
    if (pGameInstance->m_iHighestLevelAchieved == m_lLevelButton.Num())
    {
      m_lLevelButton[iIndex]->m_pButtonSelectedStyleAsset = m_pUnlockedEndlessButtonStyleAsset;
    }
    else
    {
      m_lLevelButton[iIndex]->m_pButtonSelectedStyleAsset = m_pUnlockedButtonStyleAsset;
    }
    m_lLevelButton[iIndex]->SetButtonSelected(true);
  }

  ShowScores();
  SetPauseMode(true);
  m_bIsVisible = true;

  for (UReplayResultsWidget* pLevelScore : m_lLevelScore)
  {
    pLevelScore->Show();
  }

  if (IsValid(m_pExitIconButton))
  {
    m_pExitIconButton->Show();
  }

  if (IsValid(m_pExitText))
  {
    m_pExitText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
  }

  if (IsValid(m_pCanvasPanel) && m_pCanvasPanel->GetVisibility() != ESlateVisibility::Visible)
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Visible);
  }

  OnMenuShow.Broadcast();
  OnMenuOpened.Broadcast();
}

void UReplayMenu::Hide()
{
  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
  }

  for (UReplayResultsWidget* pLevelScore : m_lLevelScore)
  {
    pLevelScore->Hide();
  }

  if (IsValid(m_pExitIconButton))
  {
    m_pExitIconButton->Hide();
  }

  if (IsValid(m_pExitText))
  {
    m_pExitText->SetVisibility(ESlateVisibility::Hidden);
  }

  SetPauseMode(false);
  m_bIsVisible = false;
  OnMenuClosed.Broadcast();
}

void UReplayMenu::SetupFocus()
{
  Super::SetupFocus();

  m_lFocusableElements.Empty();
  m_pSpecificFocusManager.Empty();
  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  InitFocusManager();
  ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  UGeneralFocusManager* pGeneralFocusManager = IsValid(pPlayerController) ? pPlayerController->GetGeneralFocusManager() : nullptr;
  if (IsValid(pGeneralFocusManager))
  {
    pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
  }
}

void UReplayMenu::CancelAction()
{
  if (IsValid(m_pConfirmationWindow) && (!m_pConfirmationWindow->IsWidgetVisible()))
  {
    GetWorld()->GetSubsystem<ULevelManager>()->SetCurrentGameState(EBaseGameState::PLAYING);
  }
}

void UReplayMenu::OnLevelClicked(int32 _iLevel)
{
  if (IsValid(m_pConfirmationWindow))
  {
    m_pConfirmationWindow->OnConfirmResult.AddWeakLambda(this, [this, _iLevel](bool _bResult) 
      {
        if (IsValid(m_pConfirmationWindow))
        {
          m_pConfirmationWindow->OnConfirmResult.RemoveAll(this);
        }

        if (!_bResult)
        {
          SetupFocus();
          return;
        }

        UWorld* pWorld = GetWorld();
        if (IsValid(pWorld))
        {
          if (UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(pWorld->GetGameInstance()))
          {
            pBaseGameInstance->m_iCurrentLevel = _iLevel;
            pBaseGameInstance->UpdateAvailableWeaponsAndConsumables(_iLevel);
            pBaseGameInstance->UpdateSelectedWeapons();
            pBaseGameInstance->UpdateSelectedConsumables();
            // Play Killer Sound Event.
            if (UAudioManager* pAudioManager = pBaseGameInstance->GetSubsystem<UAudioManager>())
            {
              pAudioManager->PlaySound2D(GetWorld(), pBaseGameInstance->m_pKillerEvent);
            }

            UGameplayStatics::OpenLevelBySoftObjectPtr(pWorld, pWorld->GetCurrentLevel());
          }
        }
      });
    m_pConfirmationWindow->Show();
  }
}

void UReplayMenu::InitFocusManager()
{
  int32 iFirstLine = 0;
  int32 iSecondLine = 0;
  bool bThirdLineEnable = false;

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (IsValid(pGameInstance))
  {
    for (int32 iIndex = 0; iIndex < m_lLevelButton.Num(); ++iIndex)
    {
      if ((pGameInstance->m_iHighestLevelAchieved >= (iIndex + 1)) && (pGameInstance->m_iCurrentLevel != (iIndex + 1)))
      {
        m_lFocusableElements.Add(m_lLevelButton[iIndex]);
        if (iIndex < 5)
        {
          ++iFirstLine;
        }
        else if(iIndex < 10)
        {
          ++iSecondLine;
        }
        else
        {
          bThirdLineEnable = true;
        }
      }
      else
      {
        m_lLevelButton[iIndex]->SetIsEnabled(false);
      }
    }
  }

  if ((iFirstLine > 0) && (iSecondLine == 0))
  {
    m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { iFirstLine }, EFocusNavigationType::HORIZONTAL, true, false);
  }
  else if ((iSecondLine > 0) && (!bThirdLineEnable))
  {
    m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { iFirstLine, iSecondLine }, EFocusNavigationType::GRID, true, false);
  }
  else if (bThirdLineEnable)
  {
    m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { iFirstLine, iSecondLine, 1 }, EFocusNavigationType::GRID, true, false);
  }
}

void UReplayMenu::ShowScores()
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  USaveGameManager* pSaveGameManager = IsValid(pGameInstance) ? pGameInstance->GetSubsystem<USaveGameManager>() : nullptr;
  if (IsValid(pSaveGameManager))
  {
    for (int32 iIndex = 0; iIndex < m_lLevelScore.Num(); ++iIndex)
    {
      if (pGameInstance->m_iHighestLevelAchieved > (iIndex + 1))
      {
        FFinalSavedScore oFSS = pSaveGameManager->GetBestFinalScore(iIndex + 1);
        if (oFSS.m_eScore != EScore::F)
        {
          int32 iScore = static_cast<int32>(oFSS.m_eScore);
          m_lLevelScore[iIndex]->m_bExtraTimeImage = oFSS.m_bIsTimerExtra;
          m_lLevelScore[iIndex]->m_bNoHitStarEnabled = !oFSS.m_bHasBeenHit;
          m_lLevelButton[iIndex]->m_pButtonSelectedStyleAsset = m_lScoreButtonStyleAsset[iScore];
          m_lLevelButton[iIndex]->SetButtonSelected(true);
        }
      }
    }
    // Endless
    if (pGameInstance->m_iHighestLevelAchieved == m_lLevelButton.Num())
    {
      FFinalSavedScore oFSS = pSaveGameManager->GetBestFinalScore(pGameInstance->m_iHighestLevelAchieved);
      if (oFSS.m_eScore != EScore::F)
      {
        int32 iScore = static_cast<int32>(oFSS.m_eScore);
        m_lLevelButton[m_lLevelButton.Num() - 1]->m_pButtonSelectedStyleAsset = m_lEndlessScoreButtonStyleAsset[iScore];
        m_lLevelButton[m_lLevelButton.Num() - 1]->SetButtonSelected(true);
      }
    }
  }
}