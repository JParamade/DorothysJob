/************************************************************************
 * @description: Manages the dialog sequences in the game, including starting, ending, and progressing through dialog lines.
 * @author: Aurora Fernandez
 * @date: 30/08/2025
 * @edited_by: Josephine Esposito
 *					   Jaime Paramo
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogsManager.generated.h"

enum class EBaseGameState : uint8;
class UDialogsDataAsset;
struct FDialogBlock;
struct FDialogSequence;
struct FDialogData;
class UBaseGameInstance;

// Delegates
DECLARE_MULTICAST_DELEGATE(FOnLineStarted);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogLineReceived, FDialogData);
DECLARE_MULTICAST_DELEGATE(FOnDialogFinish);

UCLASS()
class DOROTHYSJOB_API UDialogsManager : public UObject {
	GENERATED_BODY()
	
public:
	/**
	 * @brief Initialize the dialog manager, setting the initial game state and loading the dialog data from the Game Instance.
	 */
	UFUNCTION()
	void Init();

	/**
   * @brief Pointer to the current dialog data being displayed.
   */
	FDialogData* m_pDialogData;

	/**
	 * @brief Array of pointers to dialog sequences, where each sequence contains multiple dialog scenes.
	 */
	TArray<FDialogData*> m_lDialogSequences;

	/**
   * @brief Delegate for when a line starts to be printed.
	 */
	FOnLineStarted OnLineStarted;
	/**
   * @brief Delegate for when a dialog line is received and ready to be displayed.
	 */
	FOnDialogLineReceived OnDialogLineReceived;
	/**
   * @brief Delegate for when a dialog sequence finishes.
	 */
	FOnDialogFinish OnDialogFinish;

	/**
	 * @brief Initialize a dialog sequence based on the provided sequence ID.
   * @param _iSequenceID The ID of the dialog sequence to initialize.
	 */
	void OnInitSequence(FName _iSequenceID);

	/**
   * @brief End the current dialog sequence, resetting all related variables and broadcasting the OnDialogFinish event.
	 */
	void OnEndSequence();

	/**
	 * @brief Progress to the next dialog line in the current sequence.
	 */
	void OnNextDialogLine();

	/**
	 * @brief Skip the current dialog sequence.
	 */
	void OnSkipDialog();

	/**
   * @brief Broadcast the current dialog scene to be displayed, using the OnDialogLineReceived delegate.
   * @param _oCurrentDialogScene The dialog scene data to broadcast for display.
	 */
	void BroadcastDialog(FDialogData _oCurrentDialogScene);
	/**
   * @brief Set the current game state status, which can be used to manage transitions between dialog sequences and normal gameplay.
   * @param _status The new game state status to set for the dialog manager.
	 */
	void SetCurrentStatus(EBaseGameState _status);

private:
	/**
   * @brief The current game state status, which can be used to manage transitions between dialog sequences and normal gameplay.
	 */
	EBaseGameState m_oActualStatus;
	/**
	 * @brief The ID of the current dialog sequence being displayed.
	 */
	FName m_sCurrentDialogId;

	/**
   * @brief Pointer to the current dialog sequence being displayed, which contains multiple dialog scenes.
	 */
	const FDialogSequence* m_pCurrentDialogSequence;
	/**
   * @brief Array of pointers to the current dialog scenes that are part of the active dialog sequence, which will be displayed in order.
	 */
	TArray<FDialogData*> m_lCurrentScenes;
	/**
   * @brief Pointer to the current dialog scene being displayed, which contains the dialog lines and related data for that scene.
	 */
	const FDialogData* m_pCurrentDialogScene;

	/**
	 * @brief Array of dialog blocks that are currently being displayed.
	 */
	TArray<FDialogBlock> m_oCurrentDialogs;

	/**
	 * @brief The total number of dialog lines in the current dialog sequence, which is used to manage progression through the sequence.
	 */
	int m_iCurrentDialogsLines;

	/**
	 * @brief The index of the current dialog line being displayed within the current dialog sequence.
	 */
	int m_iCurrentDialogsIndex;

	/**
	 * @brief Pointer to the Game Instance.
	 */
	UBaseGameInstance* m_pGameInstance;

public:
	/**
	 * @brief Flag to indicate whether the current dialog line has finished printing.
	 */
	bool m_bLineIsPrinted = true;

	/**
	 * @brief Retrieve the dialog sequence data for a given sequence name from the Game Instance's dialog data map.
   * @param _sSequenceName The name of the dialog sequence to retrieve.
	 * @return An array of pointers to FDialogData representing the dialog scenes in the requested sequence.
	 */
	const TArray<FDialogData*> GetDialogSequenceByID(FName _sSequenceName);

	/**
	 * @brief Retrieve the current dialog scene data that should be displayed.
	 * @return A reference to the FDialogData struct representing the current dialog scene to be displayed.
	 */
	const FDialogData& GetCurrentDialogToDisplay();
	/**
	 * @brief Set the flag indicating whether the current dialog line has finished printing.
	 * @param _bIsPrinted A boolean value to set the m_bLineIsPrinted flag.
	 */
	void SetDialogHasStart(bool _bIsPrinted);
	/**
   * @brief Get the flag indicating whether the current dialog line has finished printing.
   * @return A boolean value indicating whether the current dialog line has finished printing (true) or is still being printed (false).
	 */
	bool GetDialogHasStart();
	/**
	 * @brief Check if there is a next dialog line to display in the current sequence and progress to it if available.
	 */
	void CheckNextDialogue();
};