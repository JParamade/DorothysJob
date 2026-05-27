/************************************************************************
 * @description: Represents the General Settings Manager class contained in the Settings Manager.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#include "GeneralSettingsManager.h"

void UGeneralSettingsManager::Initialize() {

}

void UGeneralSettingsManager::ApplySettings() {
  SetShowFPS(m_oGeneralSettings.m_bShowFPS);
}

void UGeneralSettingsManager::ResetToDefaults(const FCustomGeneralSettings& _rDefaultGeneralSettings) {
  m_oGeneralSettings = _rDefaultGeneralSettings;

  ApplySettings();
}

void UGeneralSettingsManager::SetShowFPS(bool _bShowFPS) {
  m_oGeneralSettings.m_bShowFPS = _bShowFPS;
}

const FCustomGeneralSettings& UGeneralSettingsManager::GetSettings() const {
  return m_oGeneralSettings;
}

void UGeneralSettingsManager::SetSettings(const FCustomGeneralSettings& _rNewSettings) {
  m_oGeneralSettings = _rNewSettings;

  ApplySettings();
}