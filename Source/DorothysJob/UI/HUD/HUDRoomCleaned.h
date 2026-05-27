/************************************************************************
 * @description: Represents the HUD's Room Cleaned Widget.
 * @author: Jaime Paramo
 * @date: 10/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDRoomCleaned.generated.h"

// Animation
class UWidgetAnimation;

// Component
class UImage;

// Gameplay
class UBaseRoom;

UCLASS()
class DOROTHYSJOB_API UHUDRoomCleaned : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> F@TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeInAnimation;
  /**
   * @brief -> F@TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeOutAnimation;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pBannerImage;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pRewardImage;

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
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Consumable Reward Texture", AllowPrivateAccesss = true))
  TObjectPtr<UTexture2D> m_pConsumableRewardTexture;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Time Reward Texture", AllowPrivateAccesss = true))
  TObjectPtr<UTexture2D> m_pTimeRewardTexture;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Special Reward Texture", AllowPrivateAccesss = true))
  TObjectPtr<UTexture2D> m_pSpecialRewardTexture;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Cleaning Reward Texture", AllowPrivateAccesss = true))
  TObjectPtr<UTexture2D> m_pCleaningRewardTexture;
  
  /**
   * @brief -> Triggers logic to update the visibility of the text when a floor is completely cleaned.
   */
  void UpdateFloorCleanedImage(UBaseRoom* Room);

  /**
   * @brief -> Delegate handle used to manage the binding for the room cleaned event.
   */
  FDelegateHandle m_oRoomCleanedHandle;
  /**
   * @brief -> Duration (in seconds) for which the "Room Cleaned!" message remains visible.
   */
  float m_fRoomCleanedTextTime = 4.0f;

  /**
   * @brief -> @TOFILL.
   */
  FTimerHandle oHideTextHandler;

  /**
   * @brief -> @TOFILL.
   */
  void PlayFadeOutAnimation();
};