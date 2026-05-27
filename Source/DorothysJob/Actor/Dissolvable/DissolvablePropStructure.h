// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DissolvableActor.h"
#include "DissolvablePropStructure.generated.h"

UCLASS()
class DOROTHYSJOB_API ADissolvablePropStructure : public ADissolvableActor
{
	GENERATED_BODY()
	
public:
	ADissolvablePropStructure();

protected:
	virtual void Hide() override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> HideableProps;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> UnhideableProps;

};
