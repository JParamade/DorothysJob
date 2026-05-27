/************************************************************************
 * @description: Represents the Widget of the minimap in the User's Interface
 * @author: Aurora Fernández Basanta
 * @date: 01/07/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDMinimap.h"	
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Data/DataAssets/RoomShapeImageData.h"
#include "DorothysJob/Data/DataAssets/RoomIconData.h"
#include "DorothysJob/Actor/Dissolvable/DissolvableDoor.h"

void UHUDMinimap::Show()
{
  Super::Show();

  if (UWorld* World = GetWorld())
  {
    m_pLevelManager = World->GetSubsystem<ULevelManager>();
    if (!m_pLevelManager)
    {
      UE_LOG(LogTemp, Warning, TEXT("[HUD Minimap] Could not find Level Manager"));
      return;
    }
  }

  MinimapActivation();

  UBaseRoom::OnDirtStageChanged.AddUObject(this, &UHUDMinimap::UpdateAllRoomColors);

  m_pLevelManager->OnEnteredRoom.RemoveDynamic(this, &UHUDMinimap::MoveDorothyToRoom);
  m_pLevelManager->OnEnteredRoom.AddDynamic(this, &UHUDMinimap::MoveDorothyToRoom);
}

void UHUDMinimap::Hide()
{
  Super::Hide();
  UBaseRoom::OnDirtStageChanged.RemoveAll(this);

  if (IsValid(m_pLevelManager))
  {
    m_pLevelManager->OnEnteredRoom.RemoveDynamic(this, &UHUDMinimap::MoveDorothyToRoom);
  }
}

void UHUDMinimap::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pMinimapImage) && IsValid(MinimapRoomCanvas))
  {
    m_lElements.Add(m_pMinimapImage);
    m_lElements.Add(MinimapRoomCanvas);
    m_lElements.Add(m_pRewardImage);
  }
}

#pragma region | Functions
void UHUDMinimap::MinimapActivation()
{
  GetLevelRooms();
  BuildOrUpdateRoomCache();
  CreateRoomWidgetsIfNeeded();

  if (IsValid(m_pLevelManager) && IsValid(m_pLevelManager->GetCurrentRoom()))
  {
    MoveDorothyToRoom(m_pLevelManager->GetCurrentRoom());
  }

  BuildOrUpdateDoorCache();
  CreateDoorWidgetsIfNeeded();
  UpdateAllRoomColors();

  MinimapRoomCanvas->InvalidateLayoutAndVolatility();
}

void UHUDMinimap::GetLevelRooms()
{
  if (IsValid(m_pLevelManager) && m_pRooms.IsEmpty())
  {
    m_pRooms = m_pLevelManager->GetRooms();
  }
}

void UHUDMinimap::BuildOrUpdateRoomCache()
{
  if (m_pRooms.Num() == 0)
  {
    return;
  }

  const float TileSizeWorld = 1600.f;
  FVector2D CanvasSize = FVector2D(205.0f, 205.0f);

  MinX = FLT_MAX;
  MaxX = -FLT_MAX;
  MinY = FLT_MAX;
  MaxY = -FLT_MAX;

  if (Cached.Num() == 0)
  {
    Cached.Empty();
    for (const auto& Room : m_pRooms)
    {
      FVector RoomPos = Room->GetLocation();
      float RoomRot = Room->GetRotation();
      FIntPoint TileCount = GetRoomShapeSize(Room->GetRoomShape(), RoomRot);

      FVector2D Min, Max;
      GetRoomAABB(Room->GetLocation(), TileCount, RoomRot, TileSizeWorld, Min, Max);

      MinX = FMath::Min(MinX, Min.X);
      MaxX = FMath::Max(MaxX, Max.X);
      MinY = FMath::Min(MinY, Min.Y);
      MaxY = FMath::Max(MaxY, Max.Y);

      FRoomCached C;
      C.ID = Room->GetLevelId();
      C.DirtStage = Room->GetDirtStage();
      C.Min = Min;
      C.Max = Max;
      C.Center = (Min + Max) * 0.5f;
      C.SizeWorld = (Max - Min).GetAbs();
      C.Yaw = RoomRot;
      C.RoomShape = Room->GetRoomShape();

      if (IsValid(RoomShapeDataAsset))
      {
        const FRoomShapeInfo* Info = RoomShapeDataAsset->GetInfoForShape(Room->GetRoomShape());
        if (Info)
        {
          C.CleanTexture = Info->TextureClean;
          C.MidTexture = Info->TextureMid;
          C.DirtyTexture = Info->TextureDirty;
          C.HighlightFrameTexture = Info->Frame;
        }
      }

      C.RewardIconTexture = GetIconByRoom(Room);
      Cached.Add(C);
    }

    float WorldWidth = MaxX - MinX;
    float WorldHeight = MaxY - MinY;
    Scale = FMath::Min(CanvasSize.X / WorldWidth, CanvasSize.Y / WorldHeight);
    OffsetX = (CanvasSize.X - WorldWidth * Scale) * 0.5f;
    OffsetY = (CanvasSize.Y - WorldHeight * Scale) * 0.5f;
  }
  else
  {
    for (FRoomCached& C : Cached)
    {
      for (const auto& Room : m_pRooms)
      {
        if (Room->GetLevelId() == C.ID)
        {
          C.DirtStage = Room->GetDirtStage();
          break;
        }
      }
    }
  }
}

UTexture2D* UHUDMinimap::GetIconByRoom(UBaseRoom* Room)
{
  if (!IsValid(RoomTypeDataAsset) || !IsValid(Room))
  {
    return nullptr;
  }

  return Room->GetRoomShape() == ERoomShape::Q1 && Room->HasCleaningStation() ? RoomTypeDataAsset->CleaningStationIcon : nullptr;
}

void UHUDMinimap::CreateRoomWidgetsIfNeeded()
{
  if (!MinimapRoomCanvas)
  {
    return;
  }

  for (FRoomCached& Room : Cached)
  {
    FVector2D CanvasPos((Room.Center.X - MinX) * Scale + OffsetX, (Room.Center.Y - MinY) * Scale + OffsetY);
    FVector2D SizePx = Room.SizeWorld * Scale;
    if (FMath::Abs((int32)Room.Yaw) == 90 || FMath::Abs((int32)Room.Yaw) == 270)
    {
      SizePx = FVector2D(SizePx.Y, SizePx.X);
    }

    // ROOM SHAPES
    // -- clean room image
    if (Room.CleanTexture && !Room.CleanImage)
    {
      UImage* RoomIcon = NewObject<UImage>(MinimapRoomCanvas);
      RoomIcon->SetBrushFromTexture(Room.CleanTexture);
      RoomIcon->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
      RoomIcon->SetRenderTransformAngle(Room.Yaw);

      MinimapRoomCanvas->AddChild(RoomIcon);
      Room.CleanImage = RoomIcon;

      if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(RoomIcon->Slot))
      {
        PanelSlot->SetAlignment({ 0.5f, 0.5f });
        PanelSlot->SetSize(SizePx);
        PanelSlot->SetPosition(CanvasPos);
      }
    }

    // -- mid room image
    if (Room.MidTexture && !Room.MidImage)
    {
      UImage* RoomIcon = NewObject<UImage>(MinimapRoomCanvas);
      RoomIcon->SetBrushFromTexture(Room.MidTexture);
      RoomIcon->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
      RoomIcon->SetRenderTransformAngle(Room.Yaw);

      MinimapRoomCanvas->AddChild(RoomIcon);
      Room.MidImage = RoomIcon;

      if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(RoomIcon->Slot))
      {
        PanelSlot->SetAlignment({ 0.5f, 0.5f });
        PanelSlot->SetSize(SizePx);
        PanelSlot->SetPosition(CanvasPos);
      }
    }

    // -- dirty room image
    if (Room.DirtyTexture && !Room.DirtyImage)
    {
      UImage* RoomIcon = NewObject<UImage>(MinimapRoomCanvas);
      RoomIcon->SetBrushFromTexture(Room.DirtyTexture);
      RoomIcon->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
      RoomIcon->SetRenderTransformAngle(Room.Yaw);

      MinimapRoomCanvas->AddChild(RoomIcon);
      Room.DirtyImage = RoomIcon;

      if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(RoomIcon->Slot))
      {
        PanelSlot->SetAlignment({ 0.5f, 0.5f });
        PanelSlot->SetSize(SizePx);
        PanelSlot->SetPosition(CanvasPos);
      }
    }

    UpdateRoomWidgetVisibility(Room);

    // REWARD ICON
    if (Room.RewardIconTexture && !Room.RewardIconImage)
    {
      UImage* IconImage = NewObject<UImage>(MinimapRoomCanvas);
      IconImage->SetBrushFromTexture(Room.RewardIconTexture);
      IconImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
      IconImage->SetRenderTransformAngle(Room.RoomShape == ERoomShape::Q1 ? 0.0f : 45.0f);

      MinimapRoomCanvas->AddChild(IconImage);
      Room.RewardIconImage = IconImage;

      FVector2D SizePx2 = Room.RoomShape == ERoomShape::Q1 ? FVector2D(1600.0f * Scale, 1600.0f * Scale) : FVector2D(1600.0f * Scale, 1600.0f * Scale);
      if (Room.RoomShape == ERoomShape::L3 || Room.RoomShape == ERoomShape::L4)
      {
        if ((int32)Room.Yaw == 180)
        {
          CanvasPos += SizePx2 * 0.5f;
        }
        else if ((int32)Room.Yaw == 90)
        {
          CanvasPos.X += SizePx2.X * 0.5f;
          CanvasPos.Y -= SizePx2.Y * 0.5f;
        }
        else if ((int32)Room.Yaw == 270)
        {
          CanvasPos.X -= SizePx2.X * 0.5f;
          CanvasPos.Y += SizePx2.Y * 0.5f;
        }
        else
        {
          CanvasPos -= SizePx2 * 0.5f;
        }
      }

      if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(IconImage->Slot))
      {
        PanelSlot->SetPosition(CanvasPos);
        PanelSlot->SetSize(SizePx2);
        PanelSlot->SetAlignment({ 0.5f, 0.5f });
      }
    }

    // Frame color
    if (Room.HighlightFrameTexture && !Room.HighlightFrameImage)
    {
      UImage* FrameImage = NewObject<UImage>(MinimapRoomCanvas);
      FrameImage->SetBrushFromTexture(Room.HighlightFrameTexture);
      FrameImage->SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
      FrameImage->SetRenderTransformAngle(Room.Yaw);
      FrameImage->SetVisibility(ESlateVisibility::Hidden);

      MinimapRoomCanvas->AddChild(FrameImage);
      Room.HighlightFrameImage = FrameImage;

      FVector2D SizePx2 = Room.SizeWorld * Scale;
      if (FMath::Abs((int32)Room.Yaw) == 90 || FMath::Abs((int32)Room.Yaw) == 270)
      {
        SizePx2 = FVector2D(SizePx2.Y, SizePx2.X);
      }
      FVector2D CanvasPos2((Room.Center.X - MinX) * Scale + OffsetX, (Room.Center.Y - MinY) * Scale + OffsetY);

      if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(FrameImage->Slot))
      {
        PanelSlot->SetAlignment({ 0.5f, 0.5f });
        PanelSlot->SetSize(SizePx2);
        PanelSlot->SetPosition(CanvasPos2);
      }
    }
  }
}

void UHUDMinimap::UpdateRoomWidgetAppearance(const FRoomCached& Room)
{
  FVector2D SizePx = Room.SizeWorld * Scale;
  if (FMath::Abs((int32)Room.Yaw) == 90 || FMath::Abs((int32)Room.Yaw) == 270)
  {
    SizePx = FVector2D(SizePx.Y, SizePx.X);
  }
  const FVector2D CanvasPos((Room.Center.X - MinX) * Scale + OffsetX, (Room.Center.Y - MinY) * Scale + OffsetY);

  auto UpdateSlot = [&](UImage* Img)
    {
      if (!Img)
      {
        return;
      }

      Img->SetRenderTransformAngle(Room.Yaw);

      if (UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Img->Slot))
      {
        Slot->SetSize(SizePx);
        Slot->SetPosition(CanvasPos);
        Slot->SetAlignment({ 0.5f, 0.5f });
      }
    };

  UpdateSlot(Room.CleanImage);
  UpdateSlot(Room.MidImage);
  UpdateSlot(Room.DirtyImage);

  UpdateRoomWidgetVisibility(Room);
}

void UHUDMinimap::UpdateAllRoomColors()
{
  BuildOrUpdateRoomCache();

  for (const FRoomCached& Room : Cached)
  {
    UpdateRoomWidgetVisibility(Room);
  }
}

void UHUDMinimap::MoveDorothyToRoom(UBaseRoom* Room)
{
  if (!Room)
    return;

  if (IsValid(m_pRewardImage) && IsValid(RoomTypeDataAsset))
  {
    m_pRewardImage->SetBrushFromTexture(RoomTypeDataAsset->GetRewardTextureForType(Room));
  }

  for (FRoomCached& C : Cached)
  {
    if (C.HighlightFrameImage)
    {
      C.HighlightFrameImage->SetVisibility(ESlateVisibility::Hidden);
    }
  }

  for (FRoomCached& C : Cached)
  {
    if (Room && C.ID == Room->GetLevelId() && C.HighlightFrameImage)
    {
      C.HighlightFrameImage->SetVisibility(ESlateVisibility::Visible);
      return;
    }
  }
}

FIntPoint UHUDMinimap::GetRoomShapeSize(ERoomShape Shape, float Rotation)
{
  FIntPoint Size(0, 0);

  switch (Shape)
  {
  case ERoomShape::Q1:
    Size = FIntPoint(1, 1);
    break;
  case ERoomShape::R2:
    Size = FIntPoint(1, 2);
    break;
  case ERoomShape::R3:
    Size = FIntPoint(1, 3);
    break;
  case ERoomShape::L3:
    Size = FIntPoint(2, 2);
    break;
  case ERoomShape::L4:
    Size = FIntPoint(2, 3);
    break;
  case ERoomShape::S:
    Size = FIntPoint(2, 3);
    break;
  case ERoomShape::T:
    Size = FIntPoint(2, 3);
    break;
  case ERoomShape::Q4:
    Size = FIntPoint(2, 2);
    break;
  default:
    return FIntPoint(1, 1);
  }
  if (FMath::Abs((int32)Rotation) == 90 || FMath::Abs((int32)Rotation) == 270)
  {
    Size = FIntPoint(Size.Y, Size.X);
  }
  return Size;
}

void UHUDMinimap::GetRoomAABB(const FVector& Pivot, const FIntPoint& TileCount, float YawDeg, float TileSizeWorld, FVector2D& OutMin, FVector2D& OutMax)
{
  const float W = TileCount.X * TileSizeWorld;
  const float H = TileCount.Y * TileSizeWorld;
  OutMin = { Pivot.X        , Pivot.Y };
  OutMax = { Pivot.X + W    , Pivot.Y + H };
}

void UHUDMinimap::BuildOrUpdateDoorCache()
{
  if (!DoorCached.IsEmpty())
  {
    return;
  }

  UWorld* World = GetWorld();
  if (!World) return;

  TArray<AActor*> FoundDoors;
  UGameplayStatics::GetAllActorsOfClass(World, ADissolvableDoor::StaticClass(), FoundDoors);

  for (AActor* Actor : FoundDoors)
  {
    ADissolvableDoor* Door = Cast<ADissolvableDoor>(Actor);
    if (!Door || Door->IsDoorClosed()) continue;

    FDoorCached NewDoor;
    NewDoor.Location = Door->GetActorLocation();
    NewDoor.bIsOpen = !Door->IsDoorClosed();
    NewDoor.DoorActor = Door;

    DoorCached.Add(NewDoor);
  }
}
void UHUDMinimap::CreateDoorWidgetsIfNeeded()
{
  if (!MinimapRoomCanvas) return;

  for (FDoorCached& Door : DoorCached)
  {
    if (Door.DoorImage) continue;

    UImage* DoorIcon = NewObject<UImage>(MinimapRoomCanvas);
    DoorIcon->SetBrushFromTexture(DoorTexture);
    MinimapRoomCanvas->AddChild(DoorIcon);

    FVector2D CanvasPos(
      (Door.Location.X - MinX) * Scale + OffsetX,
      (Door.Location.Y - MinY) * Scale + OffsetY
    );

    if (UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(DoorIcon->Slot))
    {
      PanelSlot->SetAlignment({ 0.5f, 0.5f });
      PanelSlot->SetSize({ 600.0f * Scale, 600.0f * Scale });
      PanelSlot->SetPosition(CanvasPos);
    }

    Door.DoorImage = DoorIcon;
  }
}

void UHUDMinimap::UpdateRoomWidgetVisibility(const FRoomCached& Room) const
{
  const int32 Index = GetTextureIndexForDirtStage(Room.DirtStage);

  if (Room.CleanImage)
  {
    if (Room.RewardIconImage)
    {
      Room.RewardIconImage->SetVisibility(Index == 0 && Room.RoomShape != ERoomShape::Q1 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }
    Room.CleanImage->SetVisibility(Index == 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  }

  if (Room.MidImage)
  {
    Room.MidImage->SetVisibility(Index == 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  }

  if (Room.DirtyImage)
  {
    Room.DirtyImage->SetVisibility(Index == 2 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
  }
}

int32 UHUDMinimap::GetTextureIndexForDirtStage(EDirtStage Stage) const
{
  switch (Stage)
  {
  case EDirtStage::CLEAN:
    return 0;
  case EDirtStage::DIRTY:
  case EDirtStage::CLEANABLE:
    return 1;
  case EDirtStage::BUFFABLE:
  default:
    return 2;
  }
}
#pragma endregion