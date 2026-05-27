#include "WasheeFlashee.h"
#include "DorothysJob/Component/Weapon/WasheeFlashee/LiquidComponent.h"
#include "DorothysJob/Component/Weapon/WasheeFlashee/NozzleComponent.h"
#include "DorothysJob/Component/Weapon/WasheeFlashee/SpecialProjectileComponent.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Data/DataAssets/Weapon/WFWeaponStatsDataAsset.h"
#include "DorothysJob/Component/Weapon/BaseWeaponAudioComponent.h"
#include "FMODAudioComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AWasheeFlashee::AWasheeFlashee()
{
  PrimaryActorTick.bCanEverTick = false;

  // Secondary Mesh component
  m_pSecondaryWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Secondary Mesh"));
  m_pSecondaryWeaponMesh->SetupAttachment(m_pWeaponMesh);

  // Components
  m_pWasheeNozzle = CreateDefaultSubobject<UNozzleComponent>(TEXT("Washee's Nozzle Component"));
  m_pFlasheeLiquid = CreateDefaultSubobject<ULiquidComponent>(TEXT("Flashee's Liquid Component"));
  m_pSpecialProjectile = CreateDefaultSubobject<USpecialProjectileComponent>(TEXT("Special Projectile Component"));
  m_pWeaponAudioComponent = CreateDefaultSubobject<UBaseWeaponAudioComponent>(TEXT("Weapon audio component"));
  m_pDrawing = CreateDefaultSubobject<UDrawing>(TEXT("Drawing component"));

  m_pWasheeNozzle->SetDrawingComp(m_pDrawing);
}

#pragma region | Getters
TObjectPtr<UWFWeaponStatsDataAsset> AWasheeFlashee::GetDataAsset()
{
  if (!m_pWFDataAsset)
  {
    m_pWFDataAsset = Cast<UWFWeaponStatsDataAsset>(m_pWeaponDataAsset);
    m_fCosHalfAngle = FMath::Cos(FMath::DegreesToRadians(m_pWFDataAsset->m_fConeAngle * 0.5f));
  }
  return m_pWFDataAsset;
}
#pragma endregion

#pragma region | Actions
void AWasheeFlashee::Attack()
{
  if (!IsValid(m_pFlasheeLiquid))
  {
    return;
  }

  m_pFlasheeLiquid->Use();
  //if (IsValid(m_pAudioComponent)) {
  //    m_pAudioComponent->SetEvent(m_pAttackEvent);
  //    m_pAudioComponent->Play();
  //}
}

void AWasheeFlashee::Clean()
{
  if (!IsValid(m_pWasheeNozzle))
  {
    return;
  }

  m_pWasheeNozzle->Use();
  OnCleanStart.Broadcast();

  //if (IsValid(m_pAudioComponent)) 
  //{
  //  m_pAudioComponent->SetEvent(m_pCleanEvent);
  //  m_pAudioComponent->Play();
  //}

  ConeStatusApplication();
}

void AWasheeFlashee::SpecialAttack()
{
  if (!IsValid(m_pSpecialProjectile))
  {
    return;
  }
  m_pSpecialProjectile->Use();
  OnSpecialStart.Broadcast();

  //if (IsValid(m_pAudioComponent)) {
  //    m_pAudioComponent->SetEvent(m_pSpecialEvent);
  //    m_pAudioComponent->Play();
  //}
}
#pragma endregion

#pragma region | Helpers
void AWasheeFlashee::ConeStatusApplication()
{
  const ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
  if (!IsValid(OwnerCharacter)) 
  {
    return;
  }

  GetDataAsset();

  const FVector StartLocation = OwnerCharacter->GetActorLocation();
  const FVector Forward = OwnerCharacter->GetSkeletalMesh()->GetForwardVector();

  TArray<AActor*> OverlappedActors;
  UKismetSystemLibrary::SphereOverlapActors(
    GetWorld(),
    StartLocation,
    m_pWFDataAsset->m_fConeDistance,
    { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1) },
    ABaseEnemy::StaticClass(),
    {},
    OverlappedActors
  );

  for (AActor* Actor : OverlappedActors)
  {
    ABaseCharacter* EnemyCharacter = Cast<ABaseCharacter>(Actor);
    if (!IsValid(EnemyCharacter) || EnemyCharacter == OwnerCharacter)
    {
      continue;
    }

    const FVector Direction = (Actor->GetActorLocation() - StartLocation).GetSafeNormal();
    const float DotProduct = FVector::DotProduct(Forward, Direction);

    if (DotProduct >= m_fCosHalfAngle)
    {
      ApplyCleanStatusEffects(EnemyCharacter);
    }
  }
}
#pragma endregion