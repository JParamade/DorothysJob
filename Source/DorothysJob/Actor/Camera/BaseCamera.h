/************************************************************************
 * @description: Isometric Camera Component
 * @author: Aurora
 * @date: [01/07/2025]
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "../BaseActor.h"
#include "BaseCamera.generated.h"

class UCameraComponent;
class UCameraFogOfWarComponent;
class UCameraShakeHandlerComponent;
class UPostProcessRooms;
class UBaseRoom;
class UMainLegacyCameraShake;

USTRUCT()
struct FSettingsForRooms
{
  GENERATED_BODY()

  int32 ID = -1;
  FPostProcessSettings Settings;
  FPostProcessSettings DirtSettings;
};

UCLASS()
class DOROTHYSJOB_API ABaseCamera : public ABaseActor
{
  GENERATED_BODY()

public:

  /** Default constructor */
  ABaseCamera();

protected:

  /** Execute at the begining of the game */
  virtual void BeginPlay() override;

public:

  /* Function called every frame, it will be use to recalculate the position to follow the player around */
  virtual void Tick(float DeltaTime) override;

protected:

#pragma region CameraComponents
  /** FogOfWar Component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13 | Camera", meta = (DisplayName = "Fog Of War Component"))
  TObjectPtr<UCameraFogOfWarComponent> m_pFogOfWarComponent;

  /** Camera Shake Handler Component */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13 | Camera", meta = (DisplayName = "Shake Handler Component"))
  TObjectPtr<UCameraShakeHandlerComponent> m_pShakeComponent;

  /** Camera Component */
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TObjectPtr<UCameraComponent> m_pCameraComponent;

  UPROPERTY()
  TArray<FSettingsForRooms> m_lCachedSettings;

  float m_fOrthoWidth = 1800.0f;
#pragma endregion

private:

#pragma region Edit Variables
  /** Movement Interpolation Speed */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | Camera", meta = (DisplayName = "Follow Speed"))
  float m_fMovementSpeed;

  /** Camera Initial Position respect Player*/
  UPROPERTY(EditAnywhere, Category = "Bola 13 | Camera", meta = (DisplayName = "Camera Pos"))
  FVector m_vMainCameraPos = FVector(-1000.0f, 1000.0f, 1500.0f);
#pragma endregion

#pragma region Variables
  /** Target Actor (Player in this case) */
  UPROPERTY()
  APawn* m_pTargetActor;

  /** Target Position */
  FVector m_vTargetPosition;

  /** New Camera Pos with the new target position*/
  FVector m_vCameraNewPos;

  /** The post process volume of the level.*/
  TObjectPtr<APostProcessVolume> m_pPPVolumeA;

  TObjectPtr<APostProcessVolume> m_pPPVolumeB;

  bool m_bUsingA = true;

  float BlendAlpha2 = 0.0f;

  float BlendDuration = 1.0f;

  float BlendTimer = 0.0f;

  bool bFoWInit = false;

  int32 PrevLevelID;

  bool m_bActiveCameraShake = false;

#pragma endregion

  void StartBlend(const FPostProcessSettings& NewSettings, float Duration);

#pragma region Functions
  /**
   * @brief Recalculate the camera position with the look ahead offset and the collision adjustment.
   */
  void RecalculateCameraPosition(float _fDeltaTime);

  /**
   * @brief Update the camera position with the desire destination.
   * @param _vDesiredCameraPos the desire destination to place the camera.
   */
  void UpdateCameraPosition(float _fDeltaTime);


  UFUNCTION()
  void HandleRoomChanged(int32 PrevRoomId, int32 NewRoomId);

  // CHANGE THIS TRO FUNCTIONS TO THE FOG OF WAR??
  /* Find the level Post process */
  void FindGlobalPPV(UWorld* World);

  /**
   * @brief Set the post process volume, the settings of it.
   * @param Level  The room where the character is.
   */
  UFUNCTION()
  void SetupPostProcessVolume(const UBaseRoom* Level);
#pragma endregion

  float m_fRunningShakeTime = 0.f;
  TSubclassOf<UMainLegacyCameraShake> m_LastShakeClass = nullptr;

public:

#pragma region Getters&Setters
  void SetupPostProcessAndRooms(TArray<TObjectPtr<const UPostProcessRooms>> _lPPVAndRooms);

  /**
   * @brief Get the target actor of the camera.
   */
  AActor* GetTargetActor();
  /**
   * @brief Set the target actor of the camera.
   * @param _TargetActor the new target actor.
   */
  void SetTargetActor(APawn* _TargetActor);

  /**
   * @brief Get the main camera component.
   */
  UCameraComponent* GetCameraComponent();

  /**
   * @brief Get the component in charge of the camera fog of war.
   */
  UCameraFogOfWarComponent* GetFogOfWarComponent();

  /**
   * @brief Play the Shake Component.
   */
  void PlayShakeComponent();

  /**
   * @brief Play the Shake Component.
   */
  void StopShakeComponent();

  /**
   * @brief Set the Shake Component.
   */
  void SetShakeComponent(TSubclassOf<UMainLegacyCameraShake> _ShakeClass);

  UFUNCTION()
  void HandleEnterRoom(UBaseRoom* EnteredRoom);

  UFUNCTION()
  void HandleExitRoom(UBaseRoom* EnteredRoom);

#pragma endregion

};
