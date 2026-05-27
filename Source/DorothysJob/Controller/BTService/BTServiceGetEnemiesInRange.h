/************************************************************************
 * @description: Service to check the enemies in attack range.
 * @author: Raul Fernandez
 * @date: 08/05/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceGetEnemiesInRange.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTServiceGetEnemiesInRange : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTServiceGetEnemiesInRange();

  /**
   * @brief It returns a short description for the service.
   */
	virtual FString GetStaticDescription() const override;

protected:

  /**
   * @brief It checks if there are enemies in attack range.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   * @param DeltaSeconds -> Seconds since the last execution.
   */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

  /**
   * Radius to detect enemies.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Detect Radius"))
  FBlackboardKeySelector m_tDetectRadiusKey;

  /**
   * Radius to flee from the player.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Flee Radius"))
  FBlackboardKeySelector m_tFleeRadiusKey;

  /**
   * Blackboard key where the last flee time will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Last Flee Date"))
  FBlackboardKeySelector m_tLastFleeDateKey;

  /**
  * Blackboard key where the brave mode will be stored.
  */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Brave Mode Key"))
  FBlackboardKeySelector m_tBraveModeKey;

  /**
   * Blackboard key where the brave duration will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Brave Duration Key"))
  FBlackboardKeySelector m_tBraveDurationKey;

  /**
   * Blackboard key if the AI should attack.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Should Attack Key"))
  FBlackboardKeySelector m_tShouldAttackKey;

  /**
   * Blackboard key if the AI should flee from the player.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Should Flee Key"))
  FBlackboardKeySelector m_tShouldFleeKey;

	/**
	 * If there are enemies in range.
	 */
	UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Characters In Range"))
	FBlackboardKeySelector m_tCharactersInRangeKey;

  /**
   * Enemy to chase.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Enemy To Chase"))
  FBlackboardKeySelector m_tEnemyToChaseKey;
};