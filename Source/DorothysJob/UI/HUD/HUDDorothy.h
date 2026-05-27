/************************************************************************
 * @description: Represents the Dorothy Info's HUD widget.
 * @author: Jaime Paramo
 * @date: 07/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDDorothy.generated.h"

// Component
class UImage;
class UProgressBar;

// Enum
enum class EDirtType : uint8;
enum class EWeaponAttackType : uint8;

UCLASS()
class DOROTHYSJOB_API UHUDDorothy : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pHealthGlow;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pHealthBackground;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pHealthFill;
  /**
 * @brief -> @TOFILL. Must match this name in UMG to compile.
 */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pDorothyImage;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimarySpecialGlow;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UProgressBar> m_pPrimarySpecialBar;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondarySpecialGlow;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UProgressBar> m_pSecondarySpecialBar;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSpecialFrame;

  /**
   * @brief -> Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;

  /**
   * @brief 
   */
  void ShowHealth();
  /**
   * @brief 
   */
  void ShowSpecial();

  /**
   * @brief -> Hides the UMG elements of this Widget.
   */
  virtual void Hide() override;

  /**
   * @brief -> Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief -> @TOFILL. THIS IS TEMPORAL!
   */
  virtual void NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) override;

private:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Dorothy B13", AllowPrivateAccess = true))
  TObjectPtr<UTexture2D> m_pDorothyB13Texture;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Dorothy Dirty", AllowPrivateAccess = true))
  TObjectPtr<UTexture2D> m_pDorothyDirtyTexture;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Dorothy Hit", AllowPrivateAccess = true))
  TObjectPtr<UTexture2D> m_pDorothyHitTexture;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Dorothy Neutral", AllowPrivateAccess = true))
  TObjectPtr<UTexture2D> m_pDorothyNeutralTexture;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Liquid Fill", AllowPrivateAccess = true))
  TObjectPtr<UTexture2D> m_pLiquidFill;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Dust Fill", AllowPrivateAccess = true))
  TObjectPtr<UTexture2D> m_pDustFill;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Liquid Glow", AllowPrivateAccess = true))
  TObjectPtr<UMaterial> m_pLiquidGlow;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Dust Glow", AllowPrivateAccess = true))
  TObjectPtr<UMaterial> m_pDustGlow;

  // @SUPER TEMPORAL
  float m_fPrimaryPercent;
  bool m_bSecondaryWeapon = false;

  /**
   * @brief 
   * @param _iNewCurrentHealth 
   * @param _iMaxHealth 
   */
  void UpdateHealthBar(int32 _iNewCurrentHealth, int32 _iMaxHealth);

  /**
   * @brief 
   * @param _fNewPercentage 
   */
  void UpdateDorothyFacials(float _fNewPercentage);

  /**
   * @brief 
   * @param _eDirtType 
   * @param _eWeaponAttackType 
   */
  void UpdateSpecialInfo(EDirtType _eDirtType, EWeaponAttackType _eWeaponAttackType);

  /**
   * @brief 
   * @param _pWidgetAnimation 
   */
  void PlayGlowAnimation(UImage* m_pImage);

  /**
   * @brief 
   */
  bool m_bPrimarySpecialGlowing = false;
  /**
   * @brief
   */
  bool m_bSecondarySpecialGlowing = false;
  /**
   * @brief 
   */
  bool m_bHealthGlowing = false;

  /**
   * @brief 
   * @param _pWidgetAnimation 
   */
  void StopGlowAnimation(UImage* m_pImage);

  /**
   * @brief
   * @param _fValue
   * @param _fMin
   * @param _fMax
   * @return
   */
  float MapPercentToRange(float _fMin, float _fMax, float _fValue);

  /**
   * @brief -> Delegate handle used to manage weapon change event bindings.
   */
  FDelegateHandle m_oWeaponChangeHandle;

  bool m_bIsMenuActive = false;
};
