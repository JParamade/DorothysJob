
#include "CameraFogOfWarComponent.h"
#include "EngineUtils.h"   
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Actor/Camera/BaseCamera.h"
#include "CameraCollisionHandler.h"

// Sets default values for this component's properties
UCameraFogOfWarComponent::UCameraFogOfWarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UCameraFogOfWarComponent::BeginPlay()
{
  Super::BeginPlay();

  AActor* Owner = GetOwner();
  if (!Owner) return;

  m_pCameraOwner = Cast<ABaseCamera>(Owner);
  if (!m_pCameraOwner) return;

  // TEXTURE -------------------------------
  InitializeTexture();
}


void UCameraFogOfWarComponent::InitPPMID(int32 _iCurrentRoomId)
{
  if (PPVolume && PPVolume->Settings.WeightedBlendables.Array.Num() > 0)
  {
    for (FWeightedBlendable& Blendable : PPVolume->Settings.WeightedBlendables.Array)
    {
      PostProcessMaterial = Cast<UMaterialInterface>(Blendable.Object);
    }
  }

  //Set Post Process Material to dynamic and set that material to teh Post Process Volume
  if (PostProcessMaterial)
  {
    PostProcessMID = UMaterialInstanceDynamic::Create(PostProcessMaterial, this);
    if (PostProcessMID)
    {
      SetRoomState(_iCurrentRoomId, FColor(255, 255, 255));
      PostProcessMID->SetScalarParameterValue("Param", 1);
      PostProcessMID->SetScalarParameterValue("Param2", _iCurrentRoomId);
      PostProcessMID->SetScalarParameterValue("TransitionAlpha", 1);
      PostProcessMID->SetTextureParameterValue("ParamTexture", DynamicTexture);
    }

    if (PPVolume)
    {
      PPVolume->Settings.WeightedBlendables.Array.Empty();
      PPVolume->Settings.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, PostProcessMID));
    }
  }
}

void UCameraFogOfWarComponent::RoomChange(int32 _iPrevRoomId, int32 _iCurrentRoomId)
{
  // Set previous room as visited
  SetRoomState(_iPrevRoomId, FColor(128, 0, 0)); // Half red, represents grey.

  // Set current room as active
  SetRoomState(_iCurrentRoomId, FColor(255, 255, 255)); // Red, represents white.

  if (PostProcessMID)
  {
    // Set the new texture in the material.
    PostProcessMID->SetTextureParameterValue("ParamTexture", DynamicTexture);
  }
}

void UCameraFogOfWarComponent::ActivateOrDesactivateFogOfWar(bool _bActivation)
{
  if (IsValid(PPVolume))
  {
    PPVolume->bUnbound = !_bActivation;
  }
}

#pragma region Texture Function
void UCameraFogOfWarComponent::InitializeTexture()
{
  TextureWidth = 100;
  TextureHeight = 1;

  // Number of pixels in the texture, should be the number of rooms.
  int32 TotalPixels = TextureWidth * TextureHeight;

  // Each pixel has 4 bytes: R, G, B, and A (8 bits per channel).
  // Calculate total bytes needed to store all pixels.
  TextureDataSize = TotalPixels * 4;

  // This is the width of one row in bytes (each pixel takes 4 bytes).
  // Useful if you want to manipulate the texture as a linear "image" in memory.
  // This value can be used later for row navigation.
  TextureDataSqrtSize = TextureWidth * 4;

  // Create a dynamic array of bytes (uint8) that will hold all color values of the texture (initially empty).
  // This is where you will manually write the color of each pixel before uploading it to the GPU.
  TextureData = new uint8[TextureDataSize];

  for (int32 i = 0; i < TotalPixels; ++i)
  {
    TextureData[i * 4 + 0] = 0;   // R
    TextureData[i * 4 + 1] = 0;   // G
    TextureData[i * 4 + 2] = 0;   // B
    TextureData[i * 4 + 3] = 255; // A
  }

  // Create a transient dynamic texture at runtime.
  // PF_R8G8B8A8 means 4 channels of 8 bits (RGBA).
  // "DynamicTexture" is the internal name.
  DynamicTexture = UTexture2D::CreateTransient(TextureWidth, TextureHeight, EPixelFormat::PF_R8G8B8A8, "DynamicTexture");

  // Disable color compression to avoid artifacts.
  // This ensures pixel values are exact.
  DynamicTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;

  // Disable gamma correction.
  // Useful when doing logical calculations or controlling exact values (like a mask or visibility in your case).
  DynamicTexture->SRGB = false;

  // Use nearest filtering (no smoothing between pixels).
  // This makes each pixel clearly distinguishable without interpolation.
  DynamicTexture->Filter = TextureFilter::TF_Nearest;

  // Prevent Unreal's Garbage Collector from destroying the texture.
  // Very important for runtime-created objects.
  DynamicTexture->AddToRoot();

  // Upload the texture to the GPU.
  // Necessary after configuring it so it becomes visually available or usable in materials.
  DynamicTexture->UpdateResource();

  // Define the entire region of the texture that you can update later.
  // Used with UpdateTextureRegions() to change texture content during gameplay
  // (like painting or showing explored rooms).
  TextureRegion = new FUpdateTextureRegion2D(0, 0, 0, 0, TextureWidth, TextureHeight);
}

void UCameraFogOfWarComponent::SetRoomState(int32 RoomIndex, FColor Color)
{
  if (!TextureData || RoomIndex < 0 || RoomIndex >= TextureWidth)
    return;

  int32 Index = RoomIndex * 4;

  TextureData[Index + 0] = Color.R;
  TextureData[Index + 1] = Color.G;
  TextureData[Index + 2] = Color.B;
  TextureData[Index + 3] = Color.A;

  // Update GPU:
  UpdateTexture();
}

void UCameraFogOfWarComponent::UpdateTexture()
{
  if (!DynamicTexture || !TextureRegion)
    return;

  DynamicTexture->UpdateTextureRegions(
    0,                                     // MipIndex
    1,                                     // NumRegions
    TextureRegion,                         // Region
    TextureDataSqrtSize,                   // SrcPitch
    4,                                     // SrcBpp
    TextureData                            // SrcData
  );
}
#pragma endregion

#pragma region Getters&Setters
UMaterialInstanceDynamic* UCameraFogOfWarComponent::GetPostProcessMID() const
{
  return PostProcessMID;
}
#pragma endregion
