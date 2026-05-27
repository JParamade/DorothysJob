/************************************************************************
 * @description: Represents the Audio Settings Manager class contained in the Settings Manager.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#include "AudioSettingsManager.h"

// Audio
#include "FMODBlueprintStatics.h"

void UAudioSettingsManager::Initialize() {
  // Initialize FMOD Master Bus (cannot be accessed through Asset).
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Runtime);
  if (pStudioSystem) {
    pStudioSystem->getBus("bus:/", &m_pMasterBus);
    pStudioSystem->getBus("bus:/Music", &m_pMusicBus);
    pStudioSystem->getBus("bus:/SoundFX", &m_pSFXBus);
  }
}

void UAudioSettingsManager::ApplySettings() {
  SetMasterVolume(m_oAudioSettings.m_fMasterVolume);
  SetMusicVolume(m_oAudioSettings.m_fMusicVolume);
  SetSFXVolume(m_oAudioSettings.m_fSFXVolume);
}

void UAudioSettingsManager::ResetToDefaults(const FCustomAudioSettings& _rDefaultAudioSettings) {
  m_oAudioSettings = _rDefaultAudioSettings;

  ApplySettings();
}

void UAudioSettingsManager::SetMasterVolume(float _fNewVolume) {
  float fFinalValue = NormalizeVolumeInput(_fNewVolume);

  m_oAudioSettings.m_fMasterVolume = fFinalValue;

  if (m_pMasterBus) m_pMasterBus->setVolume(fFinalValue);
}

void UAudioSettingsManager::SetMusicVolume(float _fNewVolume) {
  float fFinalValue = NormalizeVolumeInput(_fNewVolume);

  m_oAudioSettings.m_fMusicVolume = fFinalValue;

  if (m_pMusicBus) m_pMusicBus->setVolume(fFinalValue);
}

void UAudioSettingsManager::SetSFXVolume(float _fNewVolume) {
  float fFinalValue = NormalizeVolumeInput(_fNewVolume);

  m_oAudioSettings.m_fSFXVolume = fFinalValue;

  if (m_pSFXBus) m_pSFXBus->setVolume(fFinalValue);
}

float UAudioSettingsManager::GetMaxVolume() const {
  return m_fMaxVolume;
}

const FCustomAudioSettings& UAudioSettingsManager::GetSettings() const {
  return m_oAudioSettings;
}

void UAudioSettingsManager::SetSettings(const FCustomAudioSettings& _rNewSettings) {
  m_oAudioSettings = _rNewSettings;

  ApplySettings();
}

float UAudioSettingsManager::NormalizeVolumeInput(float _fNewVolume) const {
  if (_fNewVolume <= 1.f) return FMath::Lerp(0.f, m_fMaxVolume, FMath::Clamp(_fNewVolume, 0.f, 1.f));

  return FMath::Clamp(_fNewVolume, 0.f, m_fMaxVolume);
}