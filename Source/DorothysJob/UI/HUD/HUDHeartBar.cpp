/************************************************************************
 * @description: Represents the HUD's Heart Bar Widget containing unitary hearts.
 * @author: Jaime Paramo
 * @date: 24/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDHeartBar.h"

// Component
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

// Gameplay
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include <Kismet/GameplayStatics.h>

// Widget
#include "Blueprint/WidgetTree.h"
#include "DorothysJob/UI/HUD/HUDHeart.h"

void UHUDHeartBar::Show() {
  Super::Show();

  // Bind to the BasePlayer's OnCharacterHealthChanged event to react when the current health is updated.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    pPlayerPawn->OnCharacterHealthChanged.BindUObject(this, &UHUDHeartBar::UpdateHearts);
  }

  // Initalize heart container with maximum player's health.
  if (!m_bIsInitialized) {
    InitializeHearts(5 /* TEMPORAL PARAMETER */);
    UpdateHearts(100, 0); // TEMPORAL FUNCTION
  }
  else {
    for (TObjectPtr<UHUDHeart> pHeart : m_lHearts) pHeart->Show();
  }
}

void UHUDHeartBar::Hide() {
  Super::Hide();

  // Remove event bindings.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    pPlayerPawn->OnCharacterHealthChanged.Unbind();
  }

  // Hide all hearts.
  for (TObjectPtr<UHUDHeart> pHeart : m_lHearts) pHeart->Hide();
}

void UHUDHeartBar::NativeConstruct() {
  Super::NativeConstruct();
}

void UHUDHeartBar::InitializeHearts(int32 _iMaxHearts) {
  if (IsValid(m_pHeartContainer)) {
    // Clean containers.
    m_pHeartContainer->ClearChildren();
    m_lHearts.Empty();

    // Create hearts to fill containers.
    for (int32 iIndex = 0; iIndex < _iMaxHearts; ++iIndex) {
      UHUDHeart* pHeart = CreateWidget<UHUDHeart>(this, m_oHeartWidgetClass);

      // Add space between hearts.
      if (UHorizontalBoxSlot* pHeartSlot = m_pHeartContainer->AddChildToHorizontalBox(pHeart)) {
        pHeartSlot->SetPadding(FMargin(iIndex > 0 ? m_fHeartPadding : FMargin(0.f, 0.f, 0.f, 0.f)));
      }

      m_lHearts.Add(pHeart);
    }

    m_bIsInitialized = true;
  }
}

void UHUDHeartBar::UpdateHearts(int32 _iCurrentHealth, int32 _iMaxHealth) {
  int32 iHeartNumber = m_lHearts.Num();

  // Convert HP into "heart units" (20 HP = 1 heart)
  float fHealthInHearts = ((float)_iCurrentHealth) / 20.f;

  for (int32 iIndex = 0; iIndex < iHeartNumber; iIndex++) {
    float fHeartHealth = FMath::Clamp(fHealthInHearts - iIndex, 0.f, 1.f);

    if (fHeartHealth > 0.5f) m_lHearts[iIndex]->SetHeartState(EHeartState::FULL);
    else if (fHeartHealth > 0) m_lHearts[iIndex]->SetHeartState(EHeartState::HALF);
    else m_lHearts[iIndex]->SetHeartState(EHeartState::EMPTY);
  }
}