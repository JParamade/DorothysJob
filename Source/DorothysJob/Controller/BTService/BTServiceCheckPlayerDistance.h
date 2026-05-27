/************************************************************************
 * @description: Service to check the player distance 
                 with an attack radius and a flee radius.
 * @author: David Gonzalez
 * @date: 08/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceCheckPlayerDistance.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTServiceCheckPlayerDistance : public UBTService
{
	GENERATED_BODY()
	
public:
  UBTServiceCheckPlayerDistance();

  /**
   * @brief It returns a short description for the service.
   */
  virtual FString GetStaticDescription() const override;

protected:

  /**
   * @brief It checks the distance from the enemy to the player.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   * @param DeltaSeconds -> Seconds since the last execution.
   */
  virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

  /**
   * Radius to attack to the player.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Attack Radius"))
  FBlackboardKeySelector m_tAttackRadiusKey;

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
   * Blackboard key where the brave duration will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Brave Duration Key"))
  FBlackboardKeySelector m_tBraveDurationKey;

  /**
   * Blackboard key if the AI should attack to the player.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Should Attack Key"))
  FBlackboardKeySelector m_tShouldAttackKey;

  /**
   * Blackboard key if the AI should flee from the player.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Should Flee Key"))
  FBlackboardKeySelector m_tShouldFleeKey;

  /**
   * Target to attack.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target To Attack"))
  FBlackboardKeySelector m_tTargetToAttackKey;
};
