/************************************************************************
 * @description: Represents the enemy status World Space Widget.
 * @author: Jaime Paramo
 * @date: 17/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "WorldStatus.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"

 // Component
#include "Components/Image.h"

// Status
#include "DorothysJob/Status/BaseStatus.h"
#include "DorothysJob/Status/Dizzy/DizzyEffect.h"

void UWorldStatus::InitWidget(ABaseCharacter* _pOwner)
{
  if (!IsValid(_pOwner))
    return;

  m_pOwnerCharacter = _pOwner;

}

void UWorldStatus::Show() {
  Super::Show();
  m_pOwnerCharacter->OnStateChanged.AddDynamic(this, &UWorldStatus::UpdateStatusInfo);

}

void UWorldStatus::Hide() {
  Super::Hide();
  m_pOwnerCharacter->OnStateChanged.RemoveAll(this);
}

void UWorldStatus::NativeConstruct() {
  Super::NativeConstruct();
}

void UWorldStatus::UpdateStatusInfo(UBaseStatus* _pStatus, bool _bActive) {
  switch (_pStatus->GetStatusType()) {
    case EStatus::DIZZY:
      UE_LOG(LogTemp, Warning, TEXT("[STATUS]: Applied dizzyness effect!"));
      break;
    case EStatus::EXPOSURE: if (IsValid(m_pExposureStatusImage)) {
      m_pExposureStatusImage->SetVisibility(_bActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
      break;
    }
    case EStatus::SLOW: {
      if (IsValid(m_pSlowStatusImage)) {
        m_pSlowStatusImage->SetVisibility(_bActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
        m_iAuxStacks++;

        if (m_iAuxStacks == 1) m_pSlowStatusImage->SetBrushFromTexture(m_lDizzyTierTextures[0]);
        else if (m_iAuxStacks == 2) {
          m_pSlowStatusImage->SetBrushFromTexture(m_lDizzyTierTextures[1]);
          m_iAuxStacks = 0;
        }
      }

      break;
    }
    case EStatus::STUN: {
     
      if (IsValid(m_pSlowStatusImage)) m_pSlowStatusImage->SetVisibility(ESlateVisibility::Collapsed);
   
      break;
    }
    default: {
      UE_LOG(LogTemp, Warning, TEXT("[STATUS]: Status effect is not recognized."));
      break;
    }
  }
}