/************************************************************************
 * @description: Represents the Base Status Condition.
 * @author: Pablo Velasco
 * @date: 18/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "BaseStatus.generated.h"


class ABaseCharacter;

/**
 * Represents the different kinds of Status
 */
UENUM(BlueprintType)
enum class EStatus : uint8
{
  SLOW UMETA(DisplayName = "Slow"),

  EXPOSURE UMETA(DisplayName = "Exposure"),

  STUN UMETA(DisplayName = "Stun"),

  DIZZY UMETA(DisplayName = "Dizzy"),

  NONE UMETA(DisplayName = "None")
};

UCLASS(Blueprintable)
class DOROTHYSJOB_API UBaseStatus : public UObject
{
  GENERATED_BODY()
public:
  
  /**
   * @brief Applies the Status to the given target, this function stores the target as well.
   *        The status' effects depends on the status' class.
   * @param _pTarget The Character that the status is going to be applied to.
   */
  virtual void ApplyStatus(ABaseCharacter* _pTarget);

  /**
   * @brief Removes this Status from the previously given target.
   */
  virtual void RemoveStatus();

  /**
   * @brief Gets the duration that this status effect will have every time it's applied.
   * @return The number of seconds that the status will last.
   */
  float GetDuration() const;

  bool IsReactivable() const;

  UFUNCTION(BlueprintCallable)
  EStatus GetStatusType() const;

protected:
  /**
   * @brief Number of seconds that the status will last every thime it's applied.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Stats", meta = (DisplayName = "Duration"))
  float m_fDuration;

  /**
   * @brief Character that will be the target of the Status Effect
   */
  UPROPERTY()
  ABaseCharacter* m_pTarget;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Stats", meta = (DisplayName = "Type"))
  EStatus m_statusType;

  /**
   * @brief Handles the timer of the status.
   */
  UPROPERTY()
  FTimerHandle m_oTimeHandler;

  bool  m_bReactivable = false;

  /**
   * @brief Virtual function that will be overwritten by the childrend of this class.
   *        It will apply to the target the effects of the status.
   *        Depending on the effect of the status it will have a different behaviour.
   */
  virtual void StatusEffect() {};

  /**
   * @brief Virtual function that will be overwritten by the children of this class.
   *        It will revert the effects of the status from the target.
   *        Depending on the effect of the status it will have a different behaviour.
   */
  virtual void RevertEffect() {};
private:
};
