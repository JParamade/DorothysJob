/************************************************************************
 * @description: It represents the replay score widget.
 * @author: David Gonzalez
 * @date: 21/10/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "ReplayResultsWidget.generated.h"

class UCanvasPanel;
class UImage;

UCLASS()
class DOROTHYSJOB_API UReplayResultsWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:

  /**
   * @brief The default Constructor of the widget.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief This will activate the widget.
   */
  virtual void Show() override;

  /**
   * @brief This will deactivate the widget.
   */
  virtual void Hide() override;

  /**
   * It enables/disables no hit star.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Data", meta = (DisplayName = "No Hit Star", AllowPrivateAccess = "true"))
  bool m_bNoHitStarEnabled;

  /**
   * It enables/disables extra time star.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Data", meta = (DisplayName = "Extra Time Star", AllowPrivateAccess = "true"))
  bool m_bExtraTimeImage;

private:

  /**
   * The image to show when the star is disabled.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Star Empty", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pStarEmptyImage;

  /**
   * The image to show when the star is enabled.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Star Full", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pStarFullImage;

  /**
   * This represents the widget main panel.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  /**
   * @brief It is the no hit star image in the screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pNoHitStarImage;

  /**
   * @brief It is the extra time star image in the screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pExtraTimeStarImage;
};
