#include "ReplayResultsWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"

void UReplayResultsWidget::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
  }

  m_bIsVisible = false;
}

void UReplayResultsWidget::Show()
{
  if (IsValid(m_pNoHitStarImage))
  {
    m_pNoHitStarImage->SetBrushFromTexture(m_bNoHitStarEnabled ? m_pStarFullImage : m_pStarEmptyImage);
  }
  if (IsValid(m_pExtraTimeStarImage))
  {
    m_pExtraTimeStarImage->SetBrushFromTexture(m_bExtraTimeImage ? m_pStarFullImage : m_pStarEmptyImage);
  }
  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::HitTestInvisible);
  }
  m_bIsVisible = true;
}

void UReplayResultsWidget::Hide()
{
  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Hidden);
  }

  m_bIsVisible = false;
}