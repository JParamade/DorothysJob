/************************************************************************
 * @description: Represents the Base AI Controller in the Game.
 *							 This class executes the BehaviorTree of the AI.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Aurora
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UBlackboardComponent;

UCLASS()
class DOROTHYSJOB_API ABaseAIController : public AAIController
{
  GENERATED_BODY()

public:

  /**
   * @brief Default constructor.
   */
  ABaseAIController();

protected:
  
  /**
   * @brief It is called when game starts or when spawned.
   */
  virtual void BeginPlay() override;

  /**
   * @brief It is called when the Pawn gets contolled by the Controller.
   * @param InPawn -> Pawn to control.
   */
  virtual void OnPossess(APawn* InPawn) override;

  /**
   * Blackboard component for the behavior tree.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
  UBlackboardComponent* m_pBlackboardComponent;
};