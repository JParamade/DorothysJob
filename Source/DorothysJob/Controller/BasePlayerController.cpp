/************************************************************************
 * @description: Represents the Base Player Controller in the Game
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Jaime Paramo - 10/04/2025
 ************************************************************************/

#include "BasePlayerController.h"

#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Utils/MouseIndicator.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Dialogs/DialogsManager.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "Components/Image.h"
#include "GameFramework/InputDeviceSubsystem.h"

#include "Framework/Application/SlateApplication.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void ABasePlayerController::BeginPlay() {
  Super::BeginPlay();

  if (UEnhancedInputComponent* pEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
    pEnhancedInputComponent->BindAction(m_pNavigateAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleNavigation);
    pEnhancedInputComponent->BindAction(m_pConfirmAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleConfirmPressed);
    pEnhancedInputComponent->BindAction(m_pConfirmAction, ETriggerEvent::Completed, this, &ABasePlayerController::HandleConfirmReleased);
    pEnhancedInputComponent->BindAction(m_pCancelAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleCancel);
    pEnhancedInputComponent->BindAction(m_pMouseMovement, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleMouseMovement);
    pEnhancedInputComponent->BindAction(m_pEventConfirm, ETriggerEvent::Started, this, &ABasePlayerController::HandleEventNext);
    pEnhancedInputComponent->BindAction(m_pDialogueSkip, ETriggerEvent::Started, this, &ABasePlayerController::HandleDialogueStartSkip);
    pEnhancedInputComponent->BindAction(m_pDialogueSkip, ETriggerEvent::Canceled, this, &ABasePlayerController::HandleDialogueEndSkip);
    pEnhancedInputComponent->BindAction(m_pDialogueSkip, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleDialogueSkip);
    pEnhancedInputComponent->BindAction(m_pPauseAction, ETriggerEvent::Completed, this, &ABasePlayerController::HandlePauseKey);
    pEnhancedInputComponent->BindAction(m_pChangeTabAction, ETriggerEvent::Triggered, this, &ABasePlayerController::HandleChangeTabKey);
  }

  SwitchInputMode(m_eStartInputMode);

  if (IsValid(m_pMouseIndicator)) m_pMouseIndicator->AddToViewport(3);
}

void ABasePlayerController::Tick(float _fDeltaTime) {
  Super::Tick(_fDeltaTime);

  FVector2D mousePosition;

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    UInputDeviceSubsystem* pInputDeviceSubsystem = pGameInstance->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>();
    if (IsValid(pInputDeviceSubsystem)) {
      EHardwareDevicePrimaryType oDevice = pInputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(GetPlatformUserId()).PrimaryDeviceType;
      m_bCanShowIndicator = !(oDevice == EHardwareDevicePrimaryType::Gamepad);
      m_oOnDeviceChanged.Broadcast(oDevice);
    }
  }

  // m_bCanShowIndicator = (FSlateApplication::IsInitialized() && FSlateApplication::Get().IsGamepadAttached()) ? false : true;

  if (!m_bCanShowIndicator) { 
    // we won't show the indicator
    m_pMouseIndicator->Hide();
    return;
  }

  m_pMouseIndicator->Show();

  if (!GetMousePosition(mousePosition.X, mousePosition.Y)) { 
    // we did not get the mouse position
    return;
  }

  if (m_pMouseIndicator) {
    m_pMouseIndicator->SetPositionInViewport(mousePosition, true);
    m_pMouseIndicator->SetAlignmentInViewport(FVector2D(0.0f, 0.0f));
  }

  SetShowMouseCursor(false);
  CurrentMouseCursor = EMouseCursor::None;
}

void ABasePlayerController::SwitchInputMode(EPlayerInputMode _ePlayerInputMode) {
  if (UEnhancedInputLocalPlayerSubsystem* pInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
    pInputSubsystem->ClearAllMappings();

    switch (_ePlayerInputMode) {
      // Set input mode to Game Only and hide the mouse cursor.
    case EPlayerInputMode::GAMEPLAY:
      pInputSubsystem->AddMappingContext(m_pGameplayIMC, 0);
      SetInputMode(FInputModeGameOnly());
      SetShowMouseCursor(false);

      break;
      // Choose default navigation mode:
      //   - If a gamepad is connected, use Focus navigation mode.
      //   - Otherwise, use Free navigation mode (mouse-based).
      // Note: Focus mode can still be accessed via keyboard input, even without a gamepad.
    case EPlayerInputMode::UI:
      pInputSubsystem->AddMappingContext(m_pUIIMC, 0);
      SwitchNavigationMode((FSlateApplication::IsInitialized() && FSlateApplication::Get().IsGamepadAttached()) ? ENavigationInputType::FOCUS : ENavigationInputType::FREE);

      break;
    case EPlayerInputMode::EVENT:
      pInputSubsystem->AddMappingContext(m_pDialogueIMC, 0);
      SetInputMode(FInputModeGameOnly());
      SetShowMouseCursor(false);

      break;
    }
  }
}

void ABasePlayerController::SwitchNavigationMode(ENavigationInputType _eNavigationInputMode) {
  switch (_eNavigationInputMode) {
  case ENavigationInputType::FOCUS:
    SetInputMode(FInputModeGameOnly());
    if (IsValid(pGeneralFocusManager)) pGeneralFocusManager->ActivateFocus();

    break;
  case ENavigationInputType::FREE:
    SetInputMode(FInputModeGameAndUI());
    if (IsValid(pGeneralFocusManager)) pGeneralFocusManager->ResetFocus();

    break;
  }

  m_eCurrentNavigationMode = _eNavigationInputMode;
  m_bCanShowIndicator = (bool)_eNavigationInputMode ? true : false;
  SetShowMouseCursor(false);
}

TObjectPtr<UGeneralFocusManager> ABasePlayerController::GetGeneralFocusManager() {
  // Lazy Initialization
  if (!IsValid(pGeneralFocusManager)) pGeneralFocusManager = NewObject<UGeneralFocusManager>(this);

  return pGeneralFocusManager;
}

void ABasePlayerController::SetCinematicController(bool _bIsCinematicOn) {
  m_bIsCinematicOn = _bIsCinematicOn;
}

void ABasePlayerController::HandleNavigation(const FInputActionValue& _rIAValue) {
  if (!IsValid(pGeneralFocusManager)) return;

  // If currently in Free mode, switch to Focus mode before navigating.
  if (m_eCurrentNavigationMode == ENavigationInputType::FREE)
  {
    SwitchNavigationMode(ENavigationInputType::FOCUS);
    // passing the value to the consumables menu
    ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
    if (IsValid(pLevelManager))
    { // we send the value here
      int32 iDirection = (_rIAValue.Get<FVector2D>().X > 0) ? 1 : (_rIAValue.Get<FVector2D>().X < 0) ? -1 : 0; // direction for the carousel
      pLevelManager->OnReceivedConsumableNavigation.ExecuteIfBound(iDirection);
    }
    return;
  }

  // Proceed with directional navigation if already in Focus mode.
  pGeneralFocusManager->Navigate(_rIAValue.Get<FVector2D>());
}

void ABasePlayerController::HandleConfirmPressed(const FInputActionValue& _rIAValue)
{
  // Only confirm selection when in Focus mode (a button needs to be focused).
  if (m_eCurrentNavigationMode == ENavigationInputType::FREE || !IsValid(pGeneralFocusManager)) return;

  pGeneralFocusManager->ConfirmPressed();
}

void ABasePlayerController::HandleConfirmReleased(const FInputActionValue& _rIAValue) {

  if (ULevelManager* pManager = GetWorld()->GetSubsystem<ULevelManager>())
  {
    pManager->OnReceivedConsumableInput.Broadcast();
  }

  // Only confirm selection when in Focus mode (a button needs to be focused).
  if (m_eCurrentNavigationMode == ENavigationInputType::FREE || !IsValid(pGeneralFocusManager)) return;

  pGeneralFocusManager->ConfirmReleased();
}

void ABasePlayerController::HandleCancel(const FInputActionValue& _rIAValue) {
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return;
  }
  EBaseGameState eGameState = pLevelManager->GetCurrentGameState();
  if ((eGameState == EBaseGameState::SELECTWEAPON) || (eGameState == EBaseGameState::SELECTCONSUMABLE) || (eGameState == EBaseGameState::PRELEVEL) || (eGameState == EBaseGameState::PAUSED) || (eGameState == EBaseGameState::REPLAY))
  {
    if (IsValid(pGeneralFocusManager))
    {
      pGeneralFocusManager->Cancel();
    }

    pLevelManager->OnCancelButtonPressed.ExecuteIfBound(true);
  }
}

void ABasePlayerController::HandleMouseMovement(const FInputActionValue& _rIAValue) {
  FVector2D vDelta = _rIAValue.Get<FVector2D>();
  if (!vDelta.IsNearlyZero()) {
    if (IsValid(pGeneralFocusManager)) SwitchNavigationMode(ENavigationInputType::FREE);
  }
}

void ABasePlayerController::HandleEventNext(const FInputActionValue& _rIAValue)
{
  UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!GameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameIstance no encontrado"));
    return;
  }

  // Coger del event manager del base istance y llamar a siguiente del event manager.
  UEventManager* pEventManager = GameInstance->GetEventManager();
  pEventManager->OnEventContinue();
}

void ABasePlayerController::HandleDialogueStartSkip(const FInputActionValue& _rIAValue) {
  if (m_bIsCinematicOn)
  {
    m_oOnCinematicSkipStarts.ExecuteIfBound();
    return;
  }
  m_oOnDialogueSkipStarts.ExecuteIfBound();
}

void ABasePlayerController::HandleDialogueEndSkip(const FInputActionValue& _rIAValue) {
  if (m_bIsCinematicOn)
  {
    m_oOnCinematicSkipEnds.ExecuteIfBound();
    return;
  }
  m_oOnDialogueSkipEnds.ExecuteIfBound();
}

void ABasePlayerController::HandleDialogueSkip(const FInputActionValue& _rIAValue) {
  UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!GameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameIstance no encontrado"));
    return;
  }

  if (m_bIsCinematicOn)
  {
    m_oOnCinematicSkipEnds.ExecuteIfBound();
    m_oOnCinematicSkip.ExecuteIfBound();
    return;
  }
  // LLamra a eskipear dialogo del event manager.
  UEventManager* pEventManager = GameInstance->GetEventManager();
  if (pEventManager)
  {
    m_oOnDialogueSkipEnds.ExecuteIfBound();

    pEventManager->OnSkipEvent();
  }
}

void ABasePlayerController::HandlePauseKey(const FInputActionValue& _tIAValue)
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (IsValid(pLevelManager) && (pLevelManager->GetCurrentGameState() == EBaseGameState::PAUSED))
  {
    pLevelManager->OnCancelButtonPressed.ExecuteIfBound(true);
  }
}

void ABasePlayerController::HandleChangeTabKey(const FInputActionValue& _tIAValue)
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return;
  }

  pLevelManager->OnChangeTabButtonPressed.ExecuteIfBound(_tIAValue.Get<float>() > 0.f);
}
