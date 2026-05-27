// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseTile.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "DirtAnalysis/Public/CalculateDirtnessShader/CalculateDirtnessShader.h"
#include "DorothysJob/Manager/ProceduralGeneration.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "NiagaraFunctionLibrary.h"

ABaseTile::ABaseTile()
{
  if (!HasAnyFlags(RF_ClassDefaultObject))
    UProceduralGeneration::OnLevelReady.AddUObject(this, &ABaseTile::TryRegisterInRoom);
}

void ABaseTile::BeginPlay()
{
  TArray<UStaticMeshComponent*> StaticArray;
  GetComponents<UStaticMeshComponent*>(StaticArray);

  for (size_t i = 0; i < StaticArray.Num(); i++)
  {
    m_staticMesh = StaticArray[i];
  }
}

bool ABaseTile::InitTile()
{
  m_pRenderTargetTier = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), m_vSize.X, m_vSize.Y, RTF_RGBA16f, FLinearColor(0, 0, 0, 0));
  m_pRenderTargetColor = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), m_vSize.X, m_vSize.Y, RTF_RGBA16f, FLinearColor(0, 0, 0, 0));
  m_pRenderTargetDust = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), m_vSize.X, m_vSize.Y, RTF_RGBA16f, FLinearColor(0, 0, 0, 0));
  if (m_bBeginDirty)
  {
    UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), m_pRenderTargetColor, FLinearColor(1, 0, 0, 0));
    UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), m_pRenderTargetTier, FLinearColor(1, 0, 0, 0));
  }

  if (m_baseMaterial != nullptr)
  {
    m_materialInstance = UMaterialInstanceDynamic::Create(m_baseMaterial, this);

    if (m_staticMesh != nullptr)
    {
      m_staticMesh->SetMaterial(0, m_materialInstance);
    }
    UpdateSelf();
  }

  return true;
}

bool ABaseTile::Draw(UTexture* _texture, UTexture* _textureCleaning, FVector2D _UVPosition, FVector2D _size, float _rotation, FVector2D _pivot, EDrawColor _color, EDrawTier _tierType, bool _isCleaning)
{
  if (!IsValid(m_pRenderTargetTier))
  {
    UE_LOG(LogTemp, Error, TEXT("Render Texture is nullptr, could be an error or the render texture is not inicialize"));
    return false;
  }
  if (!IsValid(m_pRenderTargetColor))
  {
    UE_LOG(LogTemp, Error, TEXT("Render Texture is nullptr, could be an error or the render texture is not inicialize"));
    return false;
  }
  UCanvas* canvas;
  FDrawToRenderTargetContext context;
  FVector2D UVcoordinades;
  FVector2D UVSize;

  UVcoordinades.X = 0;
  UVcoordinades.Y = 0;

  UVSize.X = 1;
  UVSize.Y = 1;

  FLinearColor DrawColor(0, 0, 0, 0);

  FVector2D size;
  FVector2D drawLoc;

  _rotation = _rotation - GetActorRotation().Yaw;


  if (_tierType == EDrawTier::Liquid)
  {
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), m_pRenderTargetTier, canvas, size, context); //Drawing on the Tier Texture
    drawLoc = (size * _UVPosition) - (_size / 2);
    canvas->K2_DrawTexture(_textureCleaning, drawLoc, _size, UVcoordinades, UVSize, FLinearColor(1, 1, 1, 1), BLEND_Modulate, _rotation, _pivot);
    if (!_isCleaning)
    {
      DrawColor = FLinearColor(1, 0, 0, 0);
      canvas->K2_DrawTexture(_texture, drawLoc, _size, UVcoordinades, UVSize, DrawColor, BLEND_Additive, _rotation, _pivot);
    }
    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), context);
  }
  else
  {
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), m_pRenderTargetDust, canvas, size, context); //Drawing on the Tier Texture
    drawLoc = (size * _UVPosition) - (_size / 2);

    canvas->K2_DrawTexture(_textureCleaning, drawLoc, _size, UVcoordinades, UVSize, FLinearColor(1, 1, 1, 1), BLEND_Modulate, _rotation, _pivot);

    if (!_isCleaning)
    {
      if (_tierType == EDrawTier::Dust1)
      {
        DrawColor = FLinearColor(1, 0, 0, 0);
      }
      else if (_tierType == EDrawTier::Dust2)
      {
        DrawColor = FLinearColor(0, 1, 0, 0);
      }
      canvas->K2_DrawTexture(_texture, drawLoc, _size, UVcoordinades, UVSize, DrawColor, BLEND_Additive, _rotation, _pivot);
    }

    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), context);
  }

  UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(GetWorld(), m_pRenderTargetColor, canvas, size, context);
  drawLoc = (size * _UVPosition) - (_size / 2);

  canvas->K2_DrawTexture(_textureCleaning, drawLoc, _size, UVcoordinades, UVSize, FLinearColor(1, 1, 1, 1), BLEND_Modulate, _rotation, _pivot);
  if (!_isCleaning)
  {
    DrawColor = GetDrawColor(_color);
    canvas->K2_DrawTexture(_texture, drawLoc, _size, UVcoordinades, UVSize, DrawColor, BLEND_Additive, _rotation, _pivot);
  }

  UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(GetWorld(), context);

  UpdateSelf();

  return true;
}

void ABaseTile::UpdateMaterial(UMaterialInstanceDynamic* _materialInstace)
{
  if (!IsValid(m_materialInstance))
  {
    UE_LOG(LogTemp, Error, TEXT("Material Instance is nullptr"));
    return;
  }
  if (!IsValid(m_pRenderTargetTier))
  {
    UE_LOG(LogTemp, Error, TEXT("Render Texture is nullptr, could be an error or the render texture is not inicialize"));
    return;
  }
  //GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, TEXT("Actualizando"));
  m_materialInstance->SetTextureParameterValue(m_nMaskParamTier, m_pRenderTargetTier);
  m_materialInstance->SetTextureParameterValue(m_nMaskParamColor, m_pRenderTargetColor);
  m_materialInstance->SetTextureParameterValue(m_nMaskParamDust, m_pRenderTargetDust);
}

void ABaseTile::UpdateSelf()
{
  UpdateMaterial(m_materialInstance);
  UpdateCleaningPercentage();
}


void ABaseTile::UpdateCleaningPercentage()
{
  if (!IsValid(m_pRenderTargetTier))
  {
    return;
  }

  TWeakObjectPtr<UTextureRenderTarget2D> WeakRenderTarget(m_pRenderTargetColor);
  FCalculateDirtnessShaderDispatchParams Params(m_pRenderTargetColor->SizeX, m_pRenderTargetColor->SizeY, 1);
  Params.LiquidRenderTarget = m_pRenderTargetColor->GameThread_GetRenderTargetResource();
  Params.DustRenderTarget = m_pRenderTargetDust->GameThread_GetRenderTargetResource();

  FCalculateDirtnessShaderInterface::Dispatch(
    Params,
    [this, WeakRenderTarget](int PixelCount)
    {
      if (!WeakRenderTarget.IsValid() || IsEngineExitRequested())
      {
        return;
      }
      const int TotalPixels = WeakRenderTarget->SizeX * WeakRenderTarget->SizeY;
      m_fNotCleanPercetage = TotalPixels > 0
        ? (float(PixelCount) / float(TotalPixels))
        : 0.0f;
    }
  );

  // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Porcentage %.2f"), m_fNotCleanPercetage));
}


ETileType ABaseTile::GetType() const
{
  return m_eType;
}

UMaterialInstanceDynamic* ABaseTile::GetMaterialInstance() const
{
  return m_materialInstance;
}

UTextureRenderTarget2D* ABaseTile::GetRenderTexture2D() const
{
  return m_pRenderTargetTier;
}

float ABaseTile::GetPercetage() const
{
  return m_fNotCleanPercetage;
}

FName ABaseTile::GetParamName() const
{
  return m_nMaskParamTier;
}

UStaticMeshComponent* ABaseTile::GetStaticMesh() const
{
  return m_staticMesh;
}

void ABaseTile::SetStaticMesh(UStaticMeshComponent* _staticMeshComponent)
{
  m_staticMesh = _staticMeshComponent;
}

void ABaseTile::BeginDestroy()
{
  Super::BeginDestroy();
  if (IsValid(m_pRenderTargetTier))
  {
    UKismetRenderingLibrary::ReleaseRenderTarget2D(m_pRenderTargetTier);
    m_pRenderTargetTier = nullptr;
  }
  if (IsValid(m_pRenderTargetColor))
  {
    UKismetRenderingLibrary::ReleaseRenderTarget2D(m_pRenderTargetColor);
    m_pRenderTargetColor = nullptr;
  }
  if (IsValid(m_pRenderTargetDust))
  {
    UKismetRenderingLibrary::ReleaseRenderTarget2D(m_pRenderTargetDust);
    m_pRenderTargetDust = nullptr;
  }
}

void ABaseTile::CleanFloor(bool _bCanShowVFX)
{
  UWorld* CachedWorld = GetWorld();

  if (!CachedWorld)
  {
    return;
  }

  // I don't know if this is enough has to be checked
  UKismetRenderingLibrary::ClearRenderTarget2D(CachedWorld, m_pRenderTargetTier, FLinearColor(0, 0, 0, 0));
  UKismetRenderingLibrary::ClearRenderTarget2D(CachedWorld, m_pRenderTargetColor, FLinearColor(0, 0, 0, 0));
  UKismetRenderingLibrary::ClearRenderTarget2D(CachedWorld, m_pRenderTargetDust, FLinearColor(0, 0, 0, 0));

  m_pRenderTargetTier = nullptr;
  m_pRenderTargetColor = nullptr;
  m_pRenderTargetDust = nullptr;

  if (_bCanShowVFX)
  {
    if (m_pAutoCleanVFX)
    {
      UNiagaraFunctionLibrary::SpawnSystemAtLocation(CachedWorld, m_pAutoCleanVFX, GetActorLocation(), FRotator::ZeroRotator);
    }
    if (m_staticMesh && m_pOverlayMaterial)
    {
      m_staticMesh->SetOverlayMaterial(m_pOverlayMaterial);
      CachedWorld->GetTimerManager().SetTimer(
        OverlayTimerHandle,
        this,
        &ABaseTile::ClearOverlayMaterial,
        m_fClearingOverlayMaterialCooldown,
        false
      );
    }
  }
}

void ABaseTile::SetNegateDirt(bool _bNegateDirt)
{
  if (m_materialInstance)
  {
    m_materialInstance->SetVectorParameterValue(FName("Negate Dirt"), (_bNegateDirt ? FVector(0.0f, 0.0f, 0.0f) : FVector(1.0f, 1.0f, 1.0f)));
  }
}

void ABaseTile::TryRegisterInRoom(UBaseRoom* Room)
{
  if (!IsValid(Room))
  {
    return;
  }

  const ULevel* MyLevel = GetLevel();
  const ULevel* RoomLevel = Room->GetLevel();

  FVector MyLevelLocation = GetLevelTransform().GetLocation();
  FVector RoomLevelLocation = Room->GetLocation();

  if (IsValid(RoomLevel) && IsValid(MyLevel) && RoomLevel == MyLevel)
  {
    UProceduralGeneration::OnLevelReady.RemoveAll(this);
    Room->RegisterTile(this);
  }
}

FLinearColor ABaseTile::GetDrawColor(EDrawColor _color)
{
  FLinearColor oColor = FLinearColor(0, 0, 0, 0);

  switch (_color)
  {
  case EDrawColor::EBase:

    oColor = FLinearColor(1, 0, 0, 0);
    break;
  case EDrawColor::EOrange:

    oColor = FLinearColor(0, 1, 0, 0);
    break;
  case EDrawColor::ERed:

    oColor = FLinearColor(0, 0, 1, 0);
    break;
  case EDrawColor::EBlack:

    oColor = FLinearColor(0, 0, 0, 1);
    break;
  default:

    oColor = FLinearColor(0, 0, 0, 0);
    break;
  }

  return oColor;
}

void ABaseTile::ClearOverlayMaterial()
{
  if (m_staticMesh)
  {
    m_staticMesh->SetOverlayMaterial(nullptr);
  }
  if (m_pAutoCleanLoopVFX)
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pAutoCleanLoopVFX, GetActorLocation(), FRotator::ZeroRotator);
  }
}
