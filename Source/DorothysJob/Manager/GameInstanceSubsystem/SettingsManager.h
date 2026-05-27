/************************************************************************
 * @description: Represents the Settings Manager class.
 * @author: Jaime Paramo
 * @date: 24/06/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SettingsManager.generated.h"

class USaveGameManager;

// Audio
class UFMODBus;

// Data
class UDefaultSettingsDataAsset;

// Settings
class UAccesibilitySettingsManager;
class UAudioSettingsManager;
class UInputSettingsManager;
class UGeneralSettingsManager;
class UVisualSettingsManager;

enum class EDifficultyType : uint8;

UCLASS()
class DOROTHYSJOB_API USettingsManager : public UGameInstanceSubsystem
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Initializes the settings manager subsystem.
   * @param _rCollection -> Reference to the subsystem collection used during initialization.
   */
  void Initialize(FSubsystemCollectionBase& _rCollection) override;

  /**
   * @brief 
   */
  void SaveSettings();
  /**
   * @brief 
   */
  void LoadSettings();

  /**
   * @brief ->
   */
  void ResetToDefaults();

  /**
   * @brief
   * @return
   */
  TObjectPtr<UAccesibilitySettingsManager> GetAccesibilitySettingsManager() const;
  /**
   * @brief
   * @return
   */
  TObjectPtr<UAudioSettingsManager> GetAudioSettingsManager() const;
  /**
   * @brief
   * @return
   */
  TObjectPtr<UInputSettingsManager> GetInputSettingsManager() const;
  /**
   * @brief 
   * @return 
   */
  TObjectPtr<UGeneralSettingsManager> GetGeneralSettingsManager() const;
  /**
   * @brief 
   * @return 
   */
  TObjectPtr<UVisualSettingsManager> GetVisualSettingsManager() const;

private:
  /**
   * @brief
   */
  UPROPERTY()
  TObjectPtr<UAccesibilitySettingsManager> m_pAccesibilitySettingsManager;
  /**
   * @brief
   */
  UPROPERTY()
  TObjectPtr<UAudioSettingsManager> m_pAudioSettingsManager;
  /**
   * @brief
   */
  UPROPERTY()
  TObjectPtr<UInputSettingsManager> m_pInputSettingsManager;
  /**
   * @brief
   */
  UPROPERTY()
  TObjectPtr<UGeneralSettingsManager> m_pGeneralSettingsManager;
  /**
   * @brief
   */
  UPROPERTY()
  TObjectPtr<UVisualSettingsManager> m_pVisualSettingsManager;

  /**
   * @brief 
   */
  UPROPERTY()
  TObjectPtr<const UDefaultSettingsDataAsset> m_pDefaultSettingsData;

  UPROPERTY()
  TObjectPtr<USaveGameManager> m_pSaveGameManager;

  UPROPERTY()
  TArray<UObject*> m_lSettingsManagers;
};
