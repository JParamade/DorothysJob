/************************************************************************
 * @description: Represents the Settings Manager class.
 * @author: Jaime Paramo
 * @date: 24/06/2025
 * @edited_by:
 ************************************************************************/

#include "SettingsManager.h"

// Data
#include "DorothysJob/Data/DataAssets/Settings/DefaultSettingsDataAsset.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"

// Settings
#include "DorothysJob/Settings/AccesibilitySettingsManager.h"
#include "DorothysJob/Settings/AudioSettingsManager.h"
#include "DorothysJob/Settings/InputSettingsManager.h"
#include "DorothysJob/Settings/GeneralSettingsManager.h"
#include "DorothysJob/Settings/VisualSettingsManager.h"
#include "DorothysJob/Settings/SettingTypes.h"

void USettingsManager::Initialize(FSubsystemCollectionBase& _rCollection) {
  Super::Initialize(_rCollection);

  _rCollection.InitializeDependency<USaveGameManager>();
  
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    m_pDefaultSettingsData = pGameInstance->m_pDefaultSettingsData;
    m_pSaveGameManager = pGameInstance->GetSubsystem<USaveGameManager>();
  }
  m_lSettingsManagers.Empty();

  // Create managers.
  m_pGeneralSettingsManager = NewObject<UGeneralSettingsManager>(this);
  
  m_pAudioSettingsManager = NewObject<UAudioSettingsManager>(this);
  m_pAudioSettingsManager->Initialize();
  m_lSettingsManagers.Add(m_pAudioSettingsManager);

  m_pVisualSettingsManager = NewObject<UVisualSettingsManager>(this);
  m_pVisualSettingsManager->Initialize();
  m_lSettingsManagers.Add(m_pVisualSettingsManager);

  m_pInputSettingsManager = NewObject<UInputSettingsManager>(this);
  m_pAccesibilitySettingsManager = NewObject<UAccesibilitySettingsManager>(this);

  // Load settings on startup.
  LoadSettings();
}

void USettingsManager::SaveSettings() {
  if (m_pSaveGameManager)
  {
    m_pSaveGameManager->SaveSettings(m_lSettingsManagers);
  }
}

void USettingsManager::LoadSettings() {
  // It sets the game default language to the system configured language
  FString sSystemLanguage = FPlatformMisc::GetDefaultLanguage();
  FInternationalization::Get().SetCurrentLanguageAndLocale(sSystemLanguage);
  if (m_pSaveGameManager && m_pSaveGameManager->LoadSettings(m_lSettingsManagers))
  {
    if (IsValid(m_pAudioSettingsManager)) m_pAudioSettingsManager->ApplySettings();
    if (IsValid(m_pVisualSettingsManager)) m_pVisualSettingsManager->ApplySettings();
  }
  else
  {
    ResetToDefaults();
  }
}

void USettingsManager::ResetToDefaults() {
  if (m_pDefaultSettingsData) {
    if (IsValid(m_pAudioSettingsManager)) m_pAudioSettingsManager->ResetToDefaults(m_pDefaultSettingsData->m_oDefaultAudio);
    if (IsValid(m_pVisualSettingsManager)) m_pVisualSettingsManager->ResetToDefaults(m_pDefaultSettingsData->m_oDefaultVisual);
  }
}

TObjectPtr<UGeneralSettingsManager> USettingsManager::GetGeneralSettingsManager() const {
  return m_pGeneralSettingsManager;
}

TObjectPtr<UAudioSettingsManager> USettingsManager::GetAudioSettingsManager() const {
  return m_pAudioSettingsManager;
}

TObjectPtr<UVisualSettingsManager> USettingsManager::GetVisualSettingsManager() const {
  return m_pVisualSettingsManager;
}

TObjectPtr<UInputSettingsManager> USettingsManager::GetInputSettingsManager() const {
  return m_pInputSettingsManager;
}

TObjectPtr<UAccesibilitySettingsManager> USettingsManager::GetAccesibilitySettingsManager() const {
  return m_pAccesibilitySettingsManager;
}