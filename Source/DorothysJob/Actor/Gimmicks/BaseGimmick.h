// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "BaseGimmick.generated.h"

class UCapsuleComponent;
class UBaseRoom;

UCLASS()
class DOROTHYSJOB_API ABaseGimmick : public ABaseActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseGimmick();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Activate();

	virtual void GetHit();

protected:
	// Capsule component for the gimmicks
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* m_pCapsuleComponent;

	// Mesh for the gimmicks
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* m_pMeshComponent;

	virtual void GimmickActivation();

private:
	FTimerHandle RetryRegisterTimerHandle;

	void TryRegisterInRoom(UBaseRoom* Room);
};
