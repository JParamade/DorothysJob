#include "EnemyAnimInstance.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Data/DataAssets/CharacterPropertiesDataAsset.h"
#include "DorothysJob/Data/DataAssets/Enemy/EnemyPropertiesDataAsset.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
  Super::NativeInitializeAnimation();

  APawn* pOwner = TryGetPawnOwner();

  if (IsValid(pOwner))
  {
    m_pEnemy = Cast<ABaseEnemy>(pOwner);
    if (IsValid(m_pEnemy) && m_pEnemy->GetDataAsset())
    {
      UEnemyPropertiesDataAsset* pEnemyPropertiesDA = Cast<UEnemyPropertiesDataAsset>(m_pEnemy->GetDataAsset());
      m_pActionMontage = IsValid(pEnemyPropertiesDA) ? pEnemyPropertiesDA->m_pActionMontage : nullptr;
    }
  }
}

void UEnemyAnimInstance::NativeUpdateAnimation(float _fDeltaSeconds)
{
  Super::NativeUpdateAnimation(_fDeltaSeconds);

  if (IsValid(m_pEnemy) && IsValid(m_pEnemy->GetMovementComponent()))
  {
    UFloatingPawnMovement* pFloatingMovement = Cast<UFloatingPawnMovement>(m_pEnemy->GetMovementComponent());
    if (IsValid(pFloatingMovement))
    {
      m_fSpeed = m_pEnemy->IsInMovement() ? pFloatingMovement->MaxSpeed : 0;
    }
  }
}