/************************************************************************
 * @description: Isometric Camera Fog Of War Component
 * @author: Aurora
 * @date: [01/07/2025]
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraFogOfWarComponent.generated.h"

class ABaseCamera;

/**
 * @brief Component in charge of the Fog Of War.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UCameraFogOfWarComponent : public UActorComponent
{
  GENERATED_BODY()

public:

  /** Sets default values for this component's properties. */
  UCameraFogOfWarComponent();

protected:

  /** Execute at the begining of the game. */
  virtual void BeginPlay() override;

public:
  APostProcessVolume* PPVolume;

  /**
   * @brief Init Post Process material with the current roo, this is for when de play starts, in teh camera, call in the BeginPlay this function.
   * @param _iCurrentRoomId The Id of the initial room.
   */
  void InitPPMID(int32 _iCurrentRoomId);

  /**
   * @brief When the character change room, change th efog of war and the texture.
   * @param _iPrevRoomId Previous room Id.
   * @param _iCurrentRoomId Current Room Id.
   */
  void RoomChange(int32 _iPrevRoomId, int32 _iCurrentRoomId);

  /**
   * @brief Function to activate or desactivate the fog of war for debugging.
   */
  void ActivateOrDesactivateFogOfWar(bool _bActivation);
private:

#pragma region EditVariables
  UPROPERTY(EditAnywhere, Category = "Bola13|Debug", meta = (DisplayName = "Desactivate Fog Of War"))
  bool m_bActivateFogOfWar;
#pragma endregion
#pragma region Variables
  /** The owner of the component. */
  ABaseCamera* m_pCameraOwner;

  // This might become unnecessary since the size and pixels are calculated in BeginPlay.
  UPROPERTY(EditDefaultsOnly)
  int32 TextureWidth = 512;

  UPROPERTY(EditDefaultsOnly)
  int32 TextureHeight = 512;

  /** The Post Process Volum of the level. */
  //APostProcessVolume* PPVolume;

  /** The Post Process Material use. */
  UMaterialInterface* PostProcessMaterial;

  /** The Post process Material dynamic istance. */
  UPROPERTY()
  UMaterialInstanceDynamic* PostProcessMID;

  /** The */
  float TransitionAlpha = 0.f;

  /** If is changing rooms, activate to do the interpolation. */
  bool bIsTransitioning = false;

  /** The Interpolation Speed of the change. */
  float TransitionSpeed = 1.5f;
#pragma endregion

#pragma region Texture Function

  /** Tetxure Data. */
  uint8* TextureData;

  /** Total Bytes of Texture Data. */
  uint32 TextureDataSize;

  /** Texture Data Sqrt Size. */
  uint32 TextureDataSqrtSize;

  /** Total Count of Pixels in Texture. */
  uint32 TextureTotalPixels;

  /** Texture Object. */
  UPROPERTY()
  UTexture2D* DynamicTexture;

  /** Update Region Struct. */
  FUpdateTextureRegion2D* TextureRegion;

  /** 
   * @brief Create and initialize the texture for the PP Material. 
   */
  void InitializeTexture();

  /** 
   * @brief Change texture data with teh new room state, with teh color. 
   */
  void SetRoomState(int32 RoomIndex, FColor Color);

  /** 
   * @brief Update the dynamic texture of the Post Process Material. 
   */
  void UpdateTexture();
#pragma endregion
public:

#pragma region Getters&Setters
  /**
   * @brief Get the Post Process Dynamic Material. 
   */
  UMaterialInstanceDynamic* GetPostProcessMID() const;
#pragma endregion
};
