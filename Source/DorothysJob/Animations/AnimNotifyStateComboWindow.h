/************************************************************************
 * @description: Combo window notifications class
 * @author: David Gonzalez
 * @date: 06/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyStateComboWindow.generated.h"

UCLASS()
class DOROTHYSJOB_API UAnimNotifyStateComboWindow : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

  /**
   * It notifies when the combo window is open.
   */
  virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

  /**
   * It notifies when the combo window is closed.
   */
  virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};