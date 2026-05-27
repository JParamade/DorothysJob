// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "BaseTile.generated.h"

class UBaseRoom;
class UNiagaraSystem;

/**
 * @brief Tile tipe for searching
 */
UENUM(BlueprintType)
enum class ETileType : uint8
{
  FLOOR,
  WALL
};

UENUM(BlueprintType)
enum class EDrawTier : uint8
{
  Liquid,
  Dust1,
  Dust2
};

UENUM(BlueprintType)
enum class EDrawColor : uint8
{
  EBase,
  EOrange,
  ERed,
  EBlack
};

/**
 *
 */
UCLASS()
class DOROTHYSJOB_API ABaseTile : public ABaseActor
{
  GENERATED_BODY()

public:
  /**
   * @brief Base constructor for ABaseTile
   */
  ABaseTile();

  virtual void BeginPlay() override;

  /**
   * @brief Inicialization of the tile
   * @return If the inicialization was succesfull
   */
  UFUNCTION(BlueprintCallable, Category = "Setting", meta = (DisplayName = "InitTile"))
  virtual bool InitTile();

  /**
   * @brief BlueprintEvent that launch when Init end
   */
  UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
  void InitEnd();

  /**
   * @brief Draw on the RenderTexture
   * @param _texture the Texture you want to draw
   * @param _UVPosition The UV position on the Render Texture
   * @param _size The size of the objetc in the render Texture
   * @param _rotation The rottation on the rendern texture
   * @param _pivot The pivot of the drawing
   * @param _color The color of the drawing
   * @param _blendMode The blendMode of the drawing
   */
  UFUNCTION(BlueprintCallable, Category = "Draw")
  virtual bool Draw(UTexture* _textureLittering, UTexture* _textureCleaning, FVector2D _UVPosition, FVector2D _size, float _rotation, FVector2D _pivot, EDrawColor _color, EDrawTier _tierType, bool _isCleaning);

  /**
   * @brief Draw on the materialInstace
   * @param _renderTexture The texture that is going to bed drawn
   */
  UFUNCTION(BlueprintCallable, Category = "Draw")
  virtual void UpdateMaterial(UMaterialInstanceDynamic* _materialInstace);

  void UpdateCleaningPercentage();

  /**
   * @brief Draw on the MaterialInstance
   */
  UFUNCTION(BlueprintCallable, Category = "Draw")
  virtual void UpdateSelf();

#pragma region Getter
  /**
   * @brief Getter of the tipe of the Tile
   * @return The type of the tile
   */
  ETileType GetType() const;

  /**
   * @brief The hetter of the dinamic material
   * @return the material
   */
  UFUNCTION(BlueprintPure)
  UMaterialInstanceDynamic* GetMaterialInstance() const;

  /**
   * @brief The getter of the Render Texture
   * @return The texture
   */
  UFUNCTION(BlueprintPure)
  UTextureRenderTarget2D* GetRenderTexture2D() const;

  /**
   * @brief Getter of the percentage
   * @return The percentage
   */
  float GetPercetage() const;

  UFUNCTION(BlueprintPure)
  FName GetParamName() const;

  UFUNCTION(BlueprintPure)
  UStaticMeshComponent* GetStaticMesh() const;
#pragma endregion


#pragma region Setter


  UFUNCTION(BlueprintCallable)
  void SetStaticMesh(UStaticMeshComponent* _staticMeshComponent);

#pragma endregion


  /**
   * @brief On the destroy of the actor
   */
  virtual void BeginDestroy() override;

  void CleanFloor(bool _bCanShowVFX = true);

  void SetNegateDirt(bool _bNegateDirt);
protected:

  /**
   * @brief Type of the Tile
   */
  UPROPERTY(EditAnywhere, Category = "Type")
  ETileType m_eType;

  float m_fNotCleanPercetage = 0;

#pragma region Render Target Configuration

  /**
   * @brief Size of the Render Target
   */
  UPROPERTY(EditAnywhere, Category = "Render configuration")
  FVector2D m_vSize = FVector2D(1024, 1024);

  /**
   * @brief Name of the mask param on the material
   */
  UPROPERTY(EditAnywhere, Category = "Render configuration")
  FName m_nMaskParamTier = FName(TEXT("TierMask"));

  /**
 * @brief Name of the mask param on the material
 */
  UPROPERTY(EditAnywhere, Category = "Render configuration")
  FName m_nMaskParamColor = FName(TEXT("Floor Dirt"));

  /**
* @brief Name of the mask param on the material
*/
  UPROPERTY(EditAnywhere, Category = "Render configuration")
  FName m_nMaskParamDust = FName(TEXT("Floor Dust"));

  /**
   * @brief Color base of the render target
   */
  UPROPERTY(EditAnywhere, Category = "Render Configuration")
  FLinearColor m_baseColor;

  /**
   * @brief Material base to the Dinamic Material
   */
  UPROPERTY(EditAnywhere, Category = "Render Configuration")
  TObjectPtr<UMaterialInterface> m_baseMaterial;

  UPROPERTY(EditAnywhere, Category = "Bola13|Begin Dirty")
  bool m_bBeginDirty;

#pragma endregion

  /**
   * @brief Dinamic material that is going to be the new material to the Mesh
   */
  UPROPERTY(EditAnywhere, Category = "Core")
  TObjectPtr<UMaterialInstanceDynamic> m_materialInstance;

  /**
   * @brief Pointer to the Render Target of the tier Textute
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core")
  TObjectPtr<UTextureRenderTarget2D> m_pRenderTargetTier;

  /**
 * @brief Pointer to the Render Target of the dust Textute
 */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Core")
  TObjectPtr<UTextureRenderTarget2D> m_pRenderTargetDust;

  /**
   * @brief Pointer to the color Texture
   */
  UPROPERTY(EditAnywhere)
  TObjectPtr<UTextureRenderTarget2D> m_pRenderTargetColor;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UNiagaraSystem> m_pAutoCleanVFX;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UNiagaraSystem> m_pAutoCleanLoopVFX;

  UPROPERTY(EditAnywhere)
  TObjectPtr<UMaterialInterface> m_pOverlayMaterial;

  /**
   * @brief StaticMesh of the tile
   */
  UPROPERTY(VisibleAnywhere, Category = "Mesh")
  UStaticMeshComponent* m_staticMesh;

  FTextureRenderTargetResource* m_pRTResource;

  void TryRegisterInRoom(UBaseRoom* Room);

private:

  FLinearColor GetDrawColor(EDrawColor _color);

  UFUNCTION()
  void ClearOverlayMaterial();

  UPROPERTY()
  FTimerHandle OverlayTimerHandle;
  
  UPROPERTY(EditAnywhere)
  float m_fClearingOverlayMaterialCooldown = 1.0f;
};
