// Fill out your copyright notice in the Description page of Project Settings.


#include "ChimneyGimmick.h"
#include "DorothysJob/Component/Dust/DustHealthComponent.h"
#include "DorothysJob/Utils/Random/RandomNumberGenerator.h"
#include "DorothysJob/Actor/Character/Enemy/DustBunny.h"


AChimneyGimmick::AChimneyGimmick()
{
  m_fSpawnCooldown = 3.f;
  m_iSpawnEnemyNumber = 2;
  m_fSpawnMinRadius = 300.f;
  m_fSpawnMaxRadius = 800.f;
  m_fSpawnAngle = 45.f;
  m_iMaxHealth = 10;
  m_iCurrentHealth = m_iMaxHealth;
  m_fRestoreCooldown = 20.f;
  m_bIsOff = false;

  m_pSpawnEnemyComponent = CreateDefaultSubobject<USpawnEnemyComponent>(TEXT("SpawnEnemyComponent"));
  m_pSpawnEnemyComponent->SetEnemyPool(ADustBunny::StaticClass(), 100.f);

  m_pDustHealthComponent = CreateDefaultSubobject<UDustHealthComponent>(TEXT("DustHealthComponent"));
}

void AChimneyGimmick::Tick(float _DeltaTime)
{

  UE_LOG(LogTemp, Display, TEXT("%d"), m_iCurrentHealth);

  //ALL THIS IS FOR DEBUG ONLY, FOR THE VISUAL OF TEH SPAWN RANGE
  FVector Origin = GetActorLocation();
  FVector Forward = GetActorForwardVector();
  FVector UpVector = FVector::UpVector;

  float MinRadius = m_fSpawnMinRadius;
  float MaxRadius = m_fSpawnMaxRadius;

  int32 NumSegments = 30; 
  float HalfAngleDegrees = m_fSpawnAngle;
  float StartAngle = -HalfAngleDegrees;
  float EndAngle = HalfAngleDegrees;

  // Dibuja un arco en el radio máximo
  for (int32 i = 0; i < NumSegments; ++i)
  {
    float AngleDeg1 = FMath::Lerp(StartAngle, EndAngle, i / (float)NumSegments);
    float AngleDeg2 = FMath::Lerp(StartAngle, EndAngle, (i + 1) / (float)NumSegments);

    FVector Dir1 = Forward.RotateAngleAxis(AngleDeg1, UpVector);
    FVector Dir2 = Forward.RotateAngleAxis(AngleDeg2, UpVector);

    FVector Point1 = Origin + Dir1 * MaxRadius;
    FVector Point2 = Origin + Dir2 * MaxRadius;

    DrawDebugLine(GetWorld(), Point1, Point2, FColor::Green, false, -1.f, 0, 2.f);
  }

  // Dibuja líneas radiales entre radio mínimo y máximo para delimitar el arco
  FVector EdgeDir1 = Forward.RotateAngleAxis(StartAngle, UpVector);
  FVector EdgeDir2 = Forward.RotateAngleAxis(EndAngle, UpVector);

  DrawDebugLine(GetWorld(), Origin + EdgeDir1 * MinRadius, Origin + EdgeDir1 * MaxRadius, FColor::Blue, false, -1.f, 0, 1.f);
  DrawDebugLine(GetWorld(), Origin + EdgeDir2 * MinRadius, Origin + EdgeDir2 * MaxRadius, FColor::Blue, false, -1.f, 0, 1.f);
}

int AChimneyGimmick::GetCurrentHealth() const
{
  return m_iCurrentHealth;
}

void AChimneyGimmick::SetCurrentHealth(int32 _iDamage)
{
  m_iCurrentHealth = _iDamage;
}

int AChimneyGimmick::GetMaxHealth() const
{
  return m_iMaxHealth;
}

int32 AChimneyGimmick::Damage(int _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _bIsCritical)
{
  //usar este componente para regenerar vida
  m_pDustHealthComponent->ModifyHealth(_iModifyHealth);
  return _iModifyHealth;
}

void AChimneyGimmick::Heal(int _iModifyHealth)
{
}

void AChimneyGimmick::BeginPlay()
{
  Super::BeginPlay();
  GimmickActivation();
}

void AChimneyGimmick::GimmickActivation()
{
  GetWorldTimerManager().SetTimer(m_oSpawnTimer, this, &AChimneyGimmick::SpawnLoop, m_fRestoreCooldown, true);

  if (m_pDustHealthComponent)
  {
    m_pDustHealthComponent->OnDeath.AddDynamic(this, &AChimneyGimmick::TurnOffActor);
    m_pDustHealthComponent->SetMaxHealth(m_iMaxHealth);
    m_pDustHealthComponent->SetCurrentHealth(m_iCurrentHealth);
  }
}

void AChimneyGimmick::SpawnLoop()
{
  //spawnear dust bunnys
  for (int i = 0; i<m_iSpawnEnemyNumber; i++)
  {
    FVector SpawnPos = GetRandomSpawnLocationAroundActor(m_fSpawnMinRadius, m_fSpawnMaxRadius);
    m_pSpawnEnemyComponent->SpawnEnemy(SpawnPos);
  }
  
}

FVector AChimneyGimmick::GetRandomSpawnLocationAroundActor(float MinRadius, float MaxRadius)
{
  //Coger localización donde spawnear dust bunnys
  RandomNumberGenerator rng;

  FVector Origin = GetActorLocation();
  FVector Forward = GetActorForwardVector();

  // Ángulo del semicírculo (180 grados)
  float HalfAngleRad = FMath::DegreesToRadians(m_fSpawnAngle);

  // Generar un ángulo aleatorio en el rango [-90°, 90°] (semicírculo delante)
  float RandomAngle = rng.GetRandomInRange(-HalfAngleRad, HalfAngleRad);

  // Rotar el vector forward alrededor del eje Z para obtener la dirección aleatoria en el semicírculo
  FVector Direction = Forward.RotateAngleAxis(FMath::RadiansToDegrees(RandomAngle), FVector::UpVector);
  Direction.Normalize();

  // Generar distancia aleatoria entre min y max
  float Distance = rng.GetRandomInRange(MinRadius, MaxRadius);

  // Calcular la posición final sumando la dirección * distancia al origen
  FVector SpawnLocation = Origin + Direction * Distance;

  return SpawnLocation;
}

void AChimneyGimmick::TurnOffActor()
{
  //Deja de spawnear, falta hacer eso.
  m_bIsOff = true;
  GetWorld()->GetTimerManager().ClearTimer(m_oSpawnTimer);
}
