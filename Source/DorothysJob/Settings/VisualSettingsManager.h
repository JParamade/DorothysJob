/************************************************************************
 * @description: Represents the Visual Settings Manager class contained in the Settings Manager.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

// Settings
#include "DorothysJob/Settings/SettingTypes.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VisualSettingsManager.generated.h"

UCLASS()
class DOROTHYSJOB_API UVisualSettingsManager : public UObject
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
  void ResetToDefaults(const FCustomVisualSettings& _rDefaultGeneralSettings);

  /**
   * @brief 
   * @param _eGraphicsQuality 
   */
  void SetGraphicsQuality(EGraphicsQuality _eGraphicsQuality);

  /**
   * @brief 
   * @param _vResolution 
   */
  void SetResolution(FIntPoint _vResolution);

  /**
   * @brief 
   * @param _bVSync 
   */
  void SetVSync(bool _bVSync);

  /**
   * @brief 
   * @param _iFPS 
   */
  void SetFPS(int32 _iFPS);

  /**
   * @brief
   * @return
   */
  const FCustomVisualSettings& GetSettings() const;
  /**
   * @brief
   * @param _pNewSettings
   */
  void SetSettings(const FCustomVisualSettings& _pNewSettings);

private:
  /**
   * @brief
   */
  UPROPERTY(SaveGame)
  FCustomVisualSettings m_oVisualSettings;

  /**
   * @brief 
   */
  TObjectPtr<UGameUserSettings> m_pGameUserSettings;
};