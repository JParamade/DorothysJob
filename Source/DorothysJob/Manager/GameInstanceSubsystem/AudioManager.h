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
struct FAudioParam {
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
class DOROTHYSJOB_API UAudioManager : public UGameInstanceSubsystem {
  GENERATED_BODY()

public:
  /**
   * @brief Initializes the audio manager subsystem.
   * @param _rCollection The collection of subsystems to initialize.
   */
  virtual void Initialize(FSubsystemCollectionBase& _rCollection) override;

  /**
   * @brief Plays a 2D sound event.
   * @param _pWorldContextObject The world context object for the sound event.
   * @param _pEvent The FMOD event to play.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlaySound2D(UObject* _pWorldContextObject, UFMODEvent* _pEvent);
  /**
   * @brief Plays a sound event at a specific location in the world.
   * @param _pWorldContextObject The world context object for the sound event.
   * @param _pEvent The FMOD event to play.
   * @param _vLocation The location in the world where the sound should be played.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlaySoundAtLocation(UObject* _pWorldContextObject, UFMODEvent* _pEvent, FVector _vLocation);
  /**
   * @brief Plays a sound event attached to a component.
   * @param _pEvent The FMOD event to play.
   * @param _pAttachToComponent The component to attach the sound to.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlaySoundAttached(UFMODEvent* _pEvent, USceneComponent* _pAttachToComponent);

  /**
   * @brief Plays a sound event on a specific FMOD audio component.
   * @param _pAudioComponent The FMOD audio component to play the sound on.
   * @param _pEvent The FMOD event to play.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayEventOnComponent(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent);
  /**
   * @brief Plays a sound event on a specific FMOD audio component with parameters.
   * @param _pAudioComponent The FMOD audio component to play the sound on.
   * @param _pEvent The FMOD event to play.
   * @param _rParameters The parameters to set on the audio component before playing the sound.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayEventWithParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters);
  /**
   * @brief Plays a sound event attached to an actor.
   * @param _pOwner The actor to attach the sound to.
   * @param _pEvent The FMOD event to play.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayAttachedEvent(AActor* _pOwner, UFMODEvent* _pEvent);

  /**
   * @brief Sets parameters on a specific FMOD audio component for a given event.
   * @param _pAudioComponent The FMOD audio component to set the parameters on.
   * @param _pEvent The FMOD event associated with the audio component.
   * @param _rParameters The parameters to set on the audio component before playing the sound.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void SetComponentParameters(UFMODAudioComponent* _pAudioComponent, UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters);

  /**
   * @brief Plays a sound event with parameters and returns the FMOD event instance.
   * @param _pEvent The FMOD event to play.
   * @param _rParameters The parameters to set on the audio component before playing the sound.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void PlayEventInstanceWithParameters(UFMODEvent* _pEvent, const TArray<FAudioParam>& _rParameters);

  /**
   * @brief Plays a sound event and returns the FMOD event instance.
   * @param _pEvent The FMOD event to play.
   * @return The FMOD event instance.
   */
  FMOD::Studio::EventInstance* PlayEventInstance(UFMODEvent* _pEvent);

  /**
   * @brief Sets global parameters for the audio system.
   * @param _rParameters The parameters to set.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void SetGlobalParameter(const TArray<FAudioParam>& _rParameters);

  /**
   * @brief Sets a parameter on a specific FMOD audio component with interpolation.
   * @param _pAudioComponent The FMOD audio component to set the parameter on.
   * @param _sParameterName The name of the parameter to set.
   * @param _fCurrentValue The current value of the parameter.
   * @param _fTargetValue The target value of the parameter.
   * @param _fInterpolationSpeed The speed of interpolation.
   * @param _fDeltaTime The delta time for interpolation.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Audio")
  void SetParameterInterpolated(UFMODAudioComponent* _pAudioComponent, FName _sParameterName, float& _fCurrentValue, float _fTargetValue, float _fInterpolationSpeed, float _fDeltaTime);
};
