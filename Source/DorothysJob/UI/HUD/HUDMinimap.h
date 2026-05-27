/************************************************************************
 * @description: Represents the Widget of the minimap in the User's Interface
 * @author: Aurora Fernández Basanta
 * @date: 01/07/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDMinimap.generated.h"

class UCanvasPanel;
class UImage;
class UBaseRoom;
enum class EDirtStage : uint8;
enum class ERoomShape : uint8;
enum class ERoomType : uint8;
class URoomShapeImageData;
class URoomIconData;
class ULevelManager;
class ADissolvableDoor;

struct FRoomCached
{
  int32 ID = -1;
  float     Yaw = 0.0f;
  FVector2D Min = FVector2D::ZeroVector;
  FVector2D Max = FVector2D::ZeroVector;
  FVector2D Center = FVector2D::ZeroVector;
  FVector2D SizeWorld = FVector2D::ZeroVector;
  EDirtStage DirtStage;
  ERoomShape RoomShape;

  TObjectPtr<UTexture2D> CleanTexture = nullptr;
  TObjectPtr<UTexture2D> MidTexture = nullptr;
  TObjectPtr<UTexture2D> DirtyTexture = nullptr;

  TObjectPtr<UImage> CleanImage = nullptr;
  TObjectPtr<UImage> MidImage = nullptr;
  TObjectPtr<UImage> DirtyImage = nullptr;

  TObjectPtr<UTexture2D> RewardIconTexture = nullptr;
  TObjectPtr<UImage> RewardIconImage = nullptr;

  TObjectPtr<UTexture2D> HighlightFrameTexture = nullptr;
  TObjectPtr<UImage> HighlightFrameImage = nullptr;
};

struct FDoorCached
{
  FVector Location;
  bool bIsOpen;
  TObjectPtr<UImage> DoorImage = nullptr;
  TWeakObjectPtr<ADissolvableDoor> DoorActor;
};

UCLASS()
class DOROTHYSJOB_API UHUDMinimap : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Reference to the Text Block displaying the minutes of the game timer. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pMinimapImage;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pRewardImage;

  virtual void Show() override;
  virtual void Hide() override;
  virtual void NativeConstruct() override;

protected:
#pragma region | Customizable Variables
  UPROPERTY(meta = (BindWidget))
  UCanvasPanel* MinimapRoomCanvas;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Minimap")
  URoomShapeImageData* RoomShapeDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Minimap")
  URoomIconData* RoomTypeDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Minimap")
  TObjectPtr<UTexture2D> DoorTexture = nullptr;
#pragma endregion

private:
#pragma region | Variables
  float MinX = FLT_MAX;

  float MaxX = -FLT_MAX;

  float MinY = FLT_MAX;

  float MaxY = -FLT_MAX;

  float Scale = -1.0f;

  float OffsetX = 1.0f;

  float OffsetY = 1.0f;

  bool bWidgetsInitialized = false;

  int32 PlayerRoomIndex = -1;

  TObjectPtr<ULevelManager> m_pLevelManager;

  TArray<TObjectPtr<UBaseRoom>> m_pRooms;

  TArray<FRoomCached> Cached;

  TArray<FDoorCached> DoorCached;

  UPROPERTY(EditAnywhere)
  FVector2D m_vCanvasSize = FVector2D::ZeroVector;
#pragma endregion

#pragma region | Functions
  void MinimapActivation();

  void GetLevelRooms();

  void BuildOrUpdateRoomCache();

  UTexture2D* GetIconByRoom(UBaseRoom* Room);

  void CreateRoomWidgetsIfNeeded();

  void UpdateRoomWidgetAppearance(const FRoomCached& Room);

  UFUNCTION()
  void UpdateAllRoomColors();

  UFUNCTION()
  void MoveDorothyToRoom(UBaseRoom* Room);

  FIntPoint GetRoomShapeSize(ERoomShape Shape, float Rotation);

  void GetRoomAABB(const FVector& Pivot, const FIntPoint& TileCount, float YawDeg, float TileSizeWorld, FVector2D& OutMin, FVector2D& OutMax);

  void BuildOrUpdateDoorCache();

  void CreateDoorWidgetsIfNeeded();

  void UpdateRoomWidgetVisibility(const FRoomCached& Room) const;

  int32 GetTextureIndexForDirtStage(EDirtStage Stage) const;
#pragma endregion
};
