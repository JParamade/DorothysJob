/************************************************************************
 * @description: Represents the Weapo Data Asset.
 * @author: David Gonzalez
 * @date: 31/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"

class ABaseWeapon;
enum class EDirtType : uint8;

UENUM()
enum class EWeaponAttackType : uint8
{
  Melee     UMETA(DisplayName = "Melee"),
  Distance  UMETA(DisplayName = "Distance")
};

UENUM()
enum class EWeaponHandSocket : uint8
{
  Left      UMETA(DisplayName = "Left"),
  Right     UMETA(DisplayName = "Right"),
  Both      UMETA(DisplayName = "Both")
};


USTRUCT()
struct FWeaponCombatData
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Montage Name"))
  FName m_sMontageSectionName;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Damage"))
  int m_iDamage = 1;
};

UCLASS()
class DOROTHYSJOB_API UWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Name"))
  FText m_sName;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Weapon Clean Type"))
  EDirtType m_eWeaponCleanType;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Weapon Attack Type"))
  EWeaponAttackType m_eWeaponAttackType;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Hand Socket"))
  EWeaponHandSocket m_eWeaponHandSocket;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Description"))
  FText m_sDescription;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Weapon Class"))
  TSubclassOf<ABaseWeapon> m_cWeaponClass;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Icon"))
  TObjectPtr<UTexture2D> m_pIcon;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters|UI", meta = (DisplayName = "Progress Bar Fill Image"))
  TObjectPtr<UTexture2D> m_pFillImage;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters|UI", meta = (DisplayName = "Progress Bar Background Image"))
  TObjectPtr<UTexture2D> m_pBackgroundImage;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters|UI", meta = (DisplayName = "Min Progress Bar Value"))
  float m_fMinBarValue;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters|UI", meta = (DisplayName = "Max Progress Bar Value"))
  float m_fMaxBarValue;
};
