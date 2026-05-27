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
class DOROTHYSJOB_API UAudioSettingsManager : public UObject
{
	GENERATED_BODY()
	
public:
  /**
   * @brief 
   */
  void Initialize();
  /**
   * @brief
   */
  void ApplySettings();
  /**
   * @brief 
   * @param Defaults 
   */
  void ResetToDefaults(const FCustomAudioSettings& _rDefaultAudioSettings);
  
  /**
   * @brief -> Sets the master volume level for the game.
   * @param _fNewVolume -> New volume value in the range [0.0, 1.0].
   */
  void SetMasterVolume(float _fNewVolume);
  /**
   * @brief -> Sets the music volume level.
   * @param _fNewVolume -> New volume value in the range [0.0, 1.0].
   */
  void SetMusicVolume(float _fNewVolume);
  /**
   * @brief -> Sets the sound effects volume level.
   * @param _fNewVolume -> New volume value in the range [0.0, 1.0].
   */
  void SetSFXVolume(float _fNewVolume);

  /**
   * @brief 
   * @return 
   */
  float GetMaxVolume() const;

  /**
   * @brief 
   * @return 
   */
  const FCustomAudioSettings& GetSettings() const;
  /**
   * @brief 
   * @param _pNewSettings 
   */
  void SetSettings(const FCustomAudioSettings& _rNewSettings);

private:
  /**
   * @brief 
   */
  float NormalizeVolumeInput(float _fNewVolume) const;

  /**
   * @brief
   */
  UPROPERTY(SaveGame)
  FCustomAudioSettings m_oAudioSettings;

  /**
   * @brief 
   */
  float m_fMaxVolume = 2.f;

  /**
   * @brief
   */
  FMOD::Studio::Bus* m_pMasterBus;
  /**
   * @brief
   */
  FMOD::Studio::Bus* m_pMusicBus;
  /**
   * @brief
   */
  FMOD::Studio::Bus* m_pSFXBus;
};