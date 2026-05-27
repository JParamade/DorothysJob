// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericGimmick.h"
#include "CleaningGimmick.generated.h"


class UDrawing;
/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API ACleaningGimmick : public AGenericGimmick
{
	GENERATED_BODY()

public:

	ACleaningGimmick();

	virtual void GetHit() override;

private:

	UPROPERTY(EditAnywhere, Category = "Cleaning", meta = (DisplayName = "Drawing Component"))
	UDrawing* m_pDrawing;

	/**
	 * @brief SIze of the texture to clean
	 */
	UPROPERTY(EditAnywhere, Category = "Cleaning", meta = (DisplayName = "Cleaning size"))
	FVector2D m_v2SCleaningSize;

	/**
	 * @brief If this component Cleans it will clean the zone determined by this texture
	 */
	UPROPERTY()
	UTexture* m_pCleaningTexture;

	/**
	 * @brief Pivot in the texture cleaning
	 */
	UPROPERTY(EditAnywhere, Category = "Cleaning", meta = (DisplayName = "Cleaning pivot"))
	FVector2D m_v2SCleaningPivot;

	/**
	 * @brief Angle in cleaning
	 */
	UPROPERTY(EditAnywhere, Category = "Cleaning", meta = (DisplayName = "Cleaning angle"))
	float m_fCleaningAngle;
};
