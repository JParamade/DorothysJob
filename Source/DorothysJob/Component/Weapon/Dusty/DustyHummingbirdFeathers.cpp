#include "DustyHummingbirdFeathers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DorothysJob/Actor/Weapon/Dusty/Dusty.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include <DorothysJob/Data/DataAssets/Weapon/DustyWeaponStatsDataAsset.h>
void UDustyHummingbirdFeathers::Use()
{
  CleanAttack();
}

void UDustyHummingbirdFeathers::Release()
{
  OnDustyCleanEnd.Broadcast();

  if (ABaseWeapon* pWeapon = Cast<ABaseWeapon>(GetOwner())) {
    pWeapon->OnCleanEnd.Broadcast();
    m_bIsCleaning = false;
  }
}

void UDustyHummingbirdFeathers::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

void UDustyHummingbirdFeathers::CleanAttack()
{
  TArray<IDamageable*> dustPoops = GetCleaningObjects();

  ABaseWeapon* pWeapon = Cast<ABaseWeapon>(GetOwner());

  ADusty* pOwner = Cast<ADusty>(GetOwner());
  if (!pOwner)
  {
    return;
  }
  UDustyWeaponStatsDataAsset* oDataAsset = Cast<UDustyWeaponStatsDataAsset>(pOwner->GetDataAsset().Get());

  for (IDamageable* dp : dustPoops)
  {
    pOwner->ApplyDustPoopDamage(dp, oDataAsset->m_iCleanDamage, EDirtType::Dust, FVector::ZeroVector, FRotator::ZeroRotator, true);
  }
  OnDustyClean.Broadcast();
  if (!m_bIsCleaning) {
    pWeapon->OnCleanStart.Broadcast();
    m_bIsCleaning = true;
  }
}

TArray<IDamageable*> UDustyHummingbirdFeathers::GetCleaningObjects()
{
  TArray<IDamageable*> return_;
  ABasePlayer* m_playerPawn = Cast<ABasePlayer>(GetOwner()->GetOwner());
  TArray<TEnumAsByte<EObjectTypeQuery>> objects;
  FVector SpherePosition = m_playerPawn->GetActorLocation() + m_playerPawn->GetSkeletalMesh()->GetForwardVector() * (m_fCleanRange / 2.f);

  objects.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel2));
  TArray<AActor*> ignoreActors;
  ignoreActors.Add(GetOwner());
  ignoreActors.Add(m_playerPawn);

  TArray<AActor*> outActors;

    DrawDebugSphere(
      GetWorld(),
      SpherePosition,
      m_fRadius,
      12,
      FColor::Blue,
      false,
      0.4f
    );
    UKismetSystemLibrary::SphereOverlapActors(
      GetWorld(),
      SpherePosition,
      m_fRadius,
      objects,
      nullptr,
      ignoreActors,
      outActors
    );

  for (AActor* actor : outActors)
  {
    if (IDamageable* DamActor = Cast<IDamageable>(actor))
    {
      return_.Add(DamActor);
    }
  }

  return return_;
}

float UDustyHummingbirdFeathers::GetCleanRange() const
{
  return m_fCleanRange;
}

float UDustyHummingbirdFeathers::GetCleanRadius() const
{
  return m_fRadius;
}

void UDustyHummingbirdFeathers::SetCleanRange(float _range)
{
  m_fCleanRange = _range;
}

void UDustyHummingbirdFeathers::SetCleanRadius(float _fRadius)
{
  m_fRadius = _fRadius;
}
