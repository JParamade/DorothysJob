#include "BaseCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "DorothysJob/Component/Camera/CameraFogOfWarComponent.h"
#include "DorothysJob/Component/Camera/CameraShakeHandlerComponent.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Data/DataAssets/PostProcessRooms.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "MainLegacyCameraShake.h"

ABaseCamera::ABaseCamera()
{
  PrimaryActorTick.bCanEverTick = true;

  // Create Camera Object, set as Root and set values:
  m_pCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
  if (IsValid(m_pCameraComponent))
  {
    RootComponent = m_pCameraComponent;

    // Set Camera component:
    m_pCameraComponent->SetUsingAbsoluteLocation(true);
    m_pCameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
    m_pCameraComponent->FieldOfView = 60.0f;
    FRotator vRelativeRotation(-45.0f, -45.0f, 0.0f);
    m_pCameraComponent->SetRelativeRotation(vRelativeRotation);
    m_pCameraComponent->SetRelativeLocation(m_vMainCameraPos);
  }

  // Init Resto of components:
  m_pFogOfWarComponent = CreateDefaultSubobject<UCameraFogOfWarComponent>(TEXT("FogOfWarComponent"));
  m_pShakeComponent = CreateDefaultSubobject<UCameraShakeHandlerComponent>(TEXT("ShakeHandlerComponent"));

  // Init variables:
  m_fMovementSpeed = 7.f;
  m_pTargetActor = nullptr;
}

void ABaseCamera::BeginPlay()
{
  Super::BeginPlay();

  //Set ortho witdh.
  m_pCameraComponent->SetOrthoWidth(m_fOrthoWidth);

  //Get and set Post Process Volume.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this)))
  {
    SetupPostProcessAndRooms(pGameInstance->m_lPPVAndRooms);
  }
}

void ABaseCamera::Tick(float _fDeltaTime)
{
  Super::Tick(_fDeltaTime);

  //Set the camera position:
  RecalculateCameraPosition(_fDeltaTime);

  /*---------------? WARNING !-----------------------------------------------------------------*/
  // THIS NEEDS TO BE MADE ONLY WHEN THE LEVEL LOAD ITS FINISH, TO SEE WHAT IS THE FIRST ROOM
  // Shoot a ray to the floor and detect the room and the volumes to take.
  // Only shoot a ray until it detects something, if it detects something, take room corners and hide objects.

  if (m_pPPVolumeA && m_pPPVolumeB)
  {
    if (BlendTimer < BlendDuration)
    {
      BlendTimer += _fDeltaTime;
      float Alpha = FMath::Clamp(BlendTimer / BlendDuration, 0.0f, 1.0f);

      if (m_bUsingA)
      {
        m_pPPVolumeA->BlendWeight = FMath::Lerp(1.0f, 0.0f, Alpha);
        m_pPPVolumeB->BlendWeight = FMath::Lerp(0.0f, 1.0f, Alpha);
      }
      else
      {
        m_pPPVolumeB->BlendWeight = FMath::Lerp(1.0f, 0.0f, Alpha);
        m_pPPVolumeA->BlendWeight = FMath::Lerp(0.0f, 1.0f, Alpha);
      }
      if (Alpha >= 1.0f)
      {
        m_bUsingA = !m_bUsingA;
      }
    }
  }
}

void ABaseCamera::StartBlend(const FPostProcessSettings& NewSettings, float Duration)
{
  BlendDuration = Duration;
  BlendTimer = 0.0f;

  if (m_bUsingA)
  {
    m_pPPVolumeB->Settings = NewSettings;
    m_pPPVolumeB->BlendWeight = 0.0f;
  }
  else
  {
    m_pPPVolumeA->Settings = NewSettings;
    m_pPPVolumeA->BlendWeight = 0.0f;
  }
}

void ABaseCamera::RecalculateCameraPosition(float _fDeltaTime)
{
  if (!IsValid(m_pTargetActor)) return;

  // Get the target position:
  m_vTargetPosition = m_pTargetActor->GetActorLocation();

  m_vCameraNewPos = m_vTargetPosition + m_vMainCameraPos;

  //Update camera position:
  UpdateCameraPosition(_fDeltaTime);
}

void ABaseCamera::UpdateCameraPosition(float _fDeltaTime)
{
  FVector CurrentPosition = m_pCameraComponent->GetRelativeLocation();

  // Get the new pos with the targets:
  FVector SafeTargetPosition = m_vCameraNewPos;

  //Set the new pos with interpolation for a smooth movement:
  FVector FinalPosition = FMath::VInterpTo(CurrentPosition, SafeTargetPosition, _fDeltaTime, m_fMovementSpeed);

  FVector ShakeOffset = FVector::ZeroVector;

  UMainLegacyCameraShake* ShakeInstance = m_pShakeComponent->GetShakeIstance();
  if (ShakeInstance && m_bActiveCameraShake)
  {
    // Si ha cambiado el tipo de shake, reiniciamos el tiempo
    if (ShakeInstance->GetClass() != m_LastShakeClass)
    {
      m_fRunningShakeTime = 0.f;
      m_LastShakeClass = ShakeInstance->GetClass();
    }

    m_fRunningShakeTime += _fDeltaTime;

    const FVOscillator& LocOsc = ShakeInstance->LocOscillation;
    float BlendInTime = ShakeInstance->OscillationBlendInTime;

    // Factor de interpolación (0 ? 1)
    float BlendAlpha = (BlendInTime > 0.f)
      ? FMath::Clamp(m_fRunningShakeTime / BlendInTime, 0.f, 1.f)
      : 1.f;

    // Lambda para calcular cada eje
    auto CalcAxis = [&](const FFOscillator& AxisOsc) -> float
      {
        if (AxisOsc.Amplitude == 0.f) return 0.f;
        return (AxisOsc.Amplitude * BlendAlpha) *
          FMath::Sin(2 * PI * AxisOsc.Frequency * m_fRunningShakeTime + AxisOsc.InitialOffset);
      };

    ShakeOffset.X = CalcAxis(LocOsc.X);
    ShakeOffset.Y = CalcAxis(LocOsc.Y);
    ShakeOffset.Z = CalcAxis(LocOsc.Z);

    float MaxDuration = ShakeInstance->OscillationDuration;
    if (MaxDuration > 0.f && m_fRunningShakeTime >= MaxDuration)
    {
      // Si ya pasó la duración, detenemos o reseteamos
      m_bActiveCameraShake = false; // o pon un fade-out si quieres
      ShakeOffset = FVector::ZeroVector;
      m_fRunningShakeTime = 0.f;
      m_LastShakeClass = ShakeInstance->GetClass();
    }
  }
  // Aplicar a la cámara
  m_pCameraComponent->SetRelativeLocation(FinalPosition + ShakeOffset);
}

void ABaseCamera::HandleRoomChanged(int32 _iPrevRoomId, int32 _iCurrentRoomId)
{
  // Everytime the room change, set the fog of war:
  m_pFogOfWarComponent->RoomChange(_iPrevRoomId, _iCurrentRoomId);
}

void ABaseCamera::FindGlobalPPV(UWorld* World)
{
  if (!IsValid(World)) return;

  FString CurrentMapName = World->GetMapName();
  CurrentMapName.RemoveFromStart(World->StreamingLevelsPrefix);
  if (CurrentMapName != TEXT("L_Procedural"))
  {
    return;
  }

  ULevel* PersistentLevel = World->PersistentLevel;
  if (!IsValid(PersistentLevel))
  {
    return;
  }
  TArray<AActor*> FoundVolumes;
  UGameplayStatics::GetAllActorsWithTag(World, FName("PPV"), FoundVolumes);
  for (AActor* Actor : FoundVolumes)
  {
    if (!IsValid(Actor)) continue;
    APostProcessVolume* PPV = Cast<APostProcessVolume>(Actor);
    if (!IsValid(PPV)) continue;

    if (IsValid(Actor->GetLevel()) && Actor->GetLevel() == PersistentLevel)
    {
      if (!m_pPPVolumeA)
      {
        m_pPPVolumeA = Cast<APostProcessVolume>(Actor);
      }
      else if (!m_pPPVolumeB)
      {
        m_pPPVolumeB = Cast<APostProcessVolume>(Actor);
      }
    }
    else
    {
      PPV->bUnbound = false;
      PPV->BlendWeight = 0.0f;
      PPV->Priority = -1.0f;
    }
  }

  if (m_pPPVolumeA)
    m_pPPVolumeA->BlendWeight = 1.0f;

  if (m_pPPVolumeB)
    m_pPPVolumeB->BlendWeight = 0.0f;
}

void ABaseCamera::SetupPostProcessVolume(const UBaseRoom* Room)
{
  if (!IsValid(m_pPPVolumeA) || !IsValid(m_pPPVolumeB) || !IsValid(Room))
  {
    return;
  }

  for (const FSettingsForRooms& CachedSettings : m_lCachedSettings)
  {
    if (Room->GetLevelId() == CachedSettings.ID)
    {
      FPostProcessSettings TargetSettings;

      if (Room->GetDirtStage() != EDirtStage::BUFFABLE)
      {
        TargetSettings = CachedSettings.Settings;
      }
      else
      {
        TargetSettings = CachedSettings.DirtSettings;
      }

      TargetSettings.WeightedBlendables.Array.Empty();
      TargetSettings.WeightedBlendables.Array.Add(FWeightedBlendable(1.f, m_pFogOfWarComponent->GetPostProcessMID()));
      StartBlend(TargetSettings, 0.5);
      break;
    }
  }
}

#pragma region Getters&Setters
void ABaseCamera::SetupPostProcessAndRooms(TArray<TObjectPtr<const UPostProcessRooms>> _lPPVAndRooms)
{
  if (UWorld* CachedWorld = GetWorld())
  {
    FindGlobalPPV(CachedWorld);

    if (ULevelManager* MyLevelManager = CachedWorld->GetSubsystem<ULevelManager>())
    {
      MyLevelManager->OnExitedRoom.AddDynamic(this, &ABaseCamera::HandleExitRoom);
      MyLevelManager->OnEnteredRoom.AddDynamic(this, &ABaseCamera::HandleEnterRoom);

      const TArray<TObjectPtr<UBaseRoom>>& RoomList = MyLevelManager->GetRooms();
      m_lCachedSettings.Reserve(RoomList.Num());

      for (const TObjectPtr<UBaseRoom>& BR : RoomList)
      {
        if (!IsValid(BR))
        {
          continue;
        }

        const ULevel* Level = BR->GetLevel();

        if (!IsValid(Level))
        {
          continue;
        }

        const FString RuntimePackage = Level->GetOutermost()->GetName();
        const FString RuntimeShort = FPackageName::GetShortName(RuntimePackage);

        for (const UPostProcessRooms* PPVRoom : _lPPVAndRooms)
        {
          if (!IsValid(PPVRoom))
          {
            continue;
          }
          const FString AssetPath = PPVRoom->Level.ToSoftObjectPath().GetLongPackageName();
          const FString AssetShort = FPackageName::GetShortName(AssetPath);
          if (RuntimeShort.Contains(AssetShort))
          {
            m_lCachedSettings.Add({ BR->GetLevelId(), PPVRoom->m_fSettings, PPVRoom->m_fDirtSettings });
          }
        }
      }
    }
  }
}

AActor* ABaseCamera::GetTargetActor()
{
  return m_pTargetActor;
}

void ABaseCamera::SetTargetActor(APawn* _TargetActor)
{
  m_pTargetActor = _TargetActor;
}

UCameraComponent* ABaseCamera::GetCameraComponent()
{
  return m_pCameraComponent;
}

UCameraFogOfWarComponent* ABaseCamera::GetFogOfWarComponent()
{
  return m_pFogOfWarComponent;
}

void ABaseCamera::PlayShakeComponent()
{
  if (IsValid(m_pShakeComponent))
  {
    m_bActiveCameraShake = true;
    m_pShakeComponent->PlayCameraShake();
  }
}

void ABaseCamera::StopShakeComponent()
{
  if (IsValid(m_pShakeComponent))
  {
    m_bActiveCameraShake = false;
    m_pShakeComponent->StopCameraShake();
  }

  // Reiniciar tiempo para que el blend in vuelva a empezar la próxima vez
  m_fRunningShakeTime = 0.f;
  m_LastShakeClass = nullptr;
}

void ABaseCamera::SetShakeComponent(TSubclassOf<UMainLegacyCameraShake> _ShakeClass)
{
  if (IsValid(m_pShakeComponent))
  {
    m_pShakeComponent->SetCameraShake(_ShakeClass);
  }
}

void ABaseCamera::HandleEnterRoom(UBaseRoom* EnteredRoom)
{
  if (!IsValid(EnteredRoom))
  {
    return;
  }

  if (!bFoWInit && IsValid(m_pFogOfWarComponent))
  {
    bFoWInit = true;
    m_pFogOfWarComponent->PPVolume = m_pPPVolumeA;
    m_pFogOfWarComponent->InitPPMID(EnteredRoom->GetStencilId());
  }

  EnteredRoom->OnEnemiesBuff.AddUObject(this, &ABaseCamera::HandleEnterRoom);
  EnteredRoom->OnEnemiesDebuff.AddUObject(this, &ABaseCamera::HandleEnterRoom);

  SetupPostProcessVolume(EnteredRoom);

  // Everytime the room change, set the fog of war:
  m_pFogOfWarComponent->RoomChange(PrevLevelID, EnteredRoom->GetStencilId());
}

void ABaseCamera::HandleExitRoom(UBaseRoom* ExitedRoom)
{
  if (ExitedRoom)
  {
    ExitedRoom->OnEnemiesBuff.RemoveAll(this);
    ExitedRoom->OnEnemiesDebuff.RemoveAll(this);
    PrevLevelID = ExitedRoom->GetStencilId();
  }
  else
  {
    PrevLevelID = -1;
  }
}
#pragma endregion
