/************************************************************************
 * @description: Represents the enemy status World Space Widget.
 * @author: Jaime Paramo
 * @date: 17/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "WorldStatus.generated.h"

// Component
class UImage;

// Status
class UBaseStatus;
class ABaseCharacter;

UCLASS()
class DOROTHYSJOB_API UWorldStatus : public UBaseWidget
{
	GENERATED_BODY()
	
public:

  void InitWidget(ABaseCharacter* _pOwner);
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pExposureStatusImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSlowStatusImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pStunStatusImage;
  
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
   * @brief 
   */
  int m_iAuxStacks = 0;

  UFUNCTION()
  void UpdateStatusInfo(UBaseStatus* _pStatus, bool _bActive);

  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Dizzy Tier Texture", AllowPrivateAccess = true))
  TArray<TObjectPtr<UTexture2D>> m_lDizzyTierTextures;

  UPROPERTY()
  TObjectPtr<ABaseCharacter> m_pOwnerCharacter;
};
