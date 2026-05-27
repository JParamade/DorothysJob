/************************************************************************
 * @description: Represents the Audio Manager class.
 * @author: Jaime Paramo
 * @date: 07/07/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioManager.generated.h"

USTRUCT(BlueprintType)
struct FAudioParam
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName sName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  float fValue = 0.0f;
};

// Audio
class UFMODAudioComponent;
class UFMODEvent;
class UFMODBus;
namespace FMOD {
  namespace Studio {
    class Bus;
    class EventInstance;
    class System;
  }
}

UCLASS()
class DOROTHYSJOB_API UAudioManager : public UGameInstanceSubsystem
{
  GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase& _rCollection) override;

  /**
   * @brief
   * @param _pWorldContextObject
   * @param _pEvent
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlaySound2D(UObject* _pWorldContextObject, UFMODEvent* _pEvent);
  /**
   * @brief
   * @param _pWorldContextObject
   * @param _pEvent
   * @param _vLocation
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlaySoundAtLocation(UObject* _pWorldContextObject, UFMODEvent* _pEvent, FVector _vLocation);
  /**
   * @brief
   * @param _pEvent
   * @param _pAttachToComponent
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlaySoundAttached(UFMODEvent* _pEvent, USceneComponent* _pAttachToComponent);

  /**
   * @brief
   * @param AudioComponent
   * @param Event
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayEventOnComponent(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent);
  /**
   * @brief
   * @param AudioComponent
   * @param Event
   * @param Parameters
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayEventWithParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters);
  /**
   * @brief
   * @param _pOwner
   * @param _pEvent
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayAttachedEvent(AActor* _pOwner, UFMODEvent* _pEvent);

  /**
   * @brief
   * @param _pOwner
   * @param _pEvent
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void SetComponentParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters);

  /**
   * @brief
   * @param AudioComponent
   * @param Event
   * @param Parameters
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayEventInstanceWithParameters(UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters);

  /**
   * @brief
   * @param AudioComponent
   * @param Event
   * @param Parameters
   */
  FMOD::Studio::EventInstance* PlayEventInstance(UFMODEvent* _pEvent);

  /**
   * @brief
   * @param AudioComponent
   * @param Event
   * @param Parameters
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void SetGlobalParameter(const TArray<FAudioParam>& _rParameters);

    /**
     * @brief 
     * @param _sParameterName 
     * @param _fCurrentValue 
     * @param _fTargetValue 
     * @param _fInterpolationSpeed 
     * @param _fDeltaTime 
     */
    UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void SetParameterInterpolated(UFMODAudioComponent* _pAudioComponent, FName _sParameterName, float& _fCurrentValue, float _fTargetValue, float _fInterpolationSpeed, float _fDeltaTime);
};
