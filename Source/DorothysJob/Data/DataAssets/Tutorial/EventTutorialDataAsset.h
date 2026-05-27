/************************************************************************
 * @description: Represents the structure for the pop-up events
 * @author: Josephine Esposito
 * @date: 04/09/2025
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EventTutorialDataAsset.generated.h"

class UFocusableWidget;
//enum class EEventType : uint8;

USTRUCT(BlueprintType)
struct FEventData
{
	GENERATED_BODY()

public:

	/**
	 * @brief The text to display in the pop-up
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Event", meta = (DisplayName = "Text", MultiLine = true))
	FText m_sText;

	/**
	 * @brief The duration of the pop-up in seconds for the timer
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Event", meta = (DisplayName = "Duration"))
	float m_fDuration = 0.0f;
};

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UEventTutorialDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief The list of temporal registered events
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13", meta = (DisplayName = "Temporal Events"))
	TMap<FName, FEventData> m_lTemporalEventData;
};
