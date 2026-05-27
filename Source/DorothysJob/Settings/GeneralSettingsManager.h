/************************************************************************
 * @description: Represents the General Settings Manager class contained in the Settings Manager.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

// Settings
#include "DorothysJob/Settings/SettingTypes.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GeneralSettingsManager.generated.h"

enum class EDifficultyType : uint8;

UCLASS()
class DOROTHYSJOB_API UGeneralSettingsManager : public UObject
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
  void ResetToDefaults(const FCustomGeneralSettings& _rDefaultGeneralSettings);

  /**
   * @brief 
   * @param _bShowFPS 
   */
  void SetShowFPS(bool _bShowFPS);

  /**
   * @brief
   * @return
   */
  const FCustomGeneralSettings& GetSettings() const;
  /**
   * @brief
   * @param _pNewSettings
   */
  void SetSettings(const FCustomGeneralSettings& _pNewSettings);

private:
  /**
   * @brief
   */
  UPROPERTY(SaveGame)
  FCustomGeneralSettings m_oGeneralSettings;
};