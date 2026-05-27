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

// Components
class UFMODAudioComponent;

UCLASS()
class DOROTHYSJOB_API UFMODAutoDestroyer : public UObject
{
	GENERATED_BODY()
	
public:
  /**
   * @brief Initializes the auto-destroyer by storing a reference to the provided FMOD audio component and binding to its OnEventStopped event.
   * @param _pAudioComponent The FMOD audio component that this auto-destroyer will manage. 
   */
  void Init(UFMODAudioComponent* _pAudioComponent);

private:
  /**
   * @brief The FMOD audio component that this auto-destroyer will manage.
   */
  UPROPERTY()
  TObjectPtr<UFMODAudioComponent> pAudioComponent;

  /**
   * @brief Called when the associated audio event is stopped.
   */
  UFUNCTION()
  void OnStopped();
};