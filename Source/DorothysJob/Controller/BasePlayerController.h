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
	 * @brief 
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief 
	 * @param _fDeltaTime 
	 */
	virtual void Tick(float _fDeltaTime) override;

	/**
	 * @brief -> Switches the player's input mapping context (UI or Gameplay).
	 * @param _ePlayerInputMode -> New input mapping context to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Bola 13|Input", DisplayName = "Switch Input Mode")
	void SwitchInputMode(EPlayerInputMode _ePlayerInputMode);
	/**
	 * @brief -> Switches the UI navigation mode.
	 * @param _ePlayerInputMode -> New navigation mode to set.
	 */
	UFUNCTION(BlueprintCallable, Category = "Bola 13|Navigation", DisplayName = "Switch Navigation Mode")
	void SwitchNavigationMode(ENavigationInputType _eNavigationInputMode);

	/**
	 * @brief 
	 * @return 
	 */
	TObjectPtr<UGeneralFocusManager> GetGeneralFocusManager();

	/**
	 * @brief Represents the mouse indicator widget
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|UI", meta = (DisplayName = "Mouse Indicator"))
	UMouseIndicator* m_pMouseIndicator;
	
	/**
	 * @brief 
	 */
	FEventAction m_oOnDialogueSkipStarts;

	/**
	 * @brief 
	 */
	FEventAction m_oOnDialogueSkipEnds;

	/**
	 * @brief
	 */
	FEventAction m_oOnCinematicSkip;

	/**
	 * @brief
	 */
	FEventAction m_oOnCinematicSkipStarts;

	/**
	 * @brief
	 */
	FEventAction m_oOnCinematicSkipEnds;

	/**
	 * @brief 
	 */
	FOnDeviceChanged m_oOnDeviceChanged;

	/**
	 * @brief 
	 */
	UFUNCTION(BlueprintCallable)
	void SetCinematicController(bool _bIsCinematicOn);
private:

	bool m_bIsCinematicOn = false;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Start Input Mode"))
	EPlayerInputMode m_eStartInputMode;

	/**
	 * @brief 
	 */
	ENavigationInputType m_eCurrentNavigationMode;


	/**
	 * @brief -> Input Mapping Context used when interacting with UI.
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "UI Input Mapping Context"))
	TObjectPtr<UInputMappingContext> m_pUIIMC;

	/**
	 * @brief -> Input Mapping Context used during gameplay.
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Gameplay Input Mapping Context"))
	TObjectPtr<UInputMappingContext> m_pGameplayIMC;
	
	/**
	 * @brief -> Input Mapping Context used during gameplay.
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Dialogue Input Mapping Context"))
	TObjectPtr<UInputMappingContext> m_pDialogueIMC;

	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Navigate Action"))
	TObjectPtr<UInputAction> m_pNavigateAction;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Confirm Action"))
	TObjectPtr<UInputAction> m_pConfirmAction;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Cancel Action"))
	TObjectPtr<UInputAction> m_pCancelAction;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Mouse Movement Action"))
	TObjectPtr<UInputAction> m_pMouseMovement;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Dialogue Confirm"))
	TObjectPtr<UInputAction> m_pEventConfirm;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Dialogue Skip"))
	TObjectPtr<UInputAction> m_pDialogueSkip;

  /**
   * Pause Input Action
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Pause Action"))
  TObjectPtr<UInputAction> m_pPauseAction;

  /**
   * Change Tab Input Action
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Input", meta = (DisplayName = "Change Tab Action"))
  TObjectPtr<UInputAction> m_pChangeTabAction;

	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleNavigation(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleConfirmPressed(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleConfirmReleased(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleCancel(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleMouseMovement(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleEventNext(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleDialogueStartSkip(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
	 */
	void HandleDialogueEndSkip(const FInputActionValue& _rIAValue);
	/**
	 * @brief 
	 * @param _rIAValue 
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
	 * @brief 
	 */
	UPROPERTY()
	TObjectPtr<UGeneralFocusManager> pGeneralFocusManager;

	/**
	 * @brief 
	 */
	UPROPERTY()
	bool m_bCanShowIndicator;
};