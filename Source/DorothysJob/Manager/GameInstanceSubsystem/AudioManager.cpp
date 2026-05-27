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
  if (!_pWorldContextObject && !_pEvent) return;

  UFMODBlueprintStatics::PlayEvent2D(
    _pWorldContextObject,
    _pEvent,
    true
  );
}

void UAudioManager::PlaySoundAtLocation(UObject* _pWorldContextObject, UFMODEvent* _pEvent, FVector _vLocation) {
  if (!_pWorldContextObject && !_pEvent) return;

  UFMODBlueprintStatics::PlayEventAtLocation(
    _pWorldContextObject,
    _pEvent,
    FTransform(_vLocation),
    true
  );
}

void UAudioManager::PlaySoundAttached(UFMODEvent* _pEvent, USceneComponent* _pAttachToComponent) {
  if (!_pEvent && !_pAttachToComponent) return;

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
  if (!_pAudioComponent && !_pEvent) return;
  _pAudioComponent->Stop();
  _pAudioComponent->Release();
  _pAudioComponent->SetEvent(_pEvent);
  _pAudioComponent->Play();
}

void UAudioManager::PlayEventWithParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters) {
  if (!_pAudioComponent && !_pEvent) return;

  _pAudioComponent->Stop();
  _pAudioComponent->Release();
  _pAudioComponent->SetEvent(_pEvent);

  for (const FAudioParam& rParams : _rParameters) {
    if (!rParams.sName.IsNone()) _pAudioComponent->SetParameter(rParams.sName, rParams.fValue);
  }

  _pAudioComponent->Play();
}

void UAudioManager::PlayAttachedEvent(AActor* _pOwner, UFMODEvent* _pEvent) {
  if (!_pOwner && !_pEvent) return;

  UFMODAudioComponent* pAudioComponent = NewObject<UFMODAudioComponent>(_pOwner);
  if (!pAudioComponent) return;

  pAudioComponent->RegisterComponent();
  pAudioComponent->AttachToComponent(
    _pOwner->GetRootComponent(),
    FAttachmentTransformRules::KeepRelativeTransform
  );

  pAudioComponent->SetEvent(_pEvent);
  pAudioComponent->Play();

  UFMODAutoDestroyer* pAutoDestroyer = NewObject<UFMODAutoDestroyer>();
  pAutoDestroyer->Init(pAudioComponent);
}

void UAudioManager::SetComponentParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters) {
  if (!_pAudioComponent && !_pEvent) return;

  for (const FAudioParam& rParams : _rParameters) {
    if (!rParams.sName.IsNone()) _pAudioComponent->SetParameter(rParams.sName, rParams.fValue);
  }
}

void UAudioManager::PlayEventInstanceWithParameters(UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters) {
  if (!_pEvent) return;

  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
  if (!pStudioSystem) return;

  FMOD_GUID oGuid = FMODUtils::ConvertGuid(_pEvent->AssetGuid);

  FMOD::Studio::EventDescription* pEventDesc = nullptr;
  if (FMOD_OK != pStudioSystem->getEventByID(&oGuid, &pEventDesc) || !pEventDesc) return;

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
  if (!_pEvent) return nullptr;

  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
  if (!pStudioSystem) return nullptr;

  FMOD_GUID oGuid = FMODUtils::ConvertGuid(_pEvent->AssetGuid);

  FMOD::Studio::EventDescription* pEventDesc = nullptr;
  if (FMOD_OK != pStudioSystem->getEventByID(&oGuid, &pEventDesc) || !pEventDesc) return nullptr;

  FMOD::Studio::EventInstance* pEventInstance = nullptr;
  if (FMOD_OK == pEventDesc->createInstance(&pEventInstance) && pEventInstance) pEventInstance->start();

  return pEventInstance;
}

void UAudioManager::SetGlobalParameter(const TArray<FAudioParam>& _rParameters) {
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
  if (!pStudioSystem) return;

  for (const FAudioParam& rParam : _rParameters) {
    pStudioSystem->setParameterByName(
      TCHAR_TO_UTF8(*rParam.sName.ToString()),
      rParam.fValue
    );
  }
}

void UAudioManager::SetParameterInterpolated(UFMODAudioComponent* _pAudioComponent, FName _sParameterName, float& _fCurrentValue, float _fTargetValue, float _fInterpolationSpeed, float _fDeltaTime) {
  _fCurrentValue = FMath::FInterpTo(_fCurrentValue, _fTargetValue, _fDeltaTime, _fInterpolationSpeed);

  _pAudioComponent->SetParameter(_sParameterName, _fCurrentValue);
}
