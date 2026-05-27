// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Trigger.h"
#include "CollisionTrigger.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API ACollisionTrigger : public ATrigger
{
	GENERATED_BODY()
	
public:

	ACollisionTrigger();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


private:

	UPROPERTY(EditAnywhere, Category = "Bola 13|Data")
	bool m_bOnTriggerExit;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UShapeComponent> m_pCollision;
};
