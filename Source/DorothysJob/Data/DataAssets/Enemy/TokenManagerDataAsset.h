// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TokenManagerDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FToken
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Max number of tokens"))
  int m_iMaxTokens = 1;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Token Time Cooldown"))
  float m_fRestoreTime = 1.f;
};


UCLASS()
class DOROTHYSJOB_API UTokenManagerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
  int GetMaxToken(int _iIndex) const;

  int GetTokenTime(int _iIndex) const;

  UPROPERTY(EditAnywhere, Category = "Bola 13|General|Tokens", meta = (DisplayName = "Tokens Paremeters"))
  TArray<FToken> m_lTokenParameters;
};
