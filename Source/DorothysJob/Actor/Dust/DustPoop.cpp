#include "DustPoop.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Actor/Room/Tile/BaseTile.h"
#include "DorothysJob/Utils/Random/RandomNumberGenerator.h"
#include "DorothysJob/Component/Dust/DustHealthComponent.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Actor/Character/Enemy/DustBunny.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

ADustPoop::ADustPoop() :
  m_vDirtSize(FVector2D(200.0f)),
  m_pDirtTexture(nullptr),
  m_pCleanTexture(nullptr),
  m_iMaxHealth(20),
  m_iDamageMultiplier(5),
  m_bShowDebug(false)
{
  PrimaryActorTick.bCanEverTick = false;
  m_pDustBunnyHidden = nullptr;

  m_pCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->InitSphereRadius(115.f);
    m_pCollisionComponent->CanCharacterStepUpOn = ECB_No;
    m_pCollisionComponent->SetCanEverAffectNavigation(false);
    m_pCollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel2);
    RootComponent = m_pCollisionComponent;
  }

  m_pMeshComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    m_pMeshComponent->SetupAttachment(RootComponent);
    m_pMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_pMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    m_pMeshComponent->SetGenerateOverlapEvents(false);
    m_pMeshComponent->SetCanEverAffectNavigation(false);
    m_pMeshComponent->SetRelativeTransform(FTransform(FRotator(-1.5, 0., 1.5), FVector(0., 0., -111.)));
  }

  m_pDrawingComponent = CreateDefaultSubobject<UDrawing>(TEXT("DrawingComponent"));
  m_pDustHealthComponent = CreateDefaultSubobject<UDustHealthComponent>(TEXT("DustHealthComponent"));

  m_pWeakPointComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WeakPoint"));
  if (IsValid(m_pWeakPointComponent))
  {
    m_pWeakPointComponent->SetupAttachment(m_pMeshComponent);
    m_pWeakPointComponent->SetRelativeLocation(FVector(0., 0., 4.));
  }
}

void ADustPoop::BeginPlay()
{
  Super::BeginPlay();

  // Random number generator
  m_pRandom = MakeShared<RandomNumberGenerator>();
  m_eDirWS = EDirWeakPoint::NONE;
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
  if (IsValid(pGameInstance))
  {
    for (const TObjectPtr<const UWeaponDataAsset>& pWeapon : pGameInstance->m_lSelectedWeapons)
    {
      if (IsValid(pWeapon) && (pWeapon->m_eWeaponAttackType == EWeaponAttackType::Melee) && pWeapon->m_eWeaponCleanType == EDirtType::Dust)
      {
        m_eDirWS = GenerateWeakPoint();
        break;
      }
    }
  }

  if (IsValid(m_pDustHealthComponent))
  {
    m_pDustHealthComponent->OnDeath.AddDynamic(this, &ADustPoop::PoopCleaned);
    m_pDustHealthComponent->SetMaxHealth(m_iMaxHealth);
    m_pDustHealthComponent->SetCurrentHealth(m_iMaxHealth);
  }

  if (IsValid(m_pDrawingComponent))
  {
    m_pDrawingComponent->OnFloorFound.AddDynamic(this, &ADustPoop::FloorDirt);
    FloorDirt();
  }
}

void ADustPoop::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (m_bShowDebug)
  {
    DrawDebug();
  }
}

#pragma region | Stats
int ADustPoop::GetCurrentHealth() const
{
  return m_pDustHealthComponent->GetCurrentHealth();
}

void ADustPoop::SetCurrentHealth(int32 _iDamage)
{
  m_pDustHealthComponent->SetCurrentHealth(_iDamage);
}

int ADustPoop::GetMaxHealth() const
{
  return m_iMaxHealth;
}

int32 ADustPoop::Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _bIsCritical)
{
  if ((m_pDustHealthComponent->GetCurrentHealth() <= 0) || (_iModifyHealth <= 0))
  {
    return 0;
  }

  int iDamageMultiplier = 1;
  if (_bIsCritical)
  {
    iDamageMultiplier = CheckCriticalHit();
  }
 
  m_pDustHealthComponent->ModifyHealth(-_iModifyHealth * iDamageMultiplier);
  OnHealthChanged.Broadcast();
  RescaleMeshHealth();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>())
    {
      pAudioManager->PlaySound2D(this, (iDamageMultiplier > 1) ? m_pCriticEvent : m_pImpactEvent);
    }
  }

  return (_iModifyHealth * iDamageMultiplier);
}

void ADustPoop::Heal(int32 _iModifyHealth)
{
  m_pDustHealthComponent->ModifyHealth(_iModifyHealth);
  RescaleMeshHealth();
}
#pragma endregion

#pragma region | Getters
TObjectPtr<UStaticMeshComponent> ADustPoop::GetStaticMesh() const
{
  return m_pMeshComponent;
}

bool ADustPoop::IsDustBunnyHidden() const
{
  return m_pDustBunnyHidden.IsValid();
}
#pragma endregion

void ADustPoop::SetDustBunnyHidden(TWeakObjectPtr<ADustBunny> _pDustBunny)
{
  m_pDustBunnyHidden = _pDustBunny;
  if (m_pDustBunnyHidden.IsValid())
  {
    m_pDustBunnyHidden->OnToHideComplete.AddDynamic(this, &ADustPoop::PoopMaterialModify);
  }
}

#pragma region | Private Functions
int ADustPoop::CheckCriticalHit()
{
  int iDamageMultiplier = 1;
  APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  if (!IsValid(pPlayerPawn))
  {
    return iDamageMultiplier;
  }

  FVector vDirectionToEnemy = (pPlayerPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal();
  double dForwardDot = FVector::DotProduct(FVector::ForwardVector, vDirectionToEnemy);
  double dRightDot = FVector::DotProduct(FVector::RightVector, vDirectionToEnemy);

  bool bCondition = (m_eDirWS == EDirWeakPoint::NORTH) && (dForwardDot > 0.707);
  bCondition = bCondition || (m_eDirWS == EDirWeakPoint::SOUTH) && (dForwardDot < -0.707);
  bCondition = bCondition || (m_eDirWS == EDirWeakPoint::EAST) && (dRightDot > 0.707);
  bCondition = bCondition || (m_eDirWS == EDirWeakPoint::WEST) && (dRightDot < -0.707);

  if (bCondition)
  {
    iDamageMultiplier = m_iDamageMultiplier;
    m_eDirWS = GenerateWeakPoint();
  }

  return iDamageMultiplier;
}

EDirWeakPoint ADustPoop::GenerateWeakPoint() const
{
  if (!IsValid(m_pWeakPointComponent))
  {
    return EDirWeakPoint::NONE;
  }

  EDirWeakPoint eWeakPoint = static_cast<EDirWeakPoint>(m_pRandom->GetRandomInRange(1, 4));
  uint32 uTries = 0u;

  while ((eWeakPoint == m_eDirWS) && (uTries < 10u))
  {
    eWeakPoint = static_cast<EDirWeakPoint>(m_pRandom->GetRandomInRange(1, 4));
    ++uTries;
  }

  m_pWeakPointComponent->SetVariableFloat(TEXT("Spawn X Negative"), eWeakPoint == EDirWeakPoint::SOUTH ? 1 : 0);
  m_pWeakPointComponent->SetVariableFloat(TEXT("Spawn Y Negative"), eWeakPoint == EDirWeakPoint::WEST ? 1 : 0);
  m_pWeakPointComponent->SetVariableFloat(TEXT("Spawn Y positive"), eWeakPoint == EDirWeakPoint::EAST ? 1 : 0);
  m_pWeakPointComponent->SetVariableFloat(TEXT("Spawn X positive"), eWeakPoint == EDirWeakPoint::NORTH ? 1 : 0);
  m_pWeakPointComponent->Activate();

  return eWeakPoint;
}

void ADustPoop::RescaleMeshHealth()
{
  if (!IsValid(m_pMeshComponent))
  {
    return;
  }

  float fNormalizeHealth = FMath::Clamp((GetCurrentHealth() - 5.f) / (GetMaxHealth() - 5.f), 0.f, 1.f);
  float fNewScale = FMath::Lerp(0.5f, 1.f, fNormalizeHealth);
  m_pMeshComponent->SetWorldScale3D(FVector(fNewScale));
}

void ADustPoop::FloorDirt()
{
  if (IsValid(m_pDrawingComponent))
  {
    m_pDrawingComponent->Dirt(m_pDirtTexture, m_pCleanTexture, m_vDirtSize, 0.f, FVector2D(0.5), EDrawColor::ERed, EDrawTier::Dust1);
  }
}

void ADustPoop::PoopCleaned()
{
  if (m_pDustBunnyHidden.IsValid())
  {
    m_pDustBunnyHidden->UnHide();
    m_pDustBunnyHidden = nullptr;
  }
  OnDustPoopEliminate.Broadcast(this);
  if (IsValid(m_pDrawingComponent))
  {
    m_pDrawingComponent->Clean(m_pCleanTexture, m_vDirtSize * 1.1f, 0.f, FVector2D(0.5f), EDrawTier::Dust1);
    m_pDrawingComponent->Clean(m_pCleanTexture, m_vDirtSize, 0.f, FVector2D(0.5f), EDrawTier::Liquid);
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->SetDustySAAchievement();
    }
  }
  Destroy();
}

void ADustPoop::PoopMaterialModify(ABaseEnemy* _pEnemy)
{
  if (m_pDustBunnyHidden.IsValid() && IsValid(_pEnemy) && IsValid(m_pPoweredMaterial))
  {
    ADustBunny* pDustBunny = Cast<ADustBunny>(_pEnemy);
    if (pDustBunny == m_pDustBunnyHidden.Get())
    {
      m_pDustBunnyHidden->OnToHideComplete.RemoveDynamic(this, &ADustPoop::PoopMaterialModify);
      m_pMeshComponent->SetMaterial(0, m_pPoweredMaterial);
    }
  }
}
#pragma endregion

#pragma region | Debug
void ADustPoop::DrawDebug()
{
  APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  if (!IsValid(pPlayerPawn))
  {
    return;
  }

  FVector vEnemyLocation = GetActorLocation();
  if (m_eDirWS != EDirWeakPoint::NONE)
  {
    float fLineLength = 200.f;
    FVector vForward = ((m_eDirWS == EDirWeakPoint::NORTH) ? fLineLength : (m_eDirWS == EDirWeakPoint::SOUTH) ? -fLineLength : 0) * FVector::ForwardVector;
    FVector vRight = ((m_eDirWS == EDirWeakPoint::EAST) ? fLineLength : (m_eDirWS == EDirWeakPoint::WEST) ? -fLineLength : 0) * FVector::RightVector;
    FVector vLineEnd = vEnemyLocation + vForward + vRight;
    DrawDebugLine(GetWorld(), vEnemyLocation, vLineEnd, FColor::Green, false, 0.2f, 0, 3.f);
  }

  FVector vPlayerLocation = pPlayerPawn->GetActorLocation();
  FVector vDirectionToPlayer = (vPlayerLocation - vEnemyLocation).GetSafeNormal();
  float fAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(GetActorForwardVector(), vDirectionToPlayer)));

  DrawDebugLine(GetWorld(), vPlayerLocation, vEnemyLocation, FColor::Red, false, 0.3f, 0, 3.f);
  GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Yellow, FString::Printf(TEXT("Angle: %.2f degrees"), fAngle));
}
#pragma endregion