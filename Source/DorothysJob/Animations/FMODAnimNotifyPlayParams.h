/************************************************************************
 * @description: To FMOD notification with params class
 * @author: David Gonzalez
 * @date: 27/09/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "FMODAnimNotifyPlay.h"
#include "FMODAnimNotifyPlayParams.generated.h"

UCLASS()
class DOROTHYSJOB_API UFMODAnimNotifyPlayParams : public UFMODAnimNotifyPlay
{
	GENERATED_BODY()

public:
  /**
   * It is the custom pitch tag for the event.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Custom Pitch Tag"))
  FName m_sCustomPitchTag;

  /**
   * It is the custom pitch value for the event.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Custom Pitch Value"))
  float m_fCustomPitchValue;

  /**
   * It is the class to apply the pitch tag value.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Enemy Class"))
  TSubclassOf<class ABaseEnemy> m_cEnemyClass;

  virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* AnimSeq) override;
};