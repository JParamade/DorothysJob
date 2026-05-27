#include "EndlessDays.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Components/CanvasPanel.h"

void UEndlessDays::Show()
{
  Super::Show();

  int DayIndex = 0;
  UBaseGameInstance* MyGI = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
  if (MyGI)
  {
    DayIndex = MyGI->m_iCurrentEndlessLevel + 1;
  }

  if (IsValid(m_pDayTextBlock))
  {
    m_pDayTextBlock->SetText(FText::Format(m_sDayText, FText::AsNumber(DayIndex)));
    if (IsValid(m_pFadeInAnimation)) 
    {
      PlayAnimation(m_pFadeInAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
    }
  }
}

void UEndlessDays::Hide()
{
  PlayFadeOutAnimation();
}

void UEndlessDays::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pCanvasPanel)) 
  {
    m_lElements.Add(m_pCanvasPanel);
  }
}

void UEndlessDays::PlayFadeOutAnimation()
{
  // If the fade out animation isn't valid, there’s nothing to play.
  if (!IsValid(m_pFadeOutAnimation)) return;

  // Get the length of the fade out animation (used for timing the next step).
  const float fFadeDuration = m_pFadeOutAnimation->GetEndTime();

  PlayAnimation(m_pFadeOutAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);

  TWeakObjectPtr<UEndlessDays> WeakThis(this);
  GetWorld()->GetTimerManager().SetTimer(
    oHideDayHandler,
    [WeakThis]()
    {
      if (WeakThis.IsValid() && IsValid(WeakThis->m_pCanvasPanel))
      {
        WeakThis->m_pCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
      }
    },
    fFadeDuration,
    false
  );
}
