/************************************************************************
 * @description: Manage the event pop up for the tutorial levels.
 * @author: Aurora Fernández
 * @date: 03/09/2025
 * @edited_by: 
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Tutorial/Trigger.h"
#include "UObject/NoExportTypes.h"
#include "EventManager.generated.h"

enum class EBaseGameState : uint8;
enum class EHUDSection : uint8;
class UEventTutorialDataAsset;
class UDialogsManager;
class USaveGameManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPermanentEventShow, FName);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTemporalEventShow, FText);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRevealHUD, EHUDSection);
DECLARE_MULTICAST_DELEGATE(FOnDelegateEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnterHubFinish);

/**
 * Class to manage all trigger events for tutorials and dialogs.
 */
UCLASS()
class DOROTHYSJOB_API UEventManager : public UObject
{
	GENERATED_BODY()
	
public:

#pragma region | Param Delegates
	/**
	 * @brief Delegate to pass the permanent info to the correct screen.
	 */
	FOnPermanentEventShow OnPermanentEventShow;	
	
	/**
	 * @brief Delegate to show elements in hud.
	 */
	FOnRevealHUD OnRevealHUD;

	/**
	 * @brief Delegate to pass the temporal info to the correct screen.
	 */
	FOnTemporalEventShow OnTemporalEventShow;
#pragma endregion

#pragma region | Simple Delegate

	/**
	 * @brief Delegate to close the temporal widget screen.
	 */
	FOnDelegateEvent OnTemporalEventClose;

	/**
	 * @brief Delegate for when all events has finish, tu set othe screen correctly.
	 */
	FOnDelegateEvent OnAllEventFinish;

#pragma endregion

#pragma region| Blueprint Delegate
	/**
	 * @brief Delegate for when the hub enter dialog is finish.
	 */
	UPROPERTY(BlueprintAssignable)
	FOnEnterHubFinish OnEnterHubFinish;
#pragma endregion

#pragma region | Main Functions
	/**
		* @brief Initialiace the manager with the dialogs manager, and teh data assets.
		*/
	void Init(UDialogsManager* _pDialogsManager, UEventTutorialDataAsset* _pEventWidgetDataAsset, TObjectPtr<UEventListDataAsset> _pGeneralEvents);

	/**
	 * @brief Start the events when the trigger activates, also see if the trigger is for the hub, if all trigers where activate.
	 */
	UFUNCTION()
	void OnTriggerEventsStart(FName _sTrigerId, const FEventDataArray& _oEventDataArray);

	/**
	 * @brief Trigger an event with teh diferent actions display in the code.
	 */
	UFUNCTION(BlueprintCallable)
	void TriggerEvent(FName _sEventName);

	/**
	 * @brief Start the first event of the event list triggered.
	 */
	UFUNCTION()
	void OnEventsStart(TArray<FTriggerData> _oEventDataArray);

	void HandleEventFinish();
#pragma endregion

#pragma region | Event Functions
	/**
	 * @brief With the data show the correct event, dialogue, permanent widget or temporal widget.
	 */
	void StartEvent(FTriggerData _oEventData);

	/**
	 * @brief Continue with the event, if the event is a dialogue, continue dialogue till finish.
	 */
	void OnEventContinue();

	/**
	 * @brief What happends when a event is skip.
	 */
	void OnSkipEvent();
#pragma endregion

#pragma region | PreLevel Functions
	/**
	 * @brief Function for the pre level screen, return if the secreen can be shown or no.
	 */
	bool CanStartLevel(int level);

	/**
	 * @brief There are triggers that always needs to be on, this is to set those active.
	 */
	void SetIsEventActive(FName _sEventName, bool _bEventIsActive);
#pragma endregion

#pragma region | On weapon and consumable screen functions
	/**
	 * @brief Update the game status in the event manager to see where the status needs to be return after all event finish.
	 */
	void SetCurrentStatus(EBaseGameState _status);
#pragma endregion

#pragma region | 
	void ActiveTriggersFromLevel(int _iLevel);
#pragma endregion
private:

#pragma region | Private Data Asset Variables.
	/**
	 * @brief Permanent widget data asset.
	 */
	TObjectPtr<UEventTutorialDataAsset> m_pEventWidgetDataAsset;

	/**
 * @brief Temporal widget data assets.
 */
	TObjectPtr<UEventListDataAsset> m_pGeneralEventDataAsset;
#pragma endregion

#pragma region | Private Event Variables
	/**
	 * @brief All the events that needs to show up. 
	 */
	TArray<FTriggerData> m_oEventArray;

	/**
	 * @brief The event array index for in what event the game is.
	 */
	int m_iEventArrayIndex = -1;

	/**
	 * @brief The actual event type that is in screen.
	 */
	ETutorialType m_pEventType;

	/**
	 * @brief The actual name of the event that is in the screen.
	 */
	FName m_sCurrentEventName;

	/**
	 * @brief Add when teh event has been skip
	 */
	bool m_bHasSkipEvent = false;
#pragma endregion

#pragma region | Private Dialogue Variables
	/**
	 * @brief The dialog manager.
	 */
	TObjectPtr<UDialogsManager> m_pDialogsManager;

	/**
	 * @brief If the dialog event has finish or not.
	 */
	bool m_pIsDialogueFinish = false;
#pragma endregion

#pragma region | Private Temporal Widget Variables
	/**
	 * @brief Temporal widget timer.
	 */
	UPROPERTY()
	FTimerHandle TemporalWidgetTimerHandle;

	/**
	 * @brief The text to display in the temporal widget.
	 */
	FText m_sText;

	/**
	 * @brief The duration of the temporal widget in seconds for the timer
	 */
	float m_fDuration;
#pragma endregion

#pragma region | Private Permanent Widget Variables
	/**
	 * @brief The image to display in the permanent widget
	 */
	TObjectPtr<UTexture2D> m_pImage;

	/**
	 * @brief The image to display in the permanent widget
	 */
	TObjectPtr<UMaterialInstanceDynamic> m_pDynamicMaterial;

	TArray<int> m_oHubLevelTutNum;

	
#pragma endregion

#pragma region | On weapon and consumable screen variables
	/**
	 * @brief The status that it will come to after event finish.
	 * @note If it's different than PLAYING then it will not change status when finish, because we are already with an open window.
	 */
	EBaseGameState m_oActualStatus;
#pragma endregion

#pragma region | Private SaveGameManager
	/**
	* @brief Reference to the SaveGame manager
	*/
	UPROPERTY()
	TObjectPtr<USaveGameManager> m_pSaveGameManager;
#pragma endregion

#pragma region | Each event functions
	/**
	 * @brief Do the dialog event.
	 */
	void DoDialogEvent(FTriggerData _oEventData);

	/**
	 * @brief Do the permanent event.
	 */
	void DoPermanentEvent(FTriggerData _oEventData);

	/**
	 * @brief Do the temporal event.
	 */
	void DoTemporalEvent(FTriggerData _oEventData);

	/**
	 * @brief Do the arrow event.
	 */
	void DoArrowEvent(FTriggerData _oEventData);

	/**
	 * @brief Do the arrow event.
	 */
	void DoArrowHideEvent(FTriggerData _oEventData);

	/**
	 * @brief Do teh reveal event.
	 */
	void DoRevealEvent(FTriggerData _oEventData);
#pragma endregion

#pragma region | Private General Event
	/**
	 * @brief What to do when a event finish.
	 */
	void OnEventFinish();

	void ResetEventState();
#pragma endregion

#pragma region | Private dialog function	
	/**
	 * @brief What hapends in the event manager when a dialog finish.
	 */
	UFUNCTION()
	void OnDialogueFinish();
#pragma endregion

#pragma region | Private permanent widget function
	/**
	 * @brief What happends when a permanent widget is skip.
	 */
	void OnSkipPermanentWidget();
#pragma endregion

#pragma region | Private tempotal widget function
	void OnTemporalWidgetFinish();
#pragma endregion

#pragma region | Audio
	/**
	 * @brief
	 */
	void PlaySilentKillerEvent();
#pragma endregion
};
