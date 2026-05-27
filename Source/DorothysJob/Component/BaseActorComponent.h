/************************************************************************
 * @description: Represents a Base Actor Component in the Game
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Josephine
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseActorComponent.generated.h"

 /**
 * @brief Declaration of the Category for Logs
 */
DECLARE_LOG_CATEGORY_EXTERN(ComponentLog, Log, All);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UBaseActorComponent : public UActorComponent
{
  GENERATED_BODY()

public:
  /**
   * @brief Default Constructor
   */
  UBaseActorComponent();

protected:
  /**
   * @brief Executes when the Compont is loaded
   */
  virtual void BeginPlay() override;

public:
  /**
   * @brief Executes every Frame
   * @param DeltaTime
   * @param TickType
   * @param ThisTickFunction
   */
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


};