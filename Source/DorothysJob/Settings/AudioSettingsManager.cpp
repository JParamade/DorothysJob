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
  // Apply the current settings to the FMOD buses.
  SetMasterVolume(m_oAudioSettings.m_fMasterVolume);
  SetMusicVolume(m_oAudioSettings.m_fMusicVolume);
  SetSFXVolume(m_oAudioSettings.m_fSFXVolume);
}

void UAudioSettingsManager::ResetToDefaults(const FCustomAudioSettings& _rDefaultAudioSettings) {
  // Reset the audio settings to the provided default values.
  m_oAudioSettings = _rDefaultAudioSettings;

  // Apply the default settings.
  ApplySettings();
}

void UAudioSettingsManager::SetMasterVolume(float _fNewVolume) {
  // Normalize the input volume value to ensure it's within the acceptable range.
  float fFinalValue = NormalizeVolumeInput(_fNewVolume);

  // Update the master volume in the settings struct.
  m_oAudioSettings.m_fMasterVolume = fFinalValue;

  // Apply the new master volume to the FMOD Master Bus if it has been initialized.
  if (m_pMasterBus) m_pMasterBus->setVolume(fFinalValue);
}

void UAudioSettingsManager::SetMusicVolume(float _fNewVolume) {
  // Normalize the input volume value to ensure it's within the acceptable range.
  float fFinalValue = NormalizeVolumeInput(_fNewVolume);

  // Update the music volume in the settings struct.
  m_oAudioSettings.m_fMusicVolume = fFinalValue;

  // Apply the new music volume to the FMOD Music Bus if it has been initialized.
  if (m_pMusicBus) m_pMusicBus->setVolume(fFinalValue);
}

void UAudioSettingsManager::SetSFXVolume(float _fNewVolume) {
  // Normalize the input volume value to ensure it's within the acceptable range.
  float fFinalValue = NormalizeVolumeInput(_fNewVolume);

  // Update the SFX volume in the settings struct.
  m_oAudioSettings.m_fSFXVolume = fFinalValue;

  // Apply the new SFX volume to the FMOD SFX Bus if it has been initialized.
  if (m_pSFXBus) m_pSFXBus->setVolume(fFinalValue);
}

float UAudioSettingsManager::GetMaxVolume() const {
  // Return the maximum allowed volume level for the audio settings.
  return m_fMaxVolume;
}

const FCustomAudioSettings& UAudioSettingsManager::GetSettings() const {
  // Return the current audio settings struct, which contains the master, music, and SFX volume levels.
  return m_oAudioSettings;
}

void UAudioSettingsManager::SetSettings(const FCustomAudioSettings& _rNewSettings) {
  // Update the audio settings struct with the new settings provided as input.
  m_oAudioSettings = _rNewSettings;

  // Apply the new settings.
  ApplySettings();
}

float UAudioSettingsManager::NormalizeVolumeInput(float _fNewVolume) const {
  // Normalize the input volume value to ensure it's within the acceptable range.
  if (_fNewVolume <= 1.f) return FMath::Lerp(0.f, m_fMaxVolume, FMath::Clamp(_fNewVolume, 0.f, 1.f));

  // If the input volume is greater than 1, we assume it's already in the range of [0, m_fMaxVolume].
  return FMath::Clamp(_fNewVolume, 0.f, m_fMaxVolume);
}