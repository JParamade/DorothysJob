// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"

#include "CleaningInParts.generated.h"

class UDrawing;

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API ACleaningInParts : public ABaseActor
{
	GENERATED_BODY()

public:
  ACleaningInParts();

  void Init(TArray<UTexture*> _textures, float _duration, float _rotation, FVector2D _size, FVector2D _pivot);


private:

  void CleanPart();

#pragma region Components

  TObjectPtr<UDrawing> m_pDrawing;
  TObjectPtr<USceneComponent> m_pSceneComponent;

#pragma endregion



  TArray<UTexture*> m_oTexture;
  float m_fDuration;
  float m_fRotation;
  FVector2D m_vPivot;
  FVector2D m_vSize;
  int m_iIterador;
};
