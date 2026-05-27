#include "DialogsManager.h"
#include "DorothysJob/Data/DataTables/DialogData.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

#pragma region | Init Manager
void UDialogsManager::Init()
{
  m_pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
  if (!IsValid(m_pGameInstance))
  {
    UE_LOG(LogTemp, Warning, TEXT("Game Instance not found."));
    return;
  }

  if (m_pGameInstance->m_mDialogsData.Num() <= 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("Dialog Data Table map found empty?"));
    return;
  }

  m_oActualStatus = EBaseGameState::PLAYING;
}
#pragma endregion

#pragma region | Sequences Functions
void UDialogsManager::OnInitSequence(FName _sSequenceID)
{
  m_sCurrentDialogId = _sSequenceID;

  m_lCurrentScenes = GetDialogSequenceByID(_sSequenceID);

  m_iCurrentDialogsLines = m_lCurrentScenes.Num();
  if (m_iCurrentDialogsLines <= 0)
  {
    UE_LOG(LogTemp, Error, TEXT("No sequence with ID %s was found!"), *_sSequenceID.ToString());
    OnEndSequence();
    return;
  }

  m_iCurrentDialogsIndex = 0;

  m_bLineIsPrinted = true;
  OnLineStarted.Broadcast();
  BroadcastDialog(GetCurrentDialogToDisplay());
}

void UDialogsManager::OnEndSequence()
{
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance)
  {
    return;
  }

  if (m_sCurrentDialogId == FName("Lev_10_Start"))
  {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->UnlockAchievement("YouHaveBeenBetrayed");
    }
  }

  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return;
  }

  if (m_oActualStatus == EBaseGameState::PLAYING)
  {
    pLevelManager->OnGameDialogue(false);
  }
  else
  {
    pLevelManager->CurrentGameState = m_oActualStatus;
  }

  if (m_sCurrentDialogId.ToString().EndsWith("End"))
  {
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Hub"));
  }

  m_sCurrentDialogId = "";
  m_pCurrentDialogSequence = nullptr;
  m_iCurrentDialogsLines = 0;
  m_iCurrentDialogsIndex = -1;
  m_oCurrentDialogs.Empty();

  OnDialogFinish.Broadcast();
}
#pragma endregion

#pragma region | Dialog Functions
void UDialogsManager::OnNextDialogLine()
{
  OnLineStarted.Broadcast();

  if (!m_bLineIsPrinted)
  {
    return;
  }
  
  CheckNextDialogue();

  BroadcastDialog(GetCurrentDialogToDisplay());
}

void UDialogsManager::OnSkipDialog()
{
  m_bLineIsPrinted = true;
  if (m_lCurrentScenes.Num() <= 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("Skip called but no dialog sequence?"));
  }

  if (m_pGameInstance) {
    if (UAchievementSubsystem* pAchSubsystem = m_pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->AddProgress("WhyDidIEvenWriteThese", 1);
    }
  }
  OnEndSequence();
}

void UDialogsManager::BroadcastDialog(FDialogData _oCurrentDialogScene)
{
  OnDialogLineReceived.Broadcast(_oCurrentDialogScene);
}
void UDialogsManager::SetCurrentStatus(EBaseGameState _status)
{
  m_oActualStatus = _status;
}
#pragma endregion

#pragma region | Getters&Setters

const TArray<FDialogData*> UDialogsManager::GetDialogSequenceByID(FName _sSequenceName)
{
  TArray<FDialogData*> lScenes_;

  if (!IsValid(m_pGameInstance))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid Game Instance??"));
    m_pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!IsValid(m_pGameInstance))
    {
      UE_LOG(LogTemp, Warning, TEXT("Game Instance not found."));
      return lScenes_;
    }
  }

  if (m_pGameInstance->m_mDialogsData.Num() <= 0) // we check if there are loaded data tables
  {
    UE_LOG(LogTemp, Error, TEXT("No dialogs to load?"));
    return lScenes_;
  }

  TObjectPtr<UDataTable>* pFoundSequence = m_pGameInstance->m_mDialogsData.Find(_sSequenceName);
  if (!pFoundSequence || !IsValid(*pFoundSequence))
  {
    UE_LOG(LogTemp, Error, TEXT("No sequence found with <%s>?"), *_sSequenceName.ToString());
    return lScenes_;
  }

  TObjectPtr<UDataTable> pSequence = *pFoundSequence;

  pSequence->GetAllRows(TEXT("Scenes"), lScenes_); // we get all the scenes
  if (lScenes_.Num() <= 0)
  {
    UE_LOG(LogTemp, Error, TEXT("No scenes found with <%s>?"), *_sSequenceName.ToString());
    OnEndSequence();
  }

  return lScenes_;
}

const FDialogData& UDialogsManager::GetCurrentDialogToDisplay()
{
  if (!m_lCurrentScenes.IsValidIndex(m_iCurrentDialogsIndex))
  {
    static FDialogData Dummy;
    UE_LOG(LogTemp, Error, TEXT("Invalid dialog index <%d>! Returning dummy block"), m_iCurrentDialogsIndex);
    return Dummy;
  }

  return *m_lCurrentScenes[m_iCurrentDialogsIndex];
}

void UDialogsManager::SetDialogHasStart(bool _bIsPrinted)
{
  m_bLineIsPrinted = _bIsPrinted;
}

bool UDialogsManager::GetDialogHasStart()
{
  return m_bLineIsPrinted;
}

void UDialogsManager::CheckNextDialogue()
{
  if (m_lCurrentScenes.Num() <= 0)
  {
    UE_LOG(LogTemp, Warning, TEXT("No dialog sequence?"));
    return;
  }

  m_iCurrentDialogsIndex++;

  if (m_iCurrentDialogsIndex > m_iCurrentDialogsLines)
  {
    UE_LOG(LogTemp, Log, TEXT("Dialog sequence finished"));
    OnEndSequence();
  }
}
#pragma endregion
