/************************************************************************
 * @description: Represents the player's HUD widget.
 * @author: Jaime Paramo
 * @date: 10/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDPlayer.generated.h"

// Enum
enum class EHUDSection : uint8;

// Manager
class UBaseGameInstance;
class USettingsManager;

// Widget
class UHUDFPS;
class UHUDDorothy;
class UHUDConsumableInfo;
class UHUDRoomCleaned;
class UHUDRoomInfo;
class UHUDEvent;

UCLASS()
class DOROTHYSJOB_API UHUDPlayer : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Reference to the FPS HUD widget, which displays game execution's current Frames Per Second.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDFPS> m_pHUDFPSWidget;
  /**
   * @brief -> Reference to the dirt HUD widget, which displays the dirtiness information (including the minimap).
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDConsumableInfo> m_pHUDConsumableInfoWidget;
  /**
   * @brief -> Reference to the dirt HUD widget, which displays the dirtiness information (including the minimap).
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDDorothy> m_pHUDDorothyWidget;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDRoomCleaned> m_pHUDRoomCleanedWidget;
  /**
   * @brief -> Reference to the dirt HUD widget, which displays the dirtiness information (including the minimap).
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDRoomInfo> m_pHUDRoomInfoWidget;
  /**
   * @brief -> Reference to the dirt HUD widget, which displays the dirtiness information (including the minimap).
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDEvent> m_pHUDEvent;

  FName m_nConsumiblesName = "Consumibles";

  /**
   * @brief -> Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;
  /**
   * @brief -> Hides the UMG elements of this Widget.
   */
  virtual void Hide() override;

  /**
   * @brief -> Displays the tutorial UMG elements of this Widget on screen.
   */
  void ShowTutorial();
  /**
   * @brief -> Displays the HUB UMG elements of this Widget on screen.
   */
  void ShowHUB();

  /**
   * @brief 
   */
  UFUNCTION()
  void HandleRevealElement(EHUDSection _eHUDSection);
  /**
   * @brief 
   */
  void RevealConsumables();
  /**
   * @brief 
   */
  void RevealMinimap();
  /**
   * @brief
   */
  void RevealClock();
  /**
  * @brief
  */
  void RevealHealth();
  /**
   * @brief 
   */
  void RevealWeaponSpecial();
  /**
   * @brief 
   */
  void RevealEverything();

  /**
   * @brief -> Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;

private:
  /**
   * @brief 
   */
  FDelegateHandle oDelegateHandle;

  TObjectPtr<USettingsManager> m_pSettingsManager;
  TObjectPtr<UBaseGameInstance> m_pGameInstance;
};