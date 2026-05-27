#include "FMODAnimNotifyPlayParams.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

void UFMODAnimNotifyPlayParams::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSeq)
{
  if (!IsValid(Event))
  {
    return;
  }

  FFMODEventInstance tEventInstance = UFMODBlueprintStatics::PlayEventAtLocation(MeshComp, Event, MeshComp->GetComponentTransform(), true);

  AActor* pOwner = MeshComp->GetOwner();
  if (IsValid(pOwner) && m_cEnemyClass)
  {
    if (pOwner->IsA(m_cEnemyClass) && !m_sCustomPitchTag.IsNone())
    {
      UFMODBlueprintStatics::EventInstanceSetParameter(tEventInstance, m_sCustomPitchTag, m_fCustomPitchValue);
    }
  }
}