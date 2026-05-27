// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraShakeHandlerComponent.generated.h"

class UMainLegacyCameraShake;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API UCameraShakeHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraShakeHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  /** Duración del shake */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake Params")
  float Duration;

  /** Amplitud de rotación en Pitch */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake Params")
  float PitchAmplitude;

  /** Amplitud de traslación en Z */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake Params")
  float LocationZAmplitude;

  /** Frecuencia de rotación en Pitch */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake Params")
  float PitchFrequency;

  /** Frecuencia de traslación en Z */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shake Params")
  float LocationZFrequency;

  /** Iniciar shake usando los parámetros actuales */
  UFUNCTION(BlueprintCallable, Category = "Shake Control")
  void PlayCameraShake(float Scale = 1.0f);

  /** Detener el shake */
  UFUNCTION(BlueprintCallable, Category = "Shake Control")
  void StopCameraShake();

  /** Detener el shake */
  UFUNCTION(BlueprintCallable, Category = "Shake Control")
  void SetCameraShake(TSubclassOf<UMainLegacyCameraShake> _ShakeClass);

private:

  UPROPERTY(EditAnywhere, Category = "Camera Shake")
  TSubclassOf<UMainLegacyCameraShake> ShakeClass;

  UPROPERTY()
  UMainLegacyCameraShake* ShakeInstance;

public:

#pragma region Getters&Setters
  UMainLegacyCameraShake* GetShakeIstance();
#pragma endregion
		
};
