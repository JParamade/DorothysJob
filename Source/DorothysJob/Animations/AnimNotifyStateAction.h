/************************************************************************
 * @description: Action notifications class
 * @author: David Gonzalez
 * @date: 06/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyStateAction.generated.h"

UCLASS()
class DOROTHYSJOB_API UAnimNotifyStateAction : public UAnimNotifyState
{
	GENERATED_BODY()
	
  /**
   * It notifies when the colliders should be enabled for the action or the one shot action should be performed.
   */
  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

  /**
   * It notifies when the colliders should be disabled.
   */
  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};