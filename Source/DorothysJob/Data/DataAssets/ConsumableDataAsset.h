/************************************************************************
 * @description: Represents the consumable Data Asset.
 * @author: Jaime Paramo
 * @date: 05/07/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ConsumableDataAsset.generated.h"

class UBaseConsumable;
class ABaseConsumableActor;

UENUM()
enum class EConsumableType : uint8
{
	DURABLE	  UMETA(DisplayName = "Durable"),
	ONESHOT   UMETA(DisplayName = "One Shot"),
	SPAWNABLE UMETA(DisplayName = "Spawnable")
};

UENUM()
enum class EConsumableClass : uint8
{
	DOGROTHY			UMETA(DisplayName = "Dogrothy"),
	MRSCLEAN			UMETA(DisplayName = "Mrs Clean"),
	CLEAN4				UMETA(DisplayName = "Clean-4"),
	B13						UMETA(DisplayName = "B13"),
	PRISTINETIME  UMETA(DisplayName = "Pristine Time")
};

UCLASS()
class DOROTHYSJOB_API UConsumableDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Name"))
	FText m_sName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Consumable Type"))
	EConsumableType m_eConsumableType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Consumable Class"))
	EConsumableClass m_eConsumableClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Description"))
	FText m_sDescription;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Stack Quantity"))
	int32 m_iStackQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Duration"))
	int32 m_iDuration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Amount Added When Reward", Tooltip = "This is the amount added to this consumable when the player gets a reward after cleaning a Greenhouse.", ClampMin = "1"))
	int32 m_iAmountAddedWhenReward = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Consumable Class"))
	TSubclassOf<UBaseConsumable> m_oConsumableClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Consumable Actor"))
	TSubclassOf<ABaseConsumableActor> m_oConsumableActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Icon"))
	UTexture2D* m_pIcon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Focus Banner"))
	UTexture2D* m_pFocusBanner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Little Banner"))
	UTexture2D* m_pLittleBanner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumable", meta = (DisplayName = "Short Info"))
	FText m_sShortInfo;
};