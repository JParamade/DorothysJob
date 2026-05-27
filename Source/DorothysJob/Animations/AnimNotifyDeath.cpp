#include "AnimNotifyDeath.h"
#include "DorothysJob/Animations/DecoyAnimInstance.h"

void UAnimNotifyDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
  UDecoyAnimInstance* pAnimInstance = Cast<UDecoyAnimInstance>(MeshComp->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->OnActionMontageEnds.ExecuteIfBound();
  }
}