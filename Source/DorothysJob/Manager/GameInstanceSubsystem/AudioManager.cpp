/************************************************************************
 * @description: Represents the Audio Manager class.
 * @author: Jaime Paramo
 * @date: 07/07/2025
 * @edited_by:
 ************************************************************************/

#include "AudioManager.h"

 // Game Instance
#include "DorothysJob/Manager/BaseGameInstance.h"

// FMOD
#include "DorothysJob/Audio/FMODAutoDestroyer.h"
#include "FMODAudioComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "FMODUtils.h"

void UAudioManager::Initialize(FSubsystemCollectionBase& _rCollection) {
  Super::Initialize(_rCollection);
}

void UAudioManager::PlaySound2D(UObject* _pWorldContextObject, UFMODEvent* _pEvent) {
  // Validate input parameters.
  if (!_pWorldContextObject && !_pEvent) return;

  // Play 2D sound using FMOD Blueprint statics.
  UFMODBlueprintStatics::PlayEvent2D(
    _pWorldContextObject,
    _pEvent,
    true
  );
}

void UAudioManager::PlaySoundAtLocation(UObject* _pWorldContextObject, UFMODEvent* _pEvent, FVector _vLocation) {
  // Validate input parameters.
  if (!_pWorldContextObject && !_pEvent) return;

  // Play sound at location using FMOD Blueprint statics.
  UFMODBlueprintStatics::PlayEventAtLocation(
    _pWorldContextObject,
    _pEvent,
    FTransform(_vLocation),
    true
  );
}

void UAudioManager::PlaySoundAttached(UFMODEvent* _pEvent, USceneComponent* _pAttachToComponent) {
  // Validate input parameters.
  if (!_pEvent && !_pAttachToComponent) return;

  // Play attached sound using FMOD Blueprint statics.
  UFMODBlueprintStatics::PlayEventAttached(
    _pEvent,
    _pAttachToComponent,
    NAME_None,
    FVector::ZeroVector,
    EAttachLocation::KeepRelativeOffset,
    true,
    true,
    true
  );
}

void UAudioManager::PlayEventOnComponent(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent) {
  // Validate input parameters.
  if (!_pAudioComponent && !_pEvent) return;
  // Stop and release the audio component before setting the new event and playing it.
  _pAudioComponent->Stop();
  _pAudioComponent->Release();
  _pAudioComponent->SetEvent(_pEvent);
  _pAudioComponent->Play();
}

void UAudioManager::PlayEventWithParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters) {
  // Validate input parameters.
  if (!_pAudioComponent && !_pEvent) return;

  // Stop and release the audio component.
  _pAudioComponent->Stop();
  _pAudioComponent->Release();
  _pAudioComponent->SetEvent(_pEvent);

  // Set parameters on the audio component before playing it.
  for (const FAudioParam& rParams : _rParameters) {
    if (!rParams.sName.IsNone()) _pAudioComponent->SetParameter(rParams.sName, rParams.fValue);
  }

  // Play the audio component.
  _pAudioComponent->Play();
}

void UAudioManager::PlayAttachedEvent(AActor* _pOwner, UFMODEvent* _pEvent) {
  // Validate input parameters.
  if (!_pOwner && !_pEvent) return;

  UFMODAudioComponent* pAudioComponent = NewObject<UFMODAudioComponent>(_pOwner);
  if (!pAudioComponent) return;

  // Register and attach the audio component to the owner's root component.
  pAudioComponent->RegisterComponent();
  pAudioComponent->AttachToComponent(
    _pOwner->GetRootComponent(),
    FAttachmentTransformRules::KeepRelativeTransform
  );

  // Set the event on the audio component and play it.
  pAudioComponent->SetEvent(_pEvent);
  pAudioComponent->Play();

  // Create an auto-destroyer for the audio component to ensure it gets cleaned up after the sound finishes playing.
  UFMODAutoDestroyer* pAutoDestroyer = NewObject<UFMODAutoDestroyer>();
  pAutoDestroyer->Init(pAudioComponent);
}

void UAudioManager::SetComponentParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters) {
  // Validate input parameters.
  if (!_pAudioComponent && !_pEvent) return;

  // Stop and release the audio component before setting the new event and parameters.
  for (const FAudioParam& rParams : _rParameters) {
    if (!rParams.sName.IsNone()) _pAudioComponent->SetParameter(rParams.sName, rParams.fValue);
  }
}

void UAudioManager::PlayEventInstanceWithParameters(UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters) {
  // Validate input parameters.
  if (!_pEvent) return;

  // Get the FMOD Studio system and event description for the specified event, then create an instance of the event, set the parameters, and play it.
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
  if (!pStudioSystem) return;
  
  // Convert the event's asset GUID to an FMOD GUID and get the event description from the FMOD Studio system.
  FMOD_GUID oGuid = FMODUtils::ConvertGuid(_pEvent->AssetGuid);

  FMOD::Studio::EventDescription* pEventDesc = nullptr;
  if (FMOD_OK != pStudioSystem->getEventByID(&oGuid, &pEventDesc) || !pEventDesc) return;

  // Create an instance of the event, set the parameters on the instance, start it, and release it.
  FMOD::Studio::EventInstance* pEventInstance = nullptr;
  if (FMOD_OK == pEventDesc->createInstance(&pEventInstance) && pEventInstance) {
    for (const FAudioParam& rParam : _rParameters) {
      pEventInstance->setParameterByName(
        TCHAR_TO_UTF8(*rParam.sName.ToString()), 
        rParam.fValue
      );
    }

    pEventInstance->start();
    pEventInstance->release();
  }
}

FMOD::Studio::EventInstance* UAudioManager::PlayEventInstance(UFMODEvent* _pEvent) {
  // Validate input parameters.
  if (!_pEvent) return nullptr;

  // Get the FMOD Studio system and event description for the specified event.
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
  if (!pStudioSystem) return nullptr;

  // Convert the event's asset GUID to an FMOD GUID and get the event description from the FMOD Studio system.
  FMOD_GUID oGuid = FMODUtils::ConvertGuid(_pEvent->AssetGuid);

  FMOD::Studio::EventDescription* pEventDesc = nullptr;
  if (FMOD_OK != pStudioSystem->getEventByID(&oGuid, &pEventDesc) || !pEventDesc) return nullptr;

  // Create an instance of the event, start it, and return the instance.
  FMOD::Studio::EventInstance* pEventInstance = nullptr;
  if (FMOD_OK == pEventDesc->createInstance(&pEventInstance) && pEventInstance) pEventInstance->start();

  return pEventInstance;
}

void UAudioManager::SetGlobalParameter(const TArray<FAudioParam>& _rParameters) {
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
  if (!pStudioSystem) return;

  // Set each parameter in the array on the FMOD Studio system.
  for (const FAudioParam& rParam : _rParameters) {
    pStudioSystem->setParameterByName(
      TCHAR_TO_UTF8(*rParam.sName.ToString()),
      rParam.fValue
    );
  }
}

void UAudioManager::SetParameterInterpolated(UFMODAudioComponent* _pAudioComponent, FName _sParameterName, float& _fCurrentValue, float _fTargetValue, float _fInterpolationSpeed, float _fDeltaTime) {
  // Interpolate the current value towards the target value using a simple linear interpolation function.
  _fCurrentValue = FMath::FInterpTo(_fCurrentValue, _fTargetValue, _fDeltaTime, _fInterpolationSpeed);

  // Set the interpolated value on the audio component for the specified parameter name.
  _pAudioComponent->SetParameter(_sParameterName, _fCurrentValue);
}
