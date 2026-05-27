/************************************************************************
 * @description: Represents the FMOD Audio Component destroy wrapper.
 * @author: Jaime Paramo
 * @date: 29/08/2025
 * @edited_by:
 ************************************************************************/

#include "FMODAutoDestroyer.h"

// FMOD
#include "FMODAudioComponent.h"

void UFMODAutoDestroyer::Init(UFMODAudioComponent* _pAudioComponent) {
  pAudioComponent = _pAudioComponent;

  if (IsValid(pAudioComponent)) pAudioComponent->OnEventStopped.AddDynamic(this, &UFMODAutoDestroyer::OnStopped);
}

void UFMODAutoDestroyer::OnStopped() {
  if (IsValid(pAudioComponent)) {
    pAudioComponent->DestroyComponent();

    ConditionalBeginDestroy();
  }
}