/************************************************************************
 * @description: Handles the widget to show current endless days
 * @author: Pablo Velasco
 * @date: 07/10/2025
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "EndlessDays.generated.h"

class UCanvasPanel;
class UTextBlock;

UCLASS()
class DOROTHYSJOB_API UEndlessDays : public UBaseWidget
{
	GENERATED_BODY()
public:
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeInAnimation;

  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeOutAnimation;

  virtual void Show() override;

  virtual void Hide() override;

  virtual void NativeConstruct() override;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pDayTextBlock;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Day Text"))
  FText m_sDayText;

protected:
  void PlayFadeOutAnimation();

private:
  FTimerHandle oHideDayHandler;	
};
