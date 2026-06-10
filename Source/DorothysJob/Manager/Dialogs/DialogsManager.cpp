/************************************************************************
 * @description: Manages the dialog sequences in the game, including starting, ending, and progressing through dialog lines.
 * @author: Aurora Fernandez
 * @date: 30/08/2025
 * @edited_by: Josephine Esposito
 *					   Jaime Paramo
 ************************************************************************/

#include "DialogsManager.h"
#include "DorothysJob/Data/DataTables/DialogData.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

void UDialogsManager::Init() {
  // Try to get the Game Instance and log a warning if it fails, as the dialog system relies on it to function properly.
  m_pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
  if (!IsValid(m_pGameInstance)) {
    UE_LOG(LogTemp, Warning, TEXT("[DIALOGS_MANAGER] Game Instance not found."));
    return;
  }

  // Check if the dialog data table map is empty and log a warning if it is, as this would indicate that no dialog sequences can be loaded.
  if (m_pGameInstance->m_mDialogsData.Num() <= 0) {
    UE_LOG(LogTemp, Warning, TEXT("[DIALOGS_MANAGER] Dialog Data Table map is empty."));
    return;
  }

  // Set the initial game state to PLAYING, which is the default state when not in a dialog sequence.
  m_oActualStatus = EBaseGameState::PLAYING;
}

void UDialogsManager::OnInitSequence(FName _sSequenceID) {
  // Set the current dialog ID to the provided sequence ID, which will be used to track which dialog sequence is currently active.
  m_sCurrentDialogId = _sSequenceID;

  // Load the dialog sequence corresponding to the provided ID and store it in m_lCurrentScenes.
  m_lCurrentScenes = GetDialogSequenceByID(_sSequenceID);

  // Set the number of dialog lines in the current sequence based on the number of scenes loaded.
  m_iCurrentDialogsLines = m_lCurrentScenes.Num();
  if (m_iCurrentDialogsLines <= 0)
  {
    UE_LOG(LogTemp, Error, TEXT("[DIALOGS_MANAGER] No sequence with ID %s was found."), *_sSequenceID.ToString());
    OnEndSequence();
    return;
  }

  // Initialize the current dialog line index to 0, which means the first line of the sequence will be displayed when the sequence starts.
  m_iCurrentDialogsIndex = 0;

  // Set the flag to indicate that a line is currently being printed.
  m_bLineIsPrinted = true;

  // Broadcast the OnLineStarted event to notify any listeners that a new dialog line is starting.
  OnLineStarted.Broadcast();
  // Broadcast the first dialog line of the sequence to be displayed.
  BroadcastDialog(GetCurrentDialogToDisplay());
}

void UDialogsManager::OnEndSequence() {
  // Try to get the Game Instance and log a warning if it fails, as the dialog system relies on it to function properly.
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance) return;

  // Unlock the "YouHaveBeenBetrayed" achievement if the current dialog ID is "Lev_10_Start".
  if (m_sCurrentDialogId == FName("Lev_10_Start")) {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->UnlockAchievement("YouHaveBeenBetrayed");
    }
  }

  // Try to get the Level Manager subsystem and log a warning if it fails.
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager)) return;

  // If the current game state is PLAYING, it means we are returning to the normal game state after a dialog sequence.
  if (m_oActualStatus == EBaseGameState::PLAYING) pLevelManager->OnGameDialogue(false);
  else pLevelManager->CurrentGameState = m_oActualStatus;

  // If the current dialog ID ends with "End", it indicates that this is the end of a level's dialog sequence, so we open the hub level.
  if (m_sCurrentDialogId.ToString().EndsWith("End")) UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Hub"));

  // Reset all dialog-related variables to their default states.
  m_sCurrentDialogId = "";
  m_pCurrentDialogSequence = nullptr;
  m_iCurrentDialogsLines = 0;
  m_iCurrentDialogsIndex = -1;
  m_oCurrentDialogs.Empty();

  // Broadcast the OnDialogFinish event to notify any listeners that the dialog sequence has finished.
  OnDialogFinish.Broadcast();
}

void UDialogsManager::OnNextDialogLine() {
  // Broadcast the OnLineStarted event to notify any listeners that a new dialog line is starting.
  OnLineStarted.Broadcast();

  // If the line has not finished printing, we do not proceed to the next line until it has.
  if (!m_bLineIsPrinted) return;
  
  // Check if there is a next dialogue to display.
  CheckNextDialogue();

  // Broadcast the next dialog line to be displayed.
  BroadcastDialog(GetCurrentDialogToDisplay());
}

void UDialogsManager::OnSkipDialog() {
  // Set the flag to indicate that a line is currently being printed.
  m_bLineIsPrinted = true;

  // Log a warning if the skip function is called but there are no dialog scenes currently loaded.
  if (m_lCurrentScenes.Num() <= 0) {
    UE_LOG(LogTemp, Warning, TEXT("[DIALOGS_MANAGER] No dialogue scenes loaded."));
  }

  // Unlock the "WhyDidIEvenWriteThese" achievement if the Game Instance is valid.
  if (m_pGameInstance) {
    if (UAchievementSubsystem* pAchSubsystem = m_pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->AddProgress("WhyDidIEvenWriteThese", 1);
    }
  }

  // End the dialog sequence immediately when skipping.
  OnEndSequence();
}

void UDialogsManager::BroadcastDialog(FDialogData _oCurrentDialogScene) { OnDialogLineReceived.Broadcast(_oCurrentDialogScene); }
void UDialogsManager::SetCurrentStatus(EBaseGameState _status) { m_oActualStatus = _status; }

const TArray<FDialogData*> UDialogsManager::GetDialogSequenceByID(FName _sSequenceName) {
  // Create an array to hold the dialog scenes that will be loaded from the data table.
  TArray<FDialogData*> lScenes_;

  // Try to get the Game Instance and log an error if it fails.
  if (!IsValid(m_pGameInstance)) {
    UE_LOG(LogTemp, Error, TEXT("[DIALOGS_MANAGER] Game Instance not found."));
    m_pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if (!IsValid(m_pGameInstance)) {
      UE_LOG(LogTemp, Warning, TEXT("[DIALOGS_MANAGER] Game Instance not found."));
      return lScenes_;
    }
  }

  // Check if the dialog data table map is empty and log an error if it is, as this would indicate that no dialog sequences can be loaded.
  if (m_pGameInstance->m_mDialogsData.Num() <= 0) {
    UE_LOG(LogTemp, Error, TEXT("[DIALOGS_MANAGER] Failed to load dialog sequence."));
    return lScenes_;
  }

  // Try to find the data table corresponding to the provided sequence name in the Game Instance's dialog data map.
  TObjectPtr<UDataTable>* pFoundSequence = m_pGameInstance->m_mDialogsData.Find(_sSequenceName);
  if (!pFoundSequence || !IsValid(*pFoundSequence)) {
    UE_LOG(LogTemp, Error, TEXT("[DIALOGS_MANAGER] No sequence found with <%s>."), *_sSequenceName.ToString());

    // If the sequence is not found, we return the empty array of dialog scenes.
    return lScenes_;
  }

  // Get all rows from the found data table and store them in the lScenes_ array. If no scenes are found, log an error and end the sequence.
  TObjectPtr<UDataTable> pSequence = *pFoundSequence;
  pSequence->GetAllRows(TEXT("Scenes"), lScenes_);
  if (lScenes_.Num() <= 0) {
    UE_LOG(LogTemp, Error, TEXT("[DIALOGS_MANAGER] No scenes found with <%s>."), *_sSequenceName.ToString());
    OnEndSequence();
  }

  // Return the array of dialog scenes that were loaded from the data table.
  return lScenes_;
}

const FDialogData& UDialogsManager::GetCurrentDialogToDisplay() {
  // Check if the current dialog index is valid and log an error if it is not.
  if (!m_lCurrentScenes.IsValidIndex(m_iCurrentDialogsIndex)) {
    static FDialogData oDummy;
    UE_LOG(LogTemp, Error, TEXT("[DIALOGS_MANAGER] Invalid dialog index <%d>."), m_iCurrentDialogsIndex);
    return oDummy;
  }

  // Return the current dialog scene.
  return *m_lCurrentScenes[m_iCurrentDialogsIndex];
}

void UDialogsManager::SetDialogHasStart(bool _bIsPrinted) {
  m_bLineIsPrinted = _bIsPrinted;
}

bool UDialogsManager::GetDialogHasStart() {
  return m_bLineIsPrinted;
}

void UDialogsManager::CheckNextDialogue() {
  // Log a warning if there are no dialog scenes currently loaded.
  if (m_lCurrentScenes.Num() <= 0) {
    UE_LOG(LogTemp, Warning, TEXT("[DIALOGS_MANAGER] Dialogue sequence not found."));
    return;
  }

  // Increment the current dialog index to move to the next line in the sequence.
  m_iCurrentDialogsIndex++;

  // Check if the current dialog index exceeds the number of dialog lines in the sequence.
  if (m_iCurrentDialogsIndex > m_iCurrentDialogsLines) {
    UE_LOG(LogTemp, Log, TEXT("[DIALOGS_MANAGER] Dialog sequence finished."));
    OnEndSequence();
  }
}