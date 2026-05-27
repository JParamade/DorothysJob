/************************************************************************
 * @description: Represents the Dorothy Info's HUD widget.
 * @author: Jaime Paramo
 * @date: 07/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDDorothy.h"

 // Component
#include "Components/Image.h"
#include "Components/ProgressBar.h"

// Data
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"

// Gameplay
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include <Kismet/GameplayStatics.h>

// Managers
#include "DorothysJob/Manager/BaseGameInstance.h"

void UHUDDorothy::Show() {
  Super::Show();

  m_bIsMenuActive = true;

  if (m_bSecondarySpecialGlowing) PlayGlowAnimation(m_pSecondarySpecialGlow);

  // Bind to the BasePlayer's combat events to react when the weapon information is updated.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    //m_oWeaponChangeHandle = pPlayerPawn->OnWeaponChange.AddUObject(this, &UHUDWeapon::UpdateWeaponIcons);
    pPlayerPawn->OnCharacterHealthChanged.BindUObject(this, &UHUDDorothy::UpdateHealthBar);
    m_oWeaponChangeHandle = pPlayerPawn->OnWeaponChange.AddUObject(this, &UHUDDorothy::UpdateSpecialInfo);
    UpdateHealthBar(pPlayerPawn->GetCurrentHealth(), pPlayerPawn->GetMaxHealth());
  }
}

void UHUDDorothy::ShowHealth() {
  if (IsValid(m_pHealthBackground)) m_pHealthBackground->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
  if (IsValid(m_pHealthFill)) m_pHealthFill->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
  if (IsValid(m_pDorothyImage)) m_pDorothyImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UHUDDorothy::ShowSpecial() {
  if (IsValid(m_pPrimarySpecialBar)) m_pPrimarySpecialBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (pGameInstance->m_lSelectedWeapons.Num() > 1) {
      if (IsValid(m_pSecondarySpecialBar)) m_pSecondarySpecialBar->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
  }
  if (IsValid(m_pSpecialFrame)) m_pSpecialFrame->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UHUDDorothy::Hide() {
  Super::Hide();

  m_bIsMenuActive = false;

  if (m_bSecondarySpecialGlowing) StopGlowAnimation(m_pSecondarySpecialGlow);

  // Hide Glow Animations
  if (m_bPrimarySpecialGlowing) {
    StopGlowAnimation(m_pPrimarySpecialGlow);
    m_bPrimarySpecialGlowing = false;
  }
  if (m_bHealthGlowing) {
    StopGlowAnimation(m_pHealthGlow);
    m_bHealthGlowing = false;
  }

  // Remove event bindings.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    pPlayerPawn->OnWeaponChange.Remove(m_oWeaponChangeHandle);
    pPlayerPawn->OnCharacterHealthChanged.Unbind();
  }
}

void UHUDDorothy::NativeConstruct() {
  Super::NativeConstruct();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (pGameInstance->m_oHUDData[EHUDSection::LIFE]) {
      // Health Bar
      if (IsValid(m_pHealthBackground)) m_lElements.Add(m_pHealthBackground);
      if (IsValid(m_pHealthFill)) m_lElements.Add(m_pHealthFill);
      if (IsValid(m_pDorothyImage)) m_lElements.Add(m_pDorothyImage);
    }
    if (pGameInstance->m_oHUDData[EHUDSection::SPECIAL]) {
      // Special Bar
      if (IsValid(m_pPrimarySpecialBar)) m_lElements.Add(m_pPrimarySpecialBar);
      if (pGameInstance->m_lSelectedWeapons.Num() > 1) {
        if (IsValid(m_pSecondarySpecialBar)) m_lElements.Add(m_pSecondarySpecialBar);
      }
      if (IsValid(m_pSpecialFrame)) m_lElements.Add(m_pSpecialFrame);
    }
  }
}

void UHUDDorothy::NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) {
  Super::NativeTick(_rMyGeometry, _fInDeltaTime);

  // SHEESH MAN, YOU CRIMINAL AF
  if (ABasePlayer* pPlayer = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      m_fPrimaryPercent = MapPercentToRange(.39f, .95f, pPlayer->GetCurrentWeaponSpecialCharge());

      if ((FMath::IsNearlyEqual(m_fPrimaryPercent, .95f)) && !m_bPrimarySpecialGlowing && m_bIsMenuActive) {
        if (pGameInstance->m_oHUDData[EHUDSection::SPECIAL]) PlayGlowAnimation(m_pPrimarySpecialGlow);
        m_bPrimarySpecialGlowing = true;
      }
      else if (!FMath::IsNearlyEqual(m_fPrimaryPercent, .95f)) {
        StopGlowAnimation(m_pPrimarySpecialGlow);
        m_bPrimarySpecialGlowing = false;
      }

      m_pPrimarySpecialBar->SetPercent(m_fPrimaryPercent);
    }
  }
}

void UHUDDorothy::UpdateHealthBar(int32 _iNewCurrentHealth, int32 _iMaxHealth) {
  // Calculate health percentage.
  float fNewPercentage = 1.f - (static_cast<float>(_iNewCurrentHealth) / static_cast<float>(_iMaxHealth));
  float fMappedPercentage = MapPercentToRange(.35f, 1.f, fNewPercentage);

  if ((FMath::IsNearlyEqual(fMappedPercentage, 1)) && !m_bHealthGlowing) {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (pGameInstance->m_oHUDData[EHUDSection::LIFE]) PlayGlowAnimation(m_pHealthGlow);
    }
    m_bHealthGlowing = true;
  }
  else {
    StopGlowAnimation(m_pHealthGlow);
    m_bHealthGlowing = false;
  }

  // @TOFILL
  UpdateDorothyFacials(fMappedPercentage);

  // Update the health bar fill percentage with the new value.
  m_pHealthFill->GetDynamicMaterial()->SetScalarParameterValue(TEXT("fPercent"), fMappedPercentage);
}

void UHUDDorothy::UpdateDorothyFacials(float _fNewPercentage) {
  float fPreviousPercentage;
  m_pHealthFill->GetDynamicMaterial()->GetScalarParameterValue(TEXT("fPercent"), fPreviousPercentage);

  if (IsValid(m_pDorothyImage)) {
    if (FMath::IsNearlyEqual(_fNewPercentage, 1)) m_pDorothyImage->SetBrushFromTexture(m_pDorothyDirtyTexture);
    else {
      if (_fNewPercentage - fPreviousPercentage > 0) {
        m_pDorothyImage->SetBrushFromTexture(m_pDorothyHitTexture);

        FTimerHandle oHandle;
        TWeakObjectPtr<UHUDDorothy> WeakThis(this);
        GetWorld()->GetTimerManager().SetTimer(
          oHandle,
          [WeakThis]() {
            if (WeakThis.IsValid() && IsValid(WeakThis->m_pDorothyImage) && IsValid(WeakThis->m_pDorothyNeutralTexture))
            {
              WeakThis->m_pDorothyImage->SetBrushFromTexture(WeakThis->m_pDorothyNeutralTexture);
            }
          },
          .3f,
          false
        );
      }
      else m_pDorothyImage->SetBrushFromTexture(m_pDorothyNeutralTexture);
    }
  }
}

void UHUDDorothy::UpdateSpecialInfo(EDirtType _eDirtType, EWeaponAttackType _eWeaponAttackType) {
  if (!IsValid(m_pPrimarySpecialBar) ||
    !IsValid(m_pSecondarySpecialBar) ||
    !IsValid(m_pPrimarySpecialGlow) ||
    !IsValid(m_pSecondarySpecialGlow)) return;

  // Swap Percent.
  m_pSecondarySpecialBar->SetPercent(m_fPrimaryPercent);

  // Swap Glow Visibility
  m_pSecondarySpecialGlow->SetVisibility(m_bPrimarySpecialGlowing ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

  // Swap Glow Image
  m_pPrimarySpecialGlow->SetBrushFromMaterial(_eDirtType == EDirtType::Liquid ? m_pLiquidGlow : m_pDustGlow);
  m_pSecondarySpecialGlow->SetBrushFromMaterial(_eDirtType == EDirtType::Liquid ? m_pDustGlow : m_pLiquidGlow);

  // Swap Fill Image
  FProgressBarStyle oPrimaryStyle = m_pPrimarySpecialBar->GetWidgetStyle();
  FProgressBarStyle oSecondaryStyle = m_pSecondarySpecialBar->GetWidgetStyle();

  oPrimaryStyle.FillImage.SetResourceObject(_eDirtType == EDirtType::Liquid ? m_pLiquidFill : m_pDustFill);
  oSecondaryStyle.FillImage.SetResourceObject(_eDirtType == EDirtType::Liquid ? m_pDustFill : m_pLiquidFill);

  m_pPrimarySpecialBar->SetWidgetStyle(oPrimaryStyle);
  m_pSecondarySpecialBar->SetWidgetStyle(oSecondaryStyle);
  
  m_bSecondarySpecialGlowing = m_bPrimarySpecialGlowing;
}

void UHUDDorothy::PlayGlowAnimation(UImage* m_pImage) {
  // Show the timer glow image, if valid.
  if (IsValid(m_pImage)) m_pImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UHUDDorothy::StopGlowAnimation(UImage* m_pImage) {
  // Hide the timer glow image, if valid.
  if (IsValid(m_pImage)) m_pImage->SetVisibility(ESlateVisibility::Hidden);
}

float UHUDDorothy::MapPercentToRange(float _fMin, float _fMax, float _fValue) {
  return FMath::Lerp(_fMin, _fMax, _fValue);
}