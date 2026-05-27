/************************************************************************
 * @description: Represents the UMG loading Throbber class used in the loading screen.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "LoadingThrobber.generated.h"

// Component
class UImage;

UCLASS()
class DOROTHYSJOB_API ULoadingThrobber : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Circular throbber widget for the loading screen. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pLoadingThrobber;

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

private: 
  /**
   * @brief -> Time in seconds between changing animation frames.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Loading", meta = (DisplayName = "Time Between Frames"))
  float m_fTimeBetweenFrames = .083f;
  /**
   * @brief -> Handle for the timer controlling frame changes.
   */
  UPROPERTY()
  FTimerHandle m_oTimeBetweenFramesHandle;

  /**
   * @brief -> Array of textures used for the loading animation frames.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Loading", meta = (DisplayName = "Loading Sprites"))
  TArray<TObjectPtr<UTexture2D>> m_lLoadingTextures;
  /**
   * @brief -> Index of the current texture frame being displayed.
   */
  int32 m_iCurrentFrame = 0;
  /**
   * @brief -> Advances the throbber animation to the next texture frame.
   */
  void NextFrame();
};