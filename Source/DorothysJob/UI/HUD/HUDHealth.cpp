/************************************************************************
 * @description: Represents the player's health widget.
 * @author: Jaime Paramo
 * @date: 10/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDHealth.h"

// Component
#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"

// Gameplay
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include <Kismet/GameplayStatics.h>

void UHUDHealth::Show() {
  Super::Show();

  // Bind to the BasePlayer's OnCharacterHealthChanged event to react when the current health is updated.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    pPlayerPawn->OnCharacterHealthChanged.BindUObject(this, &UHUDHealth::UpdateHealthBar);
  }
}

void UHUDHealth::Hide() {
  Super::Hide();

  // Remove event bindings.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    pPlayerPawn->OnCharacterHealthChanged.Unbind();
  }
}

void UHUDHealth::NativeConstruct() {
  Super::NativeConstruct();

  // Health Bar
  if (IsValid(m_pHealthBar)) m_lElements.Add(m_pHealthBar);
  if (IsValid(m_pDorothyText)) m_lElements.Add(m_pDorothyText);
}

void UHUDHealth::UpdateHealthBar(int32 _iNewCurrentHealth, int32 _iMaxHealth) {
  // Calculate health percentage.
  float fNewPercentage = static_cast<float>(_iNewCurrentHealth) / static_cast<float>(_iMaxHealth);

  // Update the health bar fill percentage with the new value.
  m_pHealthBar->SetPercent(fNewPercentage);

  // Show or hide the dirtiness text based on the player's current health.
  // If the player's health is depleted (0 or below), the dirtiness text is shown.
  m_pDirtinessText->SetVisibility(fNewPercentage <= 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}