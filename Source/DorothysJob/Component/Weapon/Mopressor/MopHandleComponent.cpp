  #include "MopHandleComponent.h"
  #include "DorothysJob/Actor/Weapon/Mopressor/Mopressor.h"
  #include "DorothysJob/Actor/Character/Player/BasePlayer.h"
  #include "DorothysJob/Component/Action/Attack/DamageCollider.h"
  #include "Components/ShapeComponent.h"
  #include "NiagaraComponent.h"
  #include "DorothysJob/Data/DataAssets/Weapon/MoppresorWeaponStatsDataAsset.h"


  UMopHandleComponent::UMopHandleComponent()
  {
    PrimaryComponentTick.bCanEverTick = true;
  }

  void UMopHandleComponent::BeginPlay()
  {
    Super::BeginPlay();

    if (UWorld* pCachedWorld = GetWorld())
    {
      CachedWorld = pCachedWorld;
    }

    if (AMopressor* Mopressor = Cast<AMopressor>(GetOwner()))
    {
      m_pWeaponOwner = Mopressor;
      m_pPlayerOwner = Cast<ABasePlayer>(Mopressor->GetOwner());
      if (UMoppresorWeaponStatsDataAsset* DataAsset = Cast<UMoppresorWeaponStatsDataAsset>(m_pWeaponOwner->GetDataAsset().Get()))
      {
        m_pDataAsset = DataAsset;
      }
    }
  }

  void UMopHandleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
  {
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (m_bIsSlashing && IsValid(m_pDamageCollider) && IsValid(m_pAttackPivot))
    {
      m_rCurrentAngle = FMath::RInterpConstantTo(m_rCurrentAngle, m_rEndAngle, DeltaTime, m_fRotationSpeed);
      m_pAttackPivot->SetRelativeRotation(m_rCurrentAngle);
      m_pDamageCollider->DrawDebug(FColor::Orange);

      if (m_rCurrentAngle.Equals(m_rEndAngle))
      {
        m_bIsSlashing = false;
        m_pDamageCollider->OnActive(false);
        m_lEnemiesToDamage.Empty();
      }
    }
  }

  void UMopHandleComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
  {
    if (IsValid(m_pDamageCollider))
    {
      TArray<UShapeComponent*> lBoxColliders = m_pDamageCollider->GetBoxColliders();
      if (lBoxColliders.IsValidIndex(lBoxColliders.Num() - 1) && IsValid(lBoxColliders[lBoxColliders.Num() - 1]))
      {
        lBoxColliders[lBoxColliders.Num() - 1]->OnComponentBeginOverlap.RemoveDynamic(this, &UMopHandleComponent::OnDamageColliderBeginOverlap);
      }
    }

    if (IsValid(m_pLastHitCollider))
    {
      TArray<UShapeComponent*> lSphereColliders = m_pLastHitCollider->GetSphereColliders();
      if (lSphereColliders.IsValidIndex(lSphereColliders.Num() - 1) && lSphereColliders[lSphereColliders.Num() - 1])
      {
        lSphereColliders[lSphereColliders.Num() - 1]->OnComponentBeginOverlap.RemoveDynamic(this, &UMopHandleComponent::OnLastHitColliderBeginOverlap);
      }
    }

    ResetTimeDilation();

    if (IsValid(CachedWorld))
    {
      CachedWorld->GetTimerManager().ClearAllTimersForObject(this);
    }

    Super::EndPlay(EndPlayReason);
  }

  void UMopHandleComponent::Use()
  {
    if (!m_pWeaponOwner.IsValid() || !IsValid(m_pLastHitCollider) || !IsValid(m_pDamageCollider) || !IsValid(m_pDataAsset))
    {
      return;
    }

    int32 Index = m_pWeaponOwner->CurrentComboHit();
    if (!m_pDataAsset->m_lComboHitsParameters.IsValidIndex(Index))
    {
      return;
    }

    FCombo CurrentComboData = m_pDataAsset->m_lComboHitsParameters[Index];

    if (Index < 2 && IsValid(m_pAttackPivot))
    {
      m_rCurrentAngle = CurrentComboData.m_rStartAngle;
      m_pAttackPivot->SetRelativeRotation(m_rCurrentAngle);
      m_rEndAngle = CurrentComboData.m_rEndAngle;
      m_fRotationSpeed = CurrentComboData.m_fRotationSpeed * m_fSpeedModifier;

      m_bIsSlashing = true;
      m_pDamageCollider->UpdateLocation(FVector(CurrentComboData.m_fBasicAttackSpawnDistance, 0, 0));
      m_pDamageCollider->OnActive(true);
    }
    else
    {
      //Third hit combo
      if (m_pPlayerOwner.IsValid() && IsValid(m_pPlayerOwner->GetSkeletalMesh()))
      {
        m_pLastHitCollider->UpdateLocation(m_pPlayerOwner->GetSkeletalMesh()->GetForwardVector() * CurrentComboData.m_fBasicAttackSpawnDistance);
      }

      m_pLastHitCollider->OnActive(true);
      m_pLastHitCollider->DrawDebug(FColor::Green);
    }

    if (IsValid(m_pSlashVFX) && !m_pSlashVFX->IsComplete())
    {
      m_pSlashVFX->DeactivateImmediate();
    }

    m_pSlashVFX = m_pWeaponOwner->CreateBasicAttackParticle(Index);

    if (IsValid(m_pSlashVFX))
    {
      m_pSlashVFX->Activate();
    }

    m_pWeaponOwner->OnMopressorAttack.Broadcast();
    m_pWeaponOwner->OnAttackStart.Broadcast(EAttackType::BASE_ATTACK);
  }

  void UMopHandleComponent::DeactivateCollider()
  {
    if (!m_pWeaponOwner.IsValid() || !IsValid(m_pLastHitCollider))
    {
      return;
    }

    if (m_pWeaponOwner->CurrentComboHit() == 2)
    {
      m_pLastHitCollider->OnActive(false);
    }

    m_bCooldownFreezeFrame = false;
  }

  void UMopHandleComponent::Attack(int32 _iInnerDamage, int32 _iOuterDamage, TArray<AActor*> _lInnerHitEnemies, AActor* _Enemy, FVector _ImpactPoint)
  {
    if (!m_pWeaponOwner.IsValid() || !IsValid(m_pDataAsset) || !IsValid(_Enemy))
    {
      return;
    }

    // we filter the actor and calculate its damage depending of its position
    bool bIsAlsoInInner = _lInnerHitEnemies.Contains(_Enemy);
    float DamageToApply = bIsAlsoInInner ? _iInnerDamage : _iOuterDamage;

    // we apply damage
    if (IDamageable* DamageableActor = Cast<IDamageable>(_Enemy))
    {
      // we apply the damage to the enemy
      m_pWeaponOwner->ApplyDamage(DamageableActor, DamageToApply, m_pDataAsset->m_eWeaponDirtType, true, _ImpactPoint);
    }
  }

  void UMopHandleComponent::Attack(int32 _iInnerDamage, int32 _iOuterDamage, TArray<AActor*> _lInnerHitEnemies, TArray<AActor*> _lOuterHitEnemies)
  {
    if (_lOuterHitEnemies.IsEmpty() || !m_pWeaponOwner.IsValid() || !IsValid(m_pDataAsset))
    {
      return;
    }

    for (AActor* Enemy : _lOuterHitEnemies)
    {
      // we check if valid
      if (!IsValid(Enemy))
      {
        continue;
      }

      // we filter the actor and calculate its damage depending of its position
      bool bIsAlsoInInner = _lInnerHitEnemies.Contains(Enemy);
      float DamageToApply = bIsAlsoInInner ? _iInnerDamage : _iOuterDamage;

      // we apply damage
      if (IDamageable* DamageableActor = Cast<IDamageable>(Enemy))
      {
        // we apply the damage to the enemy
        m_pWeaponOwner->ApplyDamage(DamageableActor, DamageToApply, m_pDataAsset->m_eWeaponDirtType, true);
      }
    }
  }

  void UMopHandleComponent::SetDamageCollider(ADamageCollider* _pDamageCollider, ADamageCollider* _pLastHitCollider)
  {
    m_pDamageCollider = _pDamageCollider;
    if (IsValid(m_pDamageCollider))
    {
      TArray<UShapeComponent*> lBoxColliders = m_pDamageCollider->GetBoxColliders();
      int32 Index = lBoxColliders.Num() - 1;
      if (lBoxColliders.IsValidIndex(Index) && IsValid(lBoxColliders[Index])) //outer box
      {
        lBoxColliders[Index]->OnComponentBeginOverlap.AddDynamic(this, &UMopHandleComponent::OnDamageColliderBeginOverlap);
      }
    }

    m_pLastHitCollider = _pLastHitCollider;
    if (IsValid(m_pLastHitCollider))
    {
      TArray<UShapeComponent*> lSphereColliders = m_pLastHitCollider->GetSphereColliders();
      int32 Index = lSphereColliders.Num() - 1;
      if (lSphereColliders.IsValidIndex(Index) && lSphereColliders[Index]) //outer sphere
      {
        lSphereColliders[Index]->OnComponentBeginOverlap.AddDynamic(this, &UMopHandleComponent::OnLastHitColliderBeginOverlap);
      }
    }
  }

  void UMopHandleComponent::SetAttackPivot(USceneComponent* _pivot)
  {
    m_pAttackPivot = _pivot;
  }

  void UMopHandleComponent::SetReductionMovility(bool _hasReduction)
  {
    m_bReductionMove = _hasReduction;
  }

  void UMopHandleComponent::SetAttackSpeedModifier(float _fSpeedModifier)
  {
    m_fSpeedModifier = _fSpeedModifier;
  }

  bool UMopHandleComponent::HasReductionMovility() const
  {
    return m_bReductionMove;
  }

  void UMopHandleComponent::SetReductionRotation(bool _hasReduction)
  {
    m_bReductionRotation = _hasReduction;
  }

  bool UMopHandleComponent::HasReductionRotation() const
  {
    return m_bReductionRotation;
  }

  bool UMopHandleComponent::IsSlashing() const
  {
    return m_bIsSlashing;
  }

  void UMopHandleComponent::ChangedWeapon()
  {
    ResetTimeDilation();
    if (IsValid(CachedWorld))
    {
      CachedWorld->GetTimerManager().ClearAllTimersForObject(this);
    }
    m_lEnemiesToDamage.Empty();

    DeactivateCollider();

    m_bIsSlashing = false;
    if (IsValid(m_pDamageCollider))
    {
      m_pDamageCollider->OnActive(false);
    }

    if (IsValid(m_pSlashVFX) && !m_pSlashVFX->IsComplete())
    {
      m_pSlashVFX->DeactivateImmediate();
    }
  }

  void UMopHandleComponent::SetFreezeFrameCooldown()
  {
    m_bCooldownFreezeFrame = false;
  }

  void UMopHandleComponent::OnDamageColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
  {
    if (!m_pWeaponOwner.IsValid() || !IsValid(OtherActor) || m_lEnemiesToDamage.Contains(OtherActor) ||
      !IsValid(m_pDataAsset) || !m_pPlayerOwner.IsValid() || !IsValid(m_pDamageCollider) || m_pDataAsset->m_oDamage.IsEmpty())
    {
      return;
    }

    //enemies that are in the smaller box
    //we get all the actors overlapping with the inner box, the area of the handle and base damage
    TArray<AActor*> lInnerHitEnemies;
    m_pDamageCollider->OnOverlap(lInnerHitEnemies, 0);

    FVector vDirection = m_pPlayerOwner->GetActorLocation() - OtherActor->GetActorLocation();
    vDirection.Z = 0;
    vDirection = vDirection.GetSafeNormal() * 80.0f;
    vDirection.Z = FMath::RandRange(50, 120);

    if (m_pDataAsset->m_oDamage.IsValidIndex(0) && m_pDataAsset->m_oDamage.IsValidIndex(1))
    {
      Attack(m_pDataAsset->m_oDamage[0], m_pDataAsset->m_oDamage[1], lInnerHitEnemies, OtherActor, OtherActor->GetActorLocation() + vDirection);
      m_pWeaponOwner->SetCameraShake(m_pDataAsset->m_oDamageCameraShake);
      m_pWeaponOwner->StartCameraShake();
      m_pWeaponOwner->StopControllerRumble();
      m_pWeaponOwner->StartControllerRumble(
        m_pDataAsset->m_oDamageVibrationParams.m_fIntensity,
        m_pDataAsset->m_oDamageVibrationParams.m_fDuration,
        m_pDataAsset->m_oDamageVibrationParams.m_bAffectsLeftLarge,
        m_pDataAsset->m_oDamageVibrationParams.m_bAffectsLeftSmall,
        m_pDataAsset->m_oDamageVibrationParams.m_bAffectsRightLarge,
        m_pDataAsset->m_oDamageVibrationParams.m_bAffectsRightSmall);
    }

    m_lEnemiesToDamage.AddUnique(OtherActor);

    if (IsValid(CachedWorld) && !CachedWorld->GetTimerManager().IsTimerActive(m_thGlobalTimeDilation) && !m_bCooldownFreezeFrame)
    {
      m_bCooldownFreezeFrame = true;
      m_pPlayerOwner->CustomTimeDilation = 0.f;
      m_pWeaponOwner->CustomTimeDilation = 0.f;

      if (IsValid(m_pSlashVFX) && !m_pSlashVFX->IsComplete())
      {
        m_pSlashVFX->SetPaused(true);
      }

      CachedWorld->GetTimerManager().SetTimer(m_thGlobalTimeDilation, this, &UMopHandleComponent::ResetTimeDilation, m_pDataAsset->m_fFreezeFrameTime, false);
    }

    if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(OtherActor))
    {
      m_pWeaponOwner->OnMeleeHit.Broadcast(pCharacter->GetMitigation(), false);
    }
  }

  void UMopHandleComponent::OnLastHitColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
  {
    if (!m_pWeaponOwner.IsValid() || !IsValid(m_pDataAsset) || !IsValid(OtherActor))
    {
      return;
    }

    if (IDamageable* DamageableActor = Cast<IDamageable>(OtherActor))
    {
      FVector vDirection = m_pPlayerOwner->GetActorLocation() - OtherActor->GetActorLocation();
      vDirection.Z = 0;
      vDirection = vDirection.GetSafeNormal() * 80;
      vDirection.Z = FMath::RandRange(50, 120);

      // we apply the damage to the enemy
      if (m_pDataAsset->m_oDamage.IsValidIndex(1))
      {
        m_pWeaponOwner->ApplyDamage(DamageableActor, m_pDataAsset->m_oDamage[1], m_pDataAsset->m_eWeaponDirtType, true, OtherActor->GetActorLocation() + vDirection);
        m_pWeaponOwner->SetCameraShake(m_pDataAsset->m_oFinalAttackCameraShake);
        m_pWeaponOwner->StartCameraShake();
        m_pWeaponOwner->StopControllerRumble();
        m_pWeaponOwner->StartControllerRumble(
          m_pDataAsset->m_oFinalAttackVibration.m_fIntensity,
          m_pDataAsset->m_oFinalAttackVibration.m_fDuration,
          m_pDataAsset->m_oFinalAttackVibration.m_bAffectsLeftLarge,
          m_pDataAsset->m_oFinalAttackVibration.m_bAffectsLeftSmall,
          m_pDataAsset->m_oFinalAttackVibration.m_bAffectsRightLarge,
          m_pDataAsset->m_oFinalAttackVibration.m_bAffectsRightSmall);
      }
      if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(OtherActor))
      {
        m_pWeaponOwner->OnMeleeHit.Broadcast(pCharacter->GetMitigation(), false);
      }
    }
  }

  void UMopHandleComponent::ResetTimeDilation()
  {
    if (m_pPlayerOwner.IsValid() && m_pWeaponOwner.IsValid())
    {
      m_pPlayerOwner->CustomTimeDilation = 1.f;
      m_pWeaponOwner->CustomTimeDilation = 1.f;
    }

    if (IsValid(m_pSlashVFX) && m_pSlashVFX->IsPaused())
    {
      m_pSlashVFX->SetPaused(false);
    }

    if (IsValid(CachedWorld) && IsValid(m_pDataAsset))
    {
      CachedWorld->GetTimerManager().SetTimer(m_thCooldownTimeDilation, this, &UMopHandleComponent::SetFreezeFrameCooldown, m_pDataAsset->m_fCooldownFreezeFrameTime, false);
    }
  }