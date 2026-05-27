#include "NozzleComponent.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/WasheeFlashee/WasheeFlashee.h"
#include "DorothysJob/Actor/CleaningInParts.h"
#include "DorothysJob/Data/DataAssets/Weapon/WFWeaponStatsDataAsset.h"
#include "NiagaraFunctionLibrary.h"

UNozzleComponent::UNozzleComponent() :
  m_iCurrentCleanTexture(0)
{
  PrimaryComponentTick.bCanEverTick = false;
}

void UNozzleComponent::Use()
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset) || !IsValid(m_pOwnerPlayer->GetMuzzle()) || !IsValid(m_pDrawing) || !IsValid(CachedWorld))
  {
    return;
  }

  FVector MuzzlePos = m_pOwnerPlayer->GetMuzzle()->GetComponentLocation();
  float CleaningPercent = m_pDrawing->GetNotCleaningPercentage();
  float Angle = m_pOwnerPlayer->GetLookAtRotation().Yaw + m_pDataAsset->m_fCleaningAngle;

  ACleaningInParts* CleaningActor = CachedWorld->SpawnActor<ACleaningInParts>(MuzzlePos, FRotator::ZeroRotator);

  if (IsValid(CleaningActor))
  {
    CleaningActor->Init(
      m_pDataAsset->m_oCleanTextures,
      m_pDataAsset->m_fDuration,
      Angle,
      m_pDataAsset->m_v2SCleaningSize,
      m_pDataAsset->m_v2SCleaningPivot
    );
  }

  if (IsValid(m_pDataAsset->m_pCleanVFX) && IsValid(m_pOwnerPlayer->GetSkeletalMesh()))
  {
    FVector Forward = m_pOwnerPlayer->GetSkeletalMesh()->GetForwardVector();
    FVector Right = m_pOwnerPlayer->GetSkeletalMesh()->GetRightVector();
    FVector Up = m_pOwnerPlayer->GetSkeletalMesh()->GetUpVector();

    FVector LocalOffset = m_pDataAsset->m_vCleaningVFXOffset;

    FVector WorldOffset = Forward * LocalOffset.X
      + Right * LocalOffset.Y
      + Up * LocalOffset.Z;

    FVector SpawnLocation = MuzzlePos + WorldOffset;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
      CachedWorld,
      m_pDataAsset->m_pCleanVFX,
      SpawnLocation,
      Forward.Rotation()
    );
  }

  if (m_pOwnerWeapon.IsValid())
  {
    m_pOwnerWeapon->NotifyCleaningLiquid(CleaningPercent);
  }

  OnCleanAction.Broadcast(MuzzlePos + m_pOwnerPlayer->GetMuzzle()->GetForwardVector() * 50.f, m_pOwnerPlayer->GetLookAtRotation());
}

void UNozzleComponent::CleanSprite()
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset) || !IsValid(m_pOwnerPlayer->GetMuzzle()) || !IsValid(m_pDrawing) || !IsValid(CachedWorld))
  {
    return;
  }

  if (m_iCurrentCleanTexture >= m_pDataAsset->m_oCleanTextures.Num())
  {
    m_iCurrentCleanTexture = 0;
    return;
  }

  FVector MuzzlePos = m_pOwnerPlayer->GetMuzzle()->GetComponentLocation();
  float Angle = m_pOwnerPlayer->GetLookAtRotation().Yaw + m_pDataAsset->m_fCleaningAngle;

  if (m_pDataAsset->m_oCleanTextures.IsValidIndex(m_iCurrentCleanTexture) && IsValid(m_pDataAsset->m_oCleanTextures[m_iCurrentCleanTexture]))
  {
    m_pDrawing->Clean(
      m_pDataAsset->m_oCleanTextures[m_iCurrentCleanTexture],
      m_pDataAsset->m_v2SCleaningSize,
      Angle,
      m_pDataAsset->m_v2SCleaningPivot,
      MuzzlePos
    );
  }

  m_iCurrentCleanTexture++;

  float Interval = m_pDataAsset->m_fDuration / static_cast<float>(m_pDataAsset->m_oCleanTextures.Num());
  CachedWorld->GetTimerManager().SetTimer(
    m_CleanTimerHandle, this, &UNozzleComponent::CleanSprite, Interval, false
  );
}

void UNozzleComponent::SetDrawingComp(UDrawing* _comp)
{
  m_pDrawing = _comp;
}

void UNozzleComponent::BeginPlay()
{
  Super::BeginPlay();

  if (UWorld* pCachedWorld = GetWorld())
  {
    CachedWorld = pCachedWorld;
  }

  if (AWasheeFlashee* WasheeFlashee = Cast<AWasheeFlashee>(GetOwner()))
  {
    m_pOwnerWeapon = WasheeFlashee;

    if (ABasePlayer* Player = Cast<ABasePlayer>(m_pOwnerWeapon->GetOwner()))
    {
      m_pOwnerPlayer = Player;
    }

    if (m_pOwnerWeapon->GetDataAsset())
    {
      m_pDataAsset = Cast<UWFWeaponStatsDataAsset>(m_pOwnerWeapon->GetDataAsset());
    }
  }
}
