/************************************************************************
 * @description: Represents the Audio Settings Manager class contained in the Settings Manager.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

// Settings
#include "DorothysJob/Settings/SettingTypes.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AudioSettingsManager.generated.h"

// Audio
namespace FMOD {
  namespace Studio {
    class Bus;
  }
}

UCLASS()
class DOROTHYSJOB_API UAudioSettingsManager : public UObject {
	GENERATED_BODY()
	
public:
  /**
   * @brief Initializes the audio settings manager.
   */
  void Initialize();
  /**
   * @brief Applies the current audio settings to the FMOD buses.
   */
  void ApplySettings();
  /**
   * @brief Resets the audio settings to the provided default values and applies them.
   * @param _rDefaultAudioSettings Struct containing the default audio settings to reset to.
   */
  void ResetToDefaults(const FCustomAudioSettings& _rDefaultAudioSettings);
  
  /**
   * @brief Sets the master volume level for the game.
   * @param _fNewVolume New volume value in the range [0.0, 1.0].
   */
  void SetMasterVolume(float _fNewVolume);
  /**
   * @brief Sets the music volume level.
   * @param _fNewVolume New volume value in the range [0.0, 1.0].
   */
  void SetMusicVolume(float _fNewVolume);
  /**
   * @brief Sets the sound effects volume level.
   * @param _fNewVolume New volume value in the range [0.0, 1.0].
   */
  void SetSFXVolume(float _fNewVolume);

  /**
   * @brief Gets the maximum allowed volume level.
   * @return The maximum volume level.
   */
  float GetMaxVolume() const;

  /**
   * @brief Retrieves the current audio settings.
   * @return A const reference to the current audio settings struct.
   */
  const FCustomAudioSettings& GetSettings() const;
  /**
   * @brief Updates the current audio settings with new values and applies them.
   * @param _pNewSettings Struct containing the new audio settings to apply.
   */
  void SetSettings(const FCustomAudioSettings& _rNewSettings);

private:
  /**
   * @brief Normalizes the input volume value to ensure it's within the acceptable range [0.0, m_fMaxVolume].
   */
  float NormalizeVolumeInput(float _fNewVolume) const;

  /**
   * @brief Struct containing the current audio settings.
   */
  UPROPERTY(SaveGame)
  FCustomAudioSettings m_oAudioSettings;

  /**
   * @brief Maximum allowed volume level for the audio settings.
   */
  float m_fMaxVolume = 2.f;

  /**
   * @brief Pointer to the FMOD Master Bus, used to control the master volume of the game.
   */
  FMOD::Studio::Bus* m_pMasterBus;
  /**
   * @brief Pointer to the FMOD Music Bus, used to control the music volume of the game.
   */
  FMOD::Studio::Bus* m_pMusicBus;
  /**
   * @brief Pointer to the FMOD SFX Bus, used to control the sound effects volume of the game.
   */
  FMOD::Studio::Bus* m_pSFXBus;
};