/************************************************************************
 * @description: Represents the input icon shown depending on the last input device used.
 * @author: Jaime Paramo
 * @date: 25/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "InputIcon.h"

// Component
#include "Components/Image.h"

// Controller
#include "DorothysJob/Controller/BasePlayerController.h"

// Input
#include "GameFramework/InputDeviceSubsystem.h"

// Gameplay
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"

void UInputIcon::Show() {
  Super::Show();

  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    m_oDelegateHandle = pPlayerController->m_oOnDeviceChanged.AddUObject(this, &UInputIcon::UpdateInputIcon);
  }
}

void UInputIcon::Hide() {
  Super::Hide();

  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    pPlayerController->m_oOnDeviceChanged.Remove(m_oDelegateHandle);
  }
}

void UInputIcon::NativeConstruct() {
  Super::NativeConstruct();

  if (IsValid(m_pInputIcon)) m_lElements.Add(m_pInputIcon);
}

void UInputIcon::UpdateInputIcon(EHardwareDevicePrimaryType _eDeviceType) {
  if (IsValid(m_pInputIcon)) {
    switch (_eDeviceType) {
      case EHardwareDevicePrimaryType::KeyboardAndMouse: {
        m_pInputIcon->SetBrushFromTexture(m_pKeyboardIcon);
        break;
      }
      case EHardwareDevicePrimaryType::Gamepad: {
        m_pInputIcon->SetBrushFromTexture(m_pGamepadIcon);
        break;
      }
      default: {
        m_pInputIcon->SetBrushFromTexture(m_pKeyboardIcon);
        break;
      }
    }
  }
}