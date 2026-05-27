// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Interactive/Interactable/InteractableComponent.h"
#include "InteractableReplayDesk.generated.h"

class UBaseGameInstance;

UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UInteractableReplayDesk : public UInteractableComponent
{
	GENERATED_BODY()
	
public:

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

	virtual void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
	UPROPERTY()
	TObjectPtr<UBaseGameInstance> m_pGameInstance;
};
