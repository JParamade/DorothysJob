// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Interactive/Interactable/InteractableComponent.h"
#include "InteractableWeaponChest.generated.h"

UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UInteractableWeaponChest : public UInteractableComponent
{
	GENERATED_BODY()
	
public:
	UInteractableWeaponChest();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;
};