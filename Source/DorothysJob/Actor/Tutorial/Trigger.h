// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseActor.h"
#include "DorothysJob/Data/DataAssets/Tutorial/EventListDataAsset.h"
#include "Trigger.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerEnterSignature, FName, TriggerId);

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API ATrigger : public ABaseActor
{
	GENERATED_BODY()

public:
	static FOnTriggerEnterSignature OnTriggerEnter;


protected:

	void ActivateTrigger();
	void ResetTrigger();

private:

	UPROPERTY(EditAnywhere, Category = "Bola 13|Data")
	FName m_sTriggerId;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Data")
	int32 m_iCurrentLevel;

	bool m_bIsActive = true;

};
