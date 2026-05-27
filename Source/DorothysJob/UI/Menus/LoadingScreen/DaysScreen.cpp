// Fill out your copyright notice in the Description page of Project Settings.


#include "DaysScreen.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DorothysJob/Data/DataAssets/DaysDataAsset.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"

// Animation
#include "Animation/WidgetAnimation.h"

void UDaysScreen::Show()
{
  Super::Show();

  int DayIndex = 0;
  UBaseGameInstance* MyGI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
  if (MyGI)
  {
    DayIndex = MyGI->m_iCurrentLevel;
  }

  if(!m_pDaysDataAsset)
  {
    return;
  }

  if (UTexture2D** FoundTexture = m_pDaysDataAsset->m_oDaysList.Find(DayIndex))
  {
    UTexture2D* Texture = *FoundTexture;

    if (Texture && m_pDayBackground)
    {
      // Crear un brush temporal
      FSlateBrush Brush;
      Brush.SetResourceObject(Texture);
      Brush.ImageSize = FVector2D(Texture->GetSizeX(), Texture->GetSizeY());

      // Asignar el brush a la imagen
      m_pDayBackground->SetBrush(Brush);

      m_pDayBackground->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

      if (IsValid(m_pFadeInAnimation)) PlayAnimation(m_pFadeInAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
    }
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("Didn't Find day texture %d"), DayIndex);
  }
}

void UDaysScreen::Hide()
{
  PlayFadeOutAnimation();
}

void UDaysScreen::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pCanvasPanel)) m_lElements.Add(m_pCanvasPanel);
}

void UDaysScreen::PlayFadeOutAnimation() {
  // If the fade out animation isn't valid, there’s nothing to play.
  if (!IsValid(m_pFadeOutAnimation)) return;

  // Get the length of the fade out animation (used for timing the next step).
  const float fFadeDuration = m_pFadeOutAnimation->GetEndTime();

  PlayAnimation(m_pFadeOutAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);

  TWeakObjectPtr<UDaysScreen> WeakThis(this);
  GetWorld()->GetTimerManager().SetTimer(
    oHideDayHandler,
    [WeakThis]() {
      if (WeakThis.IsValid() && IsValid(WeakThis->m_pDayBackground)) {
        WeakThis->m_pDayBackground->SetVisibility(ESlateVisibility::Hidden);
      }
    },
    fFadeDuration,
    false
  );
}
