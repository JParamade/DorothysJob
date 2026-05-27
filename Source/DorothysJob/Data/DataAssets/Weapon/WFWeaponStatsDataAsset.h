// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponStatsDataAsset.h"
#include "WFWeaponStatsDataAsset.generated.h"

class ABaseProjectile;
class UMainLegacyCameraShake;

UCLASS()
class DOROTHYSJOB_API UWFWeaponStatsDataAsset : public UBaseWeaponStatsDataAsset
{
  GENERATED_BODY()

public:
#pragma region | Stats
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Burst", meta = (DisplayName = "Burst Count"))
  int32 m_iBurstCount = 3;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Burst", meta = (DisplayName = "Burst Interval"))
  float m_fBurstInterval = 0.15f;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Burst", meta = (DisplayName = "Burst Dispersion"))
  float m_fBurstDispersion = 7.0f;
#pragma endregion

#pragma region | Cleaning
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Textures"))
  TArray<TObjectPtr<UTexture>> m_oCleanTextures;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Duration"))
  float m_fDuration = 0.15f;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Angle"))
  float m_fCleaningAngle = 0.0f;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean VFX Offset"))
  FVector m_vCleaningVFXOffset = FVector::ZeroVector;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Pivot"))
  FVector2D m_v2SCleaningPivot = FVector2D::ZeroVector;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Size"))
  FVector2D m_v2SCleaningSize = FVector2D::ZeroVector;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean VFX"))
  TObjectPtr<UNiagaraSystem> m_pCleanVFX;
#pragma endregion

#pragma region | Cone
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Status|Cone", meta = (DisplayName = "Cone Distance"))
  float m_fConeDistance = 700.0f;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Status|Cone", meta = (DisplayName = "Cone Angle"))
  float m_fConeAngle = 70.0f;
#pragma endregion

#pragma region | Projectile
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Specific Parameters|Projectile", meta = (DisplayName = "Base Projectile Class"))
  TSubclassOf<ABaseProjectile> m_oProjectileClass;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Specific Parameters|Projectile", meta = (DisplayName = "Special Projectile Class"))
  TSubclassOf<ABaseProjectile> m_oSpecialProjectileClass;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|VFX", meta = (DisplayName = "Shot VFX"))
  TObjectPtr<UNiagaraSystem> m_pWFShotVFX;

#pragma endregion

#pragma region | Special Attack
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Cleaning Texture"))
  TObjectPtr<UTexture> m_pCleaningTexture;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Starting Cleaning size"))
  FVector2D m_v2StartingCleaningSize = FVector2D::ZeroVector;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Max Cleaning size"))
  FVector2D m_v2SMaxCleaningSize = FVector2D::ZeroVector;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Growth Rate"))
  float m_fGrowthRate = 0.0f;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Update Rate"))
  float m_fUpdateRate = 0.0f;
#pragma endregion
};