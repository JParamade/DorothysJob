/************************************************************************
 * @description: Represents the FMOD Audio Component destroy wrapper.
 * @author: Jaime Paramo
 * @date: 29/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FMODAutoDestroyer.generated.h"

class UFMODAudioComponent;

UCLASS()
class DOROTHYSJOB_API UFMODAutoDestroyer : public UObject
{
	GENERATED_BODY()
	
public:
  void Init(UFMODAudioComponent* _pAudioComponent);

private:
  UPROPERTY()
  TObjectPtr<UFMODAudioComponent> pAudioComponent;

  UFUNCTION()
  void OnStopped();
};