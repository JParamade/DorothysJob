#include "AnimNotifyStateAction.h"
#include "DorothysJob/Animations/BaseAnimInstance.h"

void UAnimNotifyStateAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
  UBaseAnimInstance* pAnimInstance = Cast<UBaseAnimInstance>(MeshComp->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->OnActionStarts.ExecuteIfBound(pAnimInstance->GetCurrentComboIndex());
  }
}

void UAnimNotifyStateAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
  UBaseAnimInstance* pAnimInstance = Cast<UBaseAnimInstance>(MeshComp->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->OnActionEnds.ExecuteIfBound();
  }
}