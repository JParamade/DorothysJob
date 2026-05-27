// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseActor.h"
#include "TutorialArrow.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API ATutorialArrow : public ABaseActor
{
	GENERATED_BODY()

public:

	ATutorialArrow();

	virtual void BeginPlay();

	void Active();

	void DesActive();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Mesh", meta = (DisplayAfter = "Mesh Component"))
	TObjectPtr<UStaticMeshComponent> m_pMeshComponent;

private:

	UPROPERTY(EditAnywhere, Category = "Bola13|Arrow Data", meta = (DisplayName = "Arrow Id"))
	FName m_sId;

public:

	FName GetArrowId();
};
