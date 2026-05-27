// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DissolvableActor.h"
#include "DissolvableWallStructure.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API ADissolvableWallStructure : public ADissolvableActor
{
	GENERATED_BODY()

	public:
		ADissolvableWallStructure();

protected:
	virtual void Hide() override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> BasicWalls;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedBasicWalls;

	TObjectPtr<UStaticMesh> OriginalBasicWalls;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> Columns;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedColumns;

	TObjectPtr<UStaticMesh> OriginalColumns;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> BasicWallPlinths;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedBasicWallPlinths;

	TObjectPtr<UStaticMesh> OriginalBasicWallPlinths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> ColumnPlinths;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedColumnPlinths;

	TObjectPtr<UStaticMesh> OriginalColumnPlinths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> Splits01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedSplits01;

	TObjectPtr<UStaticMesh> OriginalSplits01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> Split01Plinths;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedSplitsPlinths01;

	TObjectPtr<UStaticMesh> OriginalSplitPlinths01;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> Splits02;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedSplits02;

	TObjectPtr<UStaticMesh> OriginalSplits02;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> Split02Plinths;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedSplits02Plinths;

	TObjectPtr<UStaticMesh> OriginalSplitPlinths02;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> Outcorners;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedOutcorners;

	TObjectPtr<UStaticMesh> OriginalOutcorners;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> OutcornerPlinths;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|ChangedMeshes")
	TObjectPtr<UStaticMesh> ChangedOutcornerPlinths;

	TObjectPtr<UStaticMesh> OriginalOutcornerPlinths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> HideableProps;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
	TObjectPtr<USceneComponent> UnhideableProps;

private:
	TObjectPtr<UStaticMesh> GetMeshFromFolder(TObjectPtr<USceneComponent> Folder) const;
	
	void HideProps();

	void ChangeOutcorners();

	void ChangeSplits();

	void ChangeColumns();

	void ChangeBasicWalls();

	void Change(TObjectPtr<USceneComponent> Folder, TObjectPtr<UStaticMesh> ChangedSM = nullptr, TObjectPtr<UStaticMesh> OriginalSM = nullptr);
};
