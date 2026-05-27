/************************************************************************
 * @description: Represents the Settings UMG Menu class.
 * @author: Jaime Paramo
 * @date: 24/06/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "SettingsMenu.generated.h"

// Component
class UBaseButton;
class UBaseCheckBox;
class UImage;
class UBaseSlider;
class UTextBlock;
class USettingsManager;
class USelector;
class UInputIcon;

UCLASS()
class DOROTHYSJOB_API USettingsMenu : public UFocusableWidget
{
  GENERATED_BODY()

public:

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pContainerImage;

  /**
   * @brief -> Subtitle Text Block for the audio settings section. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pAudioSettingsText;

  /**
   * @brief -> Label for the Master Volume Slider. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pMasterVolumeText;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pMasterImage;
  /**
   * @brief -> Slider that controls the Master Volume level. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseSlider> m_pMasterSlider;

  /**
   * @brief -> Label for the Music Volume Slider. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pMusicVolumeText;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pMusicImage;
  /**
   * @brief -> Slider that controls the Music Volume level. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseSlider> m_pMusicSlider;

  /**
   * @brief -> Label for the SFX Volume Slider. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pSFXVolumeText;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSFXImage;
  /**
   * @brief -> Slider that controls the SFX Volume level. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseSlider> m_pSFXSlider;

  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pGraphicsSettingsText;

  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pQualityText;
  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<USelector> m_pQualitySelector;

  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pResolutionText;
  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<USelector> m_pResolutionSelector;

  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pVSyncText;
  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseCheckBox> m_pVSyncCheckbox;

  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pFPSText;
  /**
   * @brief -> @TOFILl. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<USelector> m_pFPSSelector;

  /**
   * Label text for language selector.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pLanguageText;

  /**
   * Selector to choose the current language for the game.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<USelector> m_pLanguageSelector;

  /**
   * @brief -> Button that closes the settings menu and returns to the previous screen. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pCloseButton;

  /**
   * The button icon to change the tab.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pChangeTabIconButton;

  /**
   * Label text for change tab icon.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pChangeTabText;

  /**
   * @brief -> Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;
  /**
   * @brief -> Hides the UMG elements of this Widget.
   */
  virtual void Hide() override;

  /**
   * @brief -> Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief -> Configures the focus system for the current menu when it's opened.
   */
  virtual void SetupFocus() override;

  void LoadSettings();
  void SaveSettings();

private:
  /**
   * @brief -> Called when the Master volume Slider value is changed.
   * @param _fValue -> New Slider value in the range [0.0, 1.0], representing the Master volume.
   */
  UFUNCTION()
  void OnMasterSliderValueChanged(float _fValue);
  /**
   * @brief -> Called when the Music volume Slider value is changed.
   * @param _fValue -> New Slider value in the range [0.0, 1.0], representing the Music volume.
   */
  UFUNCTION()
  void OnMusicSliderValueChanged(float _fValue);
  /**
   * @brief -> Called when the SFX volume Slider value is changed.
   * @param _fValue -> New Slider value in the range [0.0, 1.0], representing the SFX volume.
   */
  UFUNCTION()
  void OnSFXSliderValueChanged(float _fValue);

  /**
   * Graphic options text list.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Graphic Options", AllowPrivateAccess = "true"))
  TArray<FText> m_lGraphicsOptions;
  /**
   * @brief
   */
  UFUNCTION()
  void OnGraphicsQualityChanged(int32 _iNewIndex);

  /**
   * @brief 
   */
  void InitResolutions();
  /**
   * @brief
   */
  UFUNCTION()
  void OnResolutionChanged(int32 _iNewIndex);
  /**
   * @brief
   */
  UPROPERTY()
  TArray<FIntPoint> m_lResolutions;

  /**
   * @brief
   * @param _bValue
   */
  UFUNCTION()
  void OnVSyncCheckChanged(bool _bValue);

  /**
   * @brief 
   */
  TArray<FText> m_lFPSOptions = {
    FText::FromString(TEXT("30")),
    FText::FromString(TEXT("60")),
    FText::FromString(TEXT("90")),
    FText::FromString(TEXT("120"))
  };
  /**
   * @brief
   */
  void InitFPS();
  /**
   * @brief
   */
  UFUNCTION()
  void OnFPSChanged(int32 _iNewIndex);
  /**
   * @brief
   */
  UPROPERTY()
  TArray<int32> m_lFPS = { 30, 60, 90, 120 };

  /**
   * @brief -> Called when the player clicks the Close Button.
   */
  UFUNCTION()
  void OnCloseClicked();

  TObjectPtr<USettingsManager> m_pSettingsManager;

  bool m_bMenuShown = false;

private:

  /**
   * @brief It inits the language selector.
   */
  void InitLanguageSelector();

  /**
   * @brief Delegate to change the current language in the game.
   * @param _iIndex -> The selected language index.
   */
  UFUNCTION()
  void OnLanguageChanged(int32 _iIndex);
};