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
  // Store the reference to the audio component and bind to its OnEventStopped event.
  pAudioComponent = _pAudioComponent;

  // If the audio component is valid, bind the OnStopped function to the OnEventStopped event.
  if (IsValid(pAudioComponent)) pAudioComponent->OnEventStopped.AddDynamic(this, &UFMODAutoDestroyer::OnStopped);
}

void UFMODAutoDestroyer::OnStopped() {
  if (IsValid(pAudioComponent)) {
    // Unbind the OnStopped function from the OnEventStopped event to prevent potential dangling references.
    pAudioComponent->DestroyComponent();

    // Mark this object for garbage collection, as it is no longer needed after the audio component has been destroyed.
    ConditionalBeginDestroy();
  }
}