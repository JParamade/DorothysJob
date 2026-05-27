/************************************************************************
* @description: Death notifications class
* @author : David Gonzalez
* @date : 16 / 09 / 2025
* @edited_by: [Other Contributors]
* ***********************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifyDeath.generated.h"

UCLASS()
class DOROTHYSJOB_API UAnimNotifyDeath : public UAnimNotify
{
	GENERATED_BODY()
	
  /**
   * It notifies when the death action should be executed.
   */
  virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
