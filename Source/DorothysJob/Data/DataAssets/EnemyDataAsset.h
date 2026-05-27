/************************************************************************
 * @description: Represents the Enemy Data Asset.
 * @author: Pablo Velasco
 * @date: 31/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDataAsset.generated.h"

class ABaseEnemy;

UCLASS()
class DOROTHYSJOB_API UEnemyDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Enemies", meta = (DisplayName = "Slime Class"))
  TSubclassOf<ABaseEnemy> m_cSlimeClass;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Enemies", meta = (DisplayName = "Mini Slime Class"))
  TSubclassOf<ABaseEnemy> m_cMiniSlimeClass;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Enemies", meta = (DisplayName = "Slime Divider Class"))
  TSubclassOf<ABaseEnemy> m_cSlimeDividerClass;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Enemies", meta = (DisplayName = "Ash Bunny Class"))
  TSubclassOf<ABaseEnemy> m_cAshBunny;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Enemies", meta = (DisplayName = "Dust Bunny Class"))
  TSubclassOf<ABaseEnemy> m_cDustBunny;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Enemies", meta = (DisplayName = "Goblin Class"))
  TSubclassOf<ABaseEnemy> m_cGoblin;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Others", meta = (DisplayName = "Cleaning Station Class"))
  TSubclassOf<AActor> m_cCleaningStation;

  /**
   * It is the capsule height for all the enemies.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Capsule", meta = (DisplayName = "Height"))
  float m_fCapsuleHeight = 105.f;

  /**
   * It is the z-offset to spawn the enemies.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Capsule", meta = (DisplayName = "Z Offset"))
  float m_fCapsuleZOffset = 9.9f;
};
