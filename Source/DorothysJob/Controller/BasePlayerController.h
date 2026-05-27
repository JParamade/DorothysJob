/************************************************************************
 * @description: Represents the Base Player Controller in the Game
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Jaime Paramo - 10/04/2025
 *						 Josephine Esposito - 19/09/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

DECLARE_DELEGATE(FEventAction)

UENUM(BlueprintType)
enum class EPlayerInputMode : uint8 {
  GAMEPLAY UMETA(DisplayName = "Gameplay"),
  UI		 UMETA(DisplayName = "UI"),
  EVENT		 UMETA(DisplayName = "Event"),
};

UENUM(BlueprintType)
enum class ENavigationInputType : uint8 {
  FOCUS UMETA(DisplayName = "Keyboard/Gamepad"),
  FREE  UMETA(DisplayName = "Mouse"),
};

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UGeneralFocusManager;
class UMouseIndicator;
enum class EHardwareDevicePrimaryType : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeviceChanged, EHardwareDevicePrimaryType)

UCLASS()
class DOROTHYSJOB_API ABasePlayerController : public APlayerController {
  GENERATED_BODY()

public:
  /**
   * @brief Initializes the player controller, setting up input bindings and default states.
   */
  virtual void BeginPlay() override;

  /**
   * @brief Handles per-frame updates for the player controller, such as input processing and UI updates.
   * @param _fDeltaTime Time elapsed since the last frame, used for time-based calculations.
   */
  virtual void Tick(float _fDeltaTime) override;

  /**
   * @brief Switches the player's input mapping context (UI or Gameplay).
   * @param _ePlayerInputMode New input mapping context to set.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Input", DisplayName = "Switch Input Mode")
  void SwitchInputMode(EPlayerInputMode _ePlayerInputMode);
  /**
   * @brief Switches the UI navigation mode.
   * @param _ePlayerInputMode New navigation mode to set.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Navigation", DisplayName = "Switch Navigation Mode")
  void SwitchNavigationMode(ENavigationInputType _eNavigationInputMode);

  /**
   * @brief Returns a pointer to the General Focus Manager.
   * @return Pointer to the General Focus Manager.
   */
  TObjectPtr<UGeneralFocusManager> GetGeneralFocusManager();

  /**
   * @brief Represents the mouse indicator widget, which will be used to show the mouse position when navigating through UI with mouse input.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|UI", meta = (DisplayName = "Mouse Indicator"))
  UMouseIndicator* m_pMouseIndicator;

  /**
   * @brief Event triggered when the player initiates a dialogue skip.
   */
  FEventAction m_oOnDialogueSkipStarts;

  /**
   * @brief	Event triggered when the player ends a dialogue skip, either by confirming the skip or by releasing the skip button.
   */
  FEventAction m_oOnDialogueSkipEnds;

  /**
   * @brief Event triggered when the player initiates a cinematic skip.
   */
  FEventAction m_oOnCinematicSkip;

  /**
   * @brief Event triggered when the player initiates a cinematic skip.
   */
  FEventAction m_oOnCinematicSkipStarts;

  /**
   * @brief Event triggered when the player ends a cinematic skip, either by confirming the skip or by releasing the skip button.
   */
  FEventAction m_oOnCinematicSkipEnds;

  /**
   * @brief Event triggered when the player changes their input device (e.g. from keyboard to gamepad or vice versa).
   */
  FOnDeviceChanged m_oOnDeviceChanged;

  /**
   * @brief Sets whether the cinematic controller is active, which affects input handling and UI behavior during cinematics.
   */
  UFUNCTION(BlueprintCallable)
  void SetCinematicController(bool _bIsCinematicOn);

private:
  /**
   * @brief Whether the cinematic controller is active.
   */
  bool m_bIsCinematicOn = false;

  /**
   * @brief Initial input mode when the game starts (Gameplay or UI).
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Start Input Mode"))
  EPlayerInputMode m_eStartInputMode;

  /**
   * @brief Current input mode (Gameplay, UI or Event).
   */
  ENavigationInputType m_eCurrentNavigationMode;


  /**
   * @brief Input Mapping Context used when interacting with UI.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "UI Input Mapping Context"))
  TObjectPtr<UInputMappingContext> m_pUIIMC;

  /**
   * @brief Input Mapping Context used during gameplay.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Gameplay Input Mapping Context"))
  TObjectPtr<UInputMappingContext> m_pGameplayIMC;

  /**
   * @brief Input Mapping Context used during gameplay.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Dialogue Input Mapping Context"))
  TObjectPtr<UInputMappingContext> m_pDialogueIMC;

  /**
   * @brief Navigation Input Action, used for navigating through UI elements with keyboard/gamepad input.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Navigate Action"))
  TObjectPtr<UInputAction> m_pNavigateAction;
  /**
   * @brief Confirm Input Action, used for confirming selections in UI or dialogues.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Confirm Action"))
  TObjectPtr<UInputAction> m_pConfirmAction;
  /**
   * @brief Cancel Input Action, used for canceling selections in UI or dialogues.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Cancel Action"))
  TObjectPtr<UInputAction> m_pCancelAction;
  /**
   * @brief Mouse Movement Input Action, used for moving the mouse cursor within the game.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Mouse Movement Action"))
  TObjectPtr<UInputAction> m_pMouseMovement;
  /**
   * @brief Dialogue Confirm Input Action, used for confirming selections in dialogues.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Dialogue Confirm"))
  TObjectPtr<UInputAction> m_pEventConfirm;
  /**
   * @brief Dialogue Skip Input Action, used for skipping dialogues.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Dialogue Skip"))
  TObjectPtr<UInputAction> m_pDialogueSkip;

  /**
   * @brief Pause Input Action, used for pausing the game.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Pause Action"))
  TObjectPtr<UInputAction> m_pPauseAction;

  /**
   * @brief Change Tab Input Action, used for changing tabs in the UI.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Change Tab Action"))
  TObjectPtr<UInputAction> m_pChangeTabAction;

  /**
   * @brief Handles navigation input for UI elements, allowing the player to move focus between interactable widgets using keyboard or gamepad input.
   * @param _rIAValue Value generated for the navigation input.
   */
  void HandleNavigation(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles confirm input for UI elements, allowing the player to confirm selections in the UI or dialogues using keyboard or gamepad input.
   * @param _rIAValue Value generated for the confirm input.
   */
  void HandleConfirmPressed(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles confirm input release for UI elements, allowing the player to end confirm actions in the UI or dialogues using keyboard or gamepad input.
   * @param _rIAValue Value generated for the confirm input release.
   */
  void HandleConfirmReleased(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles cancel input for UI elements, allowing the player to cancel selections in the UI or dialogues using keyboard or gamepad input.
   * @param _rIAValue Value generated for the cancel input.
   */
  void HandleCancel(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles mouse movement input, allowing the player to move the mouse cursor within the game and interact with UI elements using mouse input.
   * @param _rIAValue Value generated for the mouse movement input.
   */
  void HandleMouseMovement(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles confirm input for dialogues, allowing the player to confirm selections in dialogues using keyboard or gamepad input.
   * @param _rIAValue Value generated for the dialogue confirm input.
   */
  void HandleEventNext(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles the start of dialogue skipping.
   * @param _rIAValue Value generated for the dialogue skip input.
   */
  void HandleDialogueStartSkip(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles the end of dialogue skipping.
   * @param _rIAValue Value generated for the dialogue skip input.
   */
  void HandleDialogueEndSkip(const FInputActionValue& _rIAValue);
  /**
   * @brief Handles dialogue skipping.
   * @param _rIAValue Value generated for the dialogue skip input.
   */
  void HandleDialogueSkip(const FInputActionValue& _rIAValue);

  /**
   * @brief It's called for pause input.
   * @param _tIAValue -> Value generated for the input.
   */
  void HandlePauseKey(const FInputActionValue& _tIAValue);

  /**
   * @brief It's called for change tab input.
   * @param _tIAValue -> Value generated for the input.
   */
  void HandleChangeTabKey(const FInputActionValue& _tIAValue);

  /**
   * @brief Reference to the General Focus Manager, which is responsible for managing focus between UI elements when navigating with keyboard or gamepad input.
   */
  UPROPERTY()
  TObjectPtr<UGeneralFocusManager> pGeneralFocusManager;

  /**
   * @brief Whether the mouse indicator can be shown or not.
   */
  UPROPERTY()
  bool m_bCanShowIndicator;
};