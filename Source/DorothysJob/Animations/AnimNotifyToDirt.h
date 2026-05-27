/************************************************************************
 * @description: To Dirt notifications class
 * @author: David Gonzalez
 * @date: 14/08/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifyToDirt.generated.h"

UCLASS()
class DOROTHYSJOB_API UAnimNotifyToDirt : public UAnimNotify
{
	GENERATED_BODY()

  /**
   * It notifies when the dirt should be spawned.
   */
  virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};