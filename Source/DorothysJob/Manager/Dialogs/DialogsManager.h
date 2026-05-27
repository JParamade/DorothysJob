/************************************************************************
 * @description: The Dialog system manager.
 * @author: Aurora Fern�ndez
 * @date: 30/08/2025
 * @edited_by: Josephine Esposito - 07/09/2025
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

#pragma region | Delegates
DECLARE_MULTICAST_DELEGATE(FOnLineStarted);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDialogLineReceived, FDialogData);
DECLARE_MULTICAST_DELEGATE(FOnDialogFinish);
#pragma endregion

/**
 * This represent the manager of the dialog system.
 */
UCLASS()
class DOROTHYSJOB_API UDialogsManager : public UObject
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief Initialice class in Game Istance, take the data asset.
	 * @param InDialogDataAsset The data asset of the dialog.
	 */
	UFUNCTION()
	void Init();

	FDialogData* m_pDialogData;

	TArray<FDialogData*> m_lDialogSequences;

#pragma region | Delegates
	/* Delegate for when a line has started */
	FOnLineStarted OnLineStarted;

	/* Delegate for when a line needs to be printed */
	FOnDialogLineReceived OnDialogLineReceived;

	/* Delegate for when te dialog finish */
	FOnDialogFinish OnDialogFinish;
#pragma endregion

#pragma region | Dialog Fuctions
	/**
	 * @brief Start a dialog sequence, save the dialog lines of the secuence and printed first line.
	 * @param _iSequenceID the sequence name/id
	 */
	void OnInitSequence(FName _iSequenceID);

	/**
	 * @brief Restart all when the sequence ends.
	 */
	void OnEndSequence();

	/**
	 * @brief Print the next dialog line.
	 */
	void OnNextDialogLine();

	/**
	 * @brief End sequence when dialog is skip.
	 */
	void OnSkipDialog();

	/**
	 * @brief Broadcast the next line.
	 * @param _oCurrentDialogBloc The next line that is going to be printed.
	 */
	//void BroadcastDialog(FDialogBlock _oCurrentDialogBloc);
	void BroadcastDialog(FDialogData _oCurrentDialogScene);

	void SetCurrentStatus(EBaseGameState _status);
#pragma endregion

private:

	EBaseGameState m_oActualStatus;
#pragma region | Variables

	FName m_sCurrentDialogId;

	/* The current dialog sequence. */
	const FDialogSequence* m_pCurrentDialogSequence;

	TArray<FDialogData*> m_lCurrentScenes;

	const FDialogData* m_pCurrentDialogScene;

	/* The current line thats going to be printed. */
	TArray<FDialogBlock> m_oCurrentDialogs;

	/* How many lines has the sequence. */
	int m_iCurrentDialogsLines;

	/* The current dialog line index. */
	int m_iCurrentDialogsIndex;

	/**
	 * @brief The reference of the game instance
	 */
	UBaseGameInstance* m_pGameInstance;
#pragma endregion

public:
	/* A bool to see if the line has finish printed or no.*/
	bool m_bLineIsPrinted = true;

#pragma region | Getters&Setters
	//const FDialogSequence* GetDialogSequenceByID(FName _iSequenceID);
	const TArray<FDialogData*> GetDialogSequenceByID(FName _sSequenceName);

	//const FDialogBlock& GetCurrentDialogToDisplay();
	const FDialogData& GetCurrentDialogToDisplay();

	void SetDialogHasStart(bool _bIsPrinted);

	bool GetDialogHasStart();

	void CheckNextDialogue();
#pragma endregion

};
