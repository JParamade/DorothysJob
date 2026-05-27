/************************************************************************
 * @description: Represents the Visual Settings Manager class contained in the Settings Manager.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#include "VisualSettingsManager.h"

#include "GameFramework/GameUserSettings.h"

void UVisualSettingsManager::Initialize() {
  m_pGameUserSettings = UGameUserSettings::GetGameUserSettings();
}

void UVisualSettingsManager::ApplySettings() {
  SetGraphicsQuality(m_oVisualSettings.m_eGraphicsQuality);
  SetResolution(m_oVisualSettings.m_vResolution);
  SetVSync(m_oVisualSettings.m_bVSync);
  SetFPS(m_oVisualSettings.m_iFPS);
}

void UVisualSettingsManager::ResetToDefaults(const FCustomVisualSettings& _rDefaultGeneralSettings) {
  m_oVisualSettings = _rDefaultGeneralSettings;

  ApplySettings();
}

void UVisualSettingsManager::SetGraphicsQuality(EGraphicsQuality _eGraphicsQuality) {
  if (IsValid(m_pGameUserSettings)) {
    int32 iQuality = static_cast<int32>(_eGraphicsQuality);

    m_pGameUserSettings->SetAntiAliasingQuality(iQuality);
    m_pGameUserSettings->SetFoliageQuality(iQuality);
    m_pGameUserSettings->SetGlobalIlluminationQuality(iQuality);
    m_pGameUserSettings->SetPostProcessingQuality(iQuality);
    m_pGameUserSettings->SetReflectionQuality(iQuality);
    m_pGameUserSettings->SetShadingQuality(iQuality);
    m_pGameUserSettings->SetShadowQuality(iQuality);
    m_pGameUserSettings->SetTextureQuality(iQuality);
    m_pGameUserSettings->SetViewDistanceQuality(iQuality);
    m_pGameUserSettings->SetVisualEffectQuality(iQuality);

    m_pGameUserSettings->ApplySettings(false);
  }

  m_oVisualSettings.m_eGraphicsQuality = _eGraphicsQuality;
}

void UVisualSettingsManager::SetResolution(FIntPoint _vResolution) {
  if (IsValid(m_pGameUserSettings)) {
    m_pGameUserSettings->SetScreenResolution(_vResolution);
    m_pGameUserSettings->ApplySettings(false);
  }

  m_oVisualSettings.m_vResolution = _vResolution;
}

void UVisualSettingsManager::SetVSync(bool _bVSync) {
  if (IsValid(m_pGameUserSettings)) {
    m_pGameUserSettings->SetVSyncEnabled(_bVSync);
    m_pGameUserSettings->ApplySettings(false);
  }

  m_oVisualSettings.m_bVSync = _bVSync;
}

void UVisualSettingsManager::SetFPS(int32 _iFPS) {
  if (IsValid(m_pGameUserSettings)) {
    m_pGameUserSettings->SetFrameRateLimit(_iFPS);
    m_pGameUserSettings->ApplySettings(false);
  }

  m_oVisualSettings.m_iFPS = _iFPS;
}

const FCustomVisualSettings& UVisualSettingsManager::GetSettings() const {
  return m_oVisualSettings;
}

void UVisualSettingsManager::SetSettings(const FCustomVisualSettings& _pNewSettings) {
  m_oVisualSettings = _pNewSettings;
}