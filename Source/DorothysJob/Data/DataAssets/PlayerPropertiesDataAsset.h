/************************************************************************
 * @description: It represents the Player properties Data Asset.
 * @author: David Gonzalez
 * @date: 12/08/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "CharacterPropertiesDataAsset.h"
#include "PlayerPropertiesDataAsset.generated.h"

class UInputMappingContext;
class UInputAction;
class ABaseCamera;
class UMainLegacyCameraShake;

USTRUCT(BlueprintType)
struct FPlayerGamePadVibrationParams
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Intensity"))
  float m_fIntensity = 0.01f;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Durantion"))
  float m_fDuration = 0.01f;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Left Large"))
  bool m_bAffectsLeftLarge = true;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Left Samll"))
  bool m_bAffectsLeftSmall = true;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Right Large"))
  bool m_bAffectsRightLarge = true;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Right Small"))
  bool m_bAffectsRightSmall = true;
};


UCLASS()
class DOROTHYSJOB_API UPlayerPropertiesDataAsset : public UCharacterPropertiesDataAsset
{
	GENERATED_BODY()
	
  public:

#pragma region | Camera
  /**
   * It is the camera class to attach to the player.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Camera", meta = (DisplayName = "Camera Class"))
  TSubclassOf<ABaseCamera> m_oCameraClass;

  /**
 * It is the camera class to attach to the player.
 */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Camera", meta = (DisplayName = "Damage Camera Shake"))
  TSubclassOf<UMainLegacyCameraShake> m_oDamageShakeBP;

  /**
 * @brief Weapon special attack vibration.
 */
  UPROPERTY(EditAnywhere, Category = "Bola 13|GamePad", meta = (DisplayName = "Damage Controller Vibration"))
  FPlayerGamePadVibrationParams m_oDamageVibrationParams;

  /**
* @brief Weapon special attack vibration.
*/
  UPROPERTY(EditAnywhere, Category = "Bola 13|GamePad", meta = (DisplayName = "Skinky Controller Vibration"))
  FPlayerGamePadVibrationParams m_oStinkyVibrationParams;
#pragma endregion

#pragma region | Movement
  /**
   * It is the rotation speed for the player.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Rotation Interpolation Speed"))
  float m_fRotInterpSpeed;

  /**
   * It is the player dash distance.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Dash Distance"))
  float m_fDashDistance;

  /**
   * It is the player dash time.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Dash Time"))
  float m_fDashTime;

  /**
   * It is the player dash cooldown.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Dash Cooldown"))
  float m_fDashCooldown;
#pragma endregion

#pragma region Input Actions
  /**
   * Attack Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Attack Action"))
  TObjectPtr<UInputAction> m_pAttackAction;

  /**
   * Clean Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Clean Action"))
  TObjectPtr<UInputAction> m_pCleanAction;

  /**
   * Change Weapon Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Change Weapon Action"))
  TObjectPtr<UInputAction> m_pWeaponChangeAction;

  /**
   * Move Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Move Action"))
  TObjectPtr<UInputAction> m_pMoveAction;

  /**
   * LookAt Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "LookAt Action"))
  TObjectPtr<UInputAction> m_pLookAtAction;

  /**
   * Dash Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Dash Action"))
  TObjectPtr<UInputAction> m_pDashAction;

  /**
   * Interact Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Interact Action"))
  TObjectPtr<UInputAction> m_pInteractAction;

  /**
   * Special Attck Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Special Attack Action"))
  TObjectPtr<UInputAction> m_pSpecialAttackAction;

  /**
   * Use Primary Consumable Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Use Primary Consumable Action"))
  TObjectPtr<UInputAction> m_pUsePrimaryConsumableAction;

  /**
   * Use Secondary Consumable Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Use Secondary Consumable Action"))
  TObjectPtr<UInputAction> m_pUseSecondaryConsumableAction;

  /**
   * Pause Input Action
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Input", meta = (DisplayName = "Pause Action"))
  TObjectPtr<UInputAction> m_pPauseAction;
#pragma endregion

#pragma region | Skin
  /**
   * It will be used when Player is dirty.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Skins", meta = (DisplayName = "Dirty Skin"))
  TArray<TObjectPtr<UMaterialInterface>> m_lDirtyMaterial;
#pragma endregion

#pragma region | Dirt
  /**
   * It represents the cooldown to dirt.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Cooldown"))
  float m_fDirtCooldown;

  /**
   * It represents the cooldown to dirt when the Player is Dashing.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dashing Cooldown"))
  float m_fDashingDirtCooldown;

  /**
   * It represents the step rotation angle for the dirt texture.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Step rotation angle"))
  float m_fStepRotationAngle;

  /**
   * Dirt Size.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Size"))
  FVector2D m_vDirtSize;

  /**
   * Dirt texture.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Texture"))
  TObjectPtr<UTexture2D> m_pDirtTexture;

  /**
   * Clean texture.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Clean Texture"))
  TObjectPtr<UTexture2D> m_pCleanTexture;
#pragma endregion

#pragma region | UI
  /**
   * It's the widget with the player direction image.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|UI", meta = (DisplayName = "Direction Widget Class"))
  TSubclassOf<UUserWidget> m_oDirectionWidgetClass;

  /**
   * It will be used to show the aim direction.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Aim VFX"))
  TObjectPtr<UNiagaraSystem> m_pAimVFX;
#pragma endregion

#pragma region | VFX
  /**
   * It will be used when player is dashing.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Dash VFX"))
  TObjectPtr<UNiagaraSystem> m_pDashEffect;
#pragma endregion

#pragma region | Audio
  /**
   * It will be executed when the Base Player uses dash action.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Dash"))
  TObjectPtr<UFMODEvent> m_pDashEvent;

  /**
   * @brief 
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Unavailable Sound"))
  TObjectPtr<UFMODEvent> m_pUnavailableEvent;
#pragma endregion

  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Pristine Time VFX"))
  TObjectPtr<UNiagaraSystem> m_pPristineTimeVFX;
};
