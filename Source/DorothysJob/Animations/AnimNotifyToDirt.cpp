#include "AnimNotifyToDirt.h"
#include "DorothysJob/Animations/Enemy/EnemyAnimInstance.h"

void UAnimNotifyToDirt::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
  UEnemyAnimInstance* pAnimInstance = Cast<UEnemyAnimInstance>(MeshComp->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->OnToDirtAction.ExecuteIfBound();
  }
}