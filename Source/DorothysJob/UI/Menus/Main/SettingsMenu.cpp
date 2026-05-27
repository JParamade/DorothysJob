/************************************************************************
 * @description: Represents the Settings UMG Menu class.
 * @author: Jaime Paramo
 * @date: 24/06/2025
 * @edited_by:
 ************************************************************************/

#include "SettingsMenu.h"

 // Component
#include "Components/ComboBoxString.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "DorothysJob/UI/UMGPalette/CheckBox/BaseCheckBox.h"
#include "DorothysJob/UI/Utils/InputIcon.h"

// Gameplay
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SettingsManager.h"

// Settings
#include "DorothysJob/Settings/AccesibilitySettingsManager.h"
#include "DorothysJob/Settings/AudioSettingsManager.h"
#include "DorothysJob/Settings/InputSettingsManager.h"
#include "DorothysJob/Settings/GeneralSettingsManager.h"
#include "DorothysJob/Settings/SettingTypes.h"
#include "DorothysJob/Settings/VisualSettingsManager.h"

// Widget
#include "DorothysJob/UI/UMGPalette/Slider/BaseSlider.h"
#include "DorothysJob/UI/UMGPalette/Selector/Selector.h"
#include "DorothysJob/UI/Menus/Main/MainMenu.h"

#include "Internationalization/Culture.h"

void USettingsMenu::Show() {
  Super::Show();

  SetupFocus();
  LoadSettings();

  if (IsValid(m_pQualitySelector)) m_pQualitySelector->Show();
  if (IsValid(m_pResolutionSelector)) m_pResolutionSelector->Show();
  if (IsValid(m_pFPSSelector)) m_pFPSSelector->Show();
  if (IsValid(m_pLanguageSelector))
  {
    m_pLanguageSelector->Show();
    m_pLanguageSelector->OnSelectionChanged.AddUniqueDynamic(this, &USettingsMenu::OnLanguageChanged);
  }
  if (IsValid(m_pChangeTabIconButton))
  {
    m_pChangeTabIconButton->Show();
  }

  // Add Bindings
  if (IsValid(m_pMasterSlider)) {
    if (!m_pMasterSlider->OnValueChanged.IsAlreadyBound(this, &USettingsMenu::OnMasterSliderValueChanged)) {
      m_pMasterSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnMasterSliderValueChanged);
    }
  }
  if (IsValid(m_pMusicSlider)) {
    if (!m_pMusicSlider->OnValueChanged.IsAlreadyBound(this, &USettingsMenu::OnMusicSliderValueChanged)) {
      m_pMusicSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnMusicSliderValueChanged);
    }
  }
  if (IsValid(m_pSFXSlider)) {
    if (!m_pSFXSlider->OnValueChanged.IsAlreadyBound(this, &USettingsMenu::OnSFXSliderValueChanged)) {
      m_pSFXSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnSFXSliderValueChanged);
    }
  }
  if (IsValid(m_pVSyncCheckbox)) {
    if (!m_pVSyncCheckbox->OnCheckStateChanged.IsAlreadyBound(this, &USettingsMenu::OnVSyncCheckChanged)) {
      m_pVSyncCheckbox->OnCheckStateChanged.AddDynamic(this, &USettingsMenu::OnVSyncCheckChanged);
    }
  }
  if (IsValid(m_pCloseButton)) {
    if (!m_pCloseButton->OnClicked.IsAlreadyBound(this, &USettingsMenu::OnCloseClicked)) {
      m_pCloseButton->OnClicked.AddDynamic(this, &USettingsMenu::OnCloseClicked);
    }
  }
  if (IsValid(m_pResolutionSelector)) {
    if (!m_pResolutionSelector->OnSelectionChanged.IsAlreadyBound(this, &USettingsMenu::OnResolutionChanged)) {
      m_pResolutionSelector->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnResolutionChanged);
    }
  }
  if (IsValid(m_pQualitySelector)) {
    if (!m_pQualitySelector->OnSelectionChanged.IsAlreadyBound(this, &USettingsMenu::OnGraphicsQualityChanged)) {
      m_pQualitySelector->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnGraphicsQualityChanged);
    }
  }
  if (IsValid(m_pResolutionSelector)) {
    if (!m_pResolutionSelector->OnSelectionChanged.IsAlreadyBound(this, &USettingsMenu::OnResolutionChanged)) {
      m_pResolutionSelector->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnResolutionChanged);
    }
  }
}

void USettingsMenu::Hide() {
  Super::Hide();

  if (IsValid(m_pQualitySelector)) m_pQualitySelector->Hide();
  if (IsValid(m_pResolutionSelector)) m_pResolutionSelector->Hide();
  if (IsValid(m_pFPSSelector)) m_pFPSSelector->Hide();
  if (IsValid(m_pLanguageSelector))
  {
    m_pLanguageSelector->Hide();
    m_pLanguageSelector->OnSelectionChanged.RemoveAll(this);
  }
  if (IsValid(m_pChangeTabIconButton))
  {
    m_pChangeTabIconButton->Hide();
  }

  // Remove Bindings
  if (IsValid(m_pQualitySelector)) m_pQualitySelector->OnSelectionChanged.RemoveAll(this);
  if (IsValid(m_pResolutionSelector)) m_pResolutionSelector->OnSelectionChanged.RemoveAll(this);
  if (IsValid(m_pFPSSelector)) m_pFPSSelector->OnSelectionChanged.RemoveAll(this);
  if (IsValid(m_pMasterSlider)) m_pMasterSlider->OnValueChanged.RemoveAll(this);
  if (IsValid(m_pMusicSlider)) m_pMusicSlider->OnValueChanged.RemoveAll(this);
  if (IsValid(m_pSFXSlider)) m_pSFXSlider->OnValueChanged.RemoveAll(this);
  if (IsValid(m_pVSyncCheckbox)) m_pVSyncCheckbox->OnCheckStateChanged.RemoveAll(this);
  if (IsValid(m_pCloseButton)) m_pCloseButton->OnClicked.RemoveAll(this);
}

void USettingsMenu::NativeConstruct() {
  Super::NativeConstruct();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    // Access the SettingsManager subsystem from the GameInstance.
    m_pSettingsManager = pGameInstance->GetSubsystem<USettingsManager>();
  }

  // Initialize Selectors
  if (IsValid(m_pSettingsManager) && IsValid(m_pQualitySelector)) {
    m_pQualitySelector->InitializeOptions(
      m_lGraphicsOptions,
      static_cast<int32>(m_pSettingsManager->GetVisualSettingsManager()->GetSettings().m_eGraphicsQuality)
    );

    if (!m_pQualitySelector->OnSelectionChanged.IsAlreadyBound(this, &USettingsMenu::OnGraphicsQualityChanged)) {
      m_pQualitySelector->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnGraphicsQualityChanged);
    }
  }
  InitResolutions();
  InitFPS();
  InitLanguageSelector();

  LoadSettings();

  // Non Focusable Elements
  if (IsValid(m_pContainerImage)) m_lElements.Add(m_pContainerImage);
  if (IsValid(m_pAudioSettingsText)) m_lElements.Add(m_pAudioSettingsText);
  if (IsValid(m_pMasterVolumeText)) m_lElements.Add(m_pMasterVolumeText);
  if (IsValid(m_pMasterImage)) m_lElements.Add(m_pMasterImage);
  if (IsValid(m_pMusicVolumeText)) m_lElements.Add(m_pMusicVolumeText);
  if (IsValid(m_pMusicImage)) m_lElements.Add(m_pMusicImage);
  if (IsValid(m_pSFXVolumeText)) m_lElements.Add(m_pSFXVolumeText);
  if (IsValid(m_pSFXImage)) m_lElements.Add(m_pSFXImage);
  if (IsValid(m_pGraphicsSettingsText)) m_lElements.Add(m_pGraphicsSettingsText);
  if (IsValid(m_pQualityText)) m_lElements.Add(m_pQualityText);
  if (IsValid(m_pResolutionText)) m_lElements.Add(m_pResolutionText);
  if (IsValid(m_pVSyncText)) m_lElements.Add(m_pVSyncText);
  if (IsValid(m_pFPSText)) m_lElements.Add(m_pFPSText);
  if (IsValid(m_pLanguageText))
  {
    m_lElements.Add(m_pLanguageText);
  }
  if (IsValid(m_pChangeTabText))
  {
    m_lElements.Add(m_pChangeTabText);
  }

  // Focusable Elements
  if (IsValid(m_pMasterSlider)) {
    if (!m_pMasterSlider->OnValueChanged.IsAlreadyBound(this, &USettingsMenu::OnMasterSliderValueChanged)) {
      m_pMasterSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnMasterSliderValueChanged);
    }
    m_lFocusableElements.Add(m_pMasterSlider);
  }
  if (IsValid(m_pMusicSlider)) {
    if (!m_pMusicSlider->OnValueChanged.IsAlreadyBound(this, &USettingsMenu::OnMusicSliderValueChanged)) {
      m_pMusicSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnMusicSliderValueChanged);
    }
    m_lFocusableElements.Add(m_pMusicSlider);
  }
  if (IsValid(m_pSFXSlider)) {
    if (!m_pSFXSlider->OnValueChanged.IsAlreadyBound(this, &USettingsMenu::OnSFXSliderValueChanged)) {
      m_pSFXSlider->OnValueChanged.AddDynamic(this, &USettingsMenu::OnSFXSliderValueChanged);
    }
    m_lFocusableElements.Add(m_pSFXSlider);
  }
  if (IsValid(m_pQualitySelector)) m_lFocusableElements.Add(m_pQualitySelector);
  if (IsValid(m_pResolutionSelector)) m_lFocusableElements.Add(m_pResolutionSelector);
  if (IsValid(m_pVSyncCheckbox)) {
    if (!m_pVSyncCheckbox->OnCheckStateChanged.IsAlreadyBound(this, &USettingsMenu::OnVSyncCheckChanged)) {
      m_pVSyncCheckbox->OnCheckStateChanged.AddDynamic(this, &USettingsMenu::OnVSyncCheckChanged);
    }
    m_lFocusableElements.Add(m_pVSyncCheckbox);
  }
  if (IsValid(m_pFPSSelector)) m_lFocusableElements.Add(m_pFPSSelector);
  if (IsValid(m_pLanguageSelector))
  {
    m_lFocusableElements.Add(m_pLanguageSelector);
  }
  if (IsValid(m_pCloseButton)) {
    if (!m_pCloseButton->OnClicked.IsAlreadyBound(this, &USettingsMenu::OnCloseClicked)) {
      m_pCloseButton->OnClicked.AddDynamic(this, &USettingsMenu::OnCloseClicked);
    }
    m_lFocusableElements.Add(m_pCloseButton);
  }
}

void USettingsMenu::SetupFocus() {
  Super::SetupFocus();

  // Setup SpecificFocusManager
  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  m_pSpecificFocusManager[0]->Initialize(
    m_lFocusableElements,
    { 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    EFocusNavigationType::VERTICAL,
    true,
    false
  );

  // Set this menu as the currently active focus manager.
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    if (UGeneralFocusManager* pGeneralFocusManager = pPlayerController->GetGeneralFocusManager()) {
      pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
    }
  }
}

void USettingsMenu::LoadSettings() {
  if (IsValid(m_pSettingsManager)) {
    if (IsValid(m_pMasterSlider)) {
      m_pMasterSlider->SetValue(
        m_pSettingsManager->GetAudioSettingsManager()->GetSettings().m_fMasterVolume / m_pSettingsManager->GetAudioSettingsManager()->GetMaxVolume()
      );
    }
    if (IsValid(m_pMusicSlider)) {
      m_pMusicSlider->SetValue(
        m_pSettingsManager->GetAudioSettingsManager()->GetSettings().m_fMusicVolume / m_pSettingsManager->GetAudioSettingsManager()->GetMaxVolume()
      );
    }
    if (IsValid(m_pSFXSlider)) {
      m_pSFXSlider->SetValue(
        m_pSettingsManager->GetAudioSettingsManager()->GetSettings().m_fSFXVolume / m_pSettingsManager->GetAudioSettingsManager()->GetMaxVolume()
      );
    }

    if (IsValid(m_pVSyncCheckbox)) m_pVSyncCheckbox->SetCheckedState(
      m_pSettingsManager->GetVisualSettingsManager()->GetSettings().m_bVSync ? ECheckBoxState::Checked : ECheckBoxState::Unchecked
    );
  }
}

void USettingsMenu::SaveSettings()
{
  if (IsValid(m_pSettingsManager))
  {
    m_pSettingsManager->SaveSettings();
  }
}

void USettingsMenu::OnMasterSliderValueChanged(float _fValue) {
  if (IsValid(m_pSettingsManager)) m_pSettingsManager->GetAudioSettingsManager()->SetMasterVolume(_fValue);
}

void USettingsMenu::OnMusicSliderValueChanged(float _fValue) {
  if (IsValid(m_pSettingsManager)) m_pSettingsManager->GetAudioSettingsManager()->SetMusicVolume(_fValue);
}

void USettingsMenu::OnSFXSliderValueChanged(float _fValue) {
  if (IsValid(m_pSettingsManager)) m_pSettingsManager->GetAudioSettingsManager()->SetSFXVolume(_fValue);
}

void USettingsMenu::OnGraphicsQualityChanged(int32 _iNewIndex) {
  if (IsValid(m_pSettingsManager)) m_pSettingsManager->GetVisualSettingsManager()->SetGraphicsQuality(static_cast<EGraphicsQuality>(_iNewIndex));
}

void USettingsMenu::InitResolutions() {
  UKismetSystemLibrary::GetSupportedFullscreenResolutions(m_lResolutions);

  m_pResolutionSelector->InitializeDynamicOptions<FIntPoint>(
    m_lResolutions,
    [](const FIntPoint& Res) {
      return FText::FromString(FString::Printf(TEXT("%dx%d"), Res.X, Res.Y));
    },
    m_lResolutions.IndexOfByPredicate(
      [&](const FIntPoint& Res)
      {
        return Res == m_pSettingsManager->GetVisualSettingsManager()->GetSettings().m_vResolution;
      }
    ));

  if (!m_pResolutionSelector->OnSelectionChanged.IsAlreadyBound(this, &USettingsMenu::OnResolutionChanged)) {
    m_pResolutionSelector->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnResolutionChanged);
  }
}

void USettingsMenu::OnResolutionChanged(int32 _iNewIndex) {
  if (IsValid(m_pSettingsManager)) m_pSettingsManager->GetVisualSettingsManager()->SetResolution(m_lResolutions[_iNewIndex]);
}

void USettingsMenu::OnVSyncCheckChanged(bool _bValue) {
  if (IsValid(m_pSettingsManager)) m_pSettingsManager->GetVisualSettingsManager()->SetVSync(_bValue);
  GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("%d"), _bValue));
}

void USettingsMenu::InitFPS() {
  if (!IsValid(m_pFPSSelector) || !IsValid(m_pSettingsManager)) return;

  const int32 iCurrentFPS = m_pSettingsManager->GetVisualSettingsManager()->GetSettings().m_iFPS;

  int32 iCurrentIndex = m_lFPS.IndexOfByKey(iCurrentFPS);
  if (iCurrentIndex == INDEX_NONE) iCurrentIndex = 0;

  m_pFPSSelector->InitializeOptions(m_lFPSOptions, iCurrentIndex);
  if (!m_pFPSSelector->OnSelectionChanged.IsAlreadyBound(this, &USettingsMenu::OnFPSChanged)) {
    m_pFPSSelector->OnSelectionChanged.AddDynamic(this, &USettingsMenu::OnFPSChanged);
  }
}

void USettingsMenu::OnFPSChanged(int32 _iNewIndex) {
  if (IsValid(m_pSettingsManager)) m_pSettingsManager->GetVisualSettingsManager()->SetFPS(m_lFPS[_iNewIndex]);
}

void USettingsMenu::OnCloseClicked()
{
  OnMenuClosed.Broadcast();
}

void USettingsMenu::InitLanguageSelector()
{
  if (!IsValid(m_pSettingsManager) || !IsValid(m_pLanguageSelector))
  {
    return;
  }

  const TArray<FString> lLanguages = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
  TArray<FCultureRef> lCulturesRef = FInternationalization::Get().GetAvailableCultures(lLanguages, false);
  TArray<FText> lLanguageTexts;
  for (const FCultureRef& tCulture : lCulturesRef)
  {
    FString sLanguage = tCulture->GetNativeLanguage();
    if (sLanguage.Len() > 0)
    {
      sLanguage[0] = FChar::ToUpper(sLanguage[0]);
    }
    lLanguageTexts.Add(FText::FromString(sLanguage));
  }

  FString sCurrentLanguage = FInternationalization::Get().GetCurrentCulture()->GetName();
  int32 SelectedIndex = lLanguages.IndexOfByKey(sCurrentLanguage);
  if (SelectedIndex == INDEX_NONE)
  {
    SelectedIndex = 0;
  }

  m_pLanguageSelector->InitializeOptions(lLanguageTexts, SelectedIndex);
}

void USettingsMenu::OnLanguageChanged(int32 _iIndex)
{
  const TArray<FString> lLanguages = FTextLocalizationManager::Get().GetLocalizedCultureNames(ELocalizationLoadFlags::Game);
  if ((_iIndex < 0) || (_iIndex >= lLanguages.Num()))
  {
    return;
  }

  FString sSelectedLanguage = lLanguages[_iIndex];
  FInternationalization::Get().SetCurrentCulture(sSelectedLanguage);
}
