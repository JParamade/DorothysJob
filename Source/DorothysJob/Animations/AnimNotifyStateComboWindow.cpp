#include "AnimNotifyStateComboWindow.h"
#include "DorothysJob/Animations/BaseAnimInstance.h"

void UAnimNotifyStateComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
  UBaseAnimInstance* pAnimInstance = Cast<UBaseAnimInstance>(MeshComp->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->SetComboWindowOpen(true);
  }
}

void UAnimNotifyStateComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
  UBaseAnimInstance* pAnimInstance = Cast<UBaseAnimInstance>(MeshComp->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->SetComboWindowOpen(false);
  }
}