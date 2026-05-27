/************************************************************************
 * @description: Represents the structure for the info that appears in the menu
 * @author: Josephine Esposito
 * @date: 19/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "ConsumablesMenuData.generated.h"

enum class EConsumableClass : uint8;

/**
 * Struct for the consumable info in the menu
 */
USTRUCT(BlueprintType)
struct FConsumablesMenuData : public FTableRowBase
{
  GENERATED_BODY()
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Consumable type"))
  EConsumableClass m_eConsumableType;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Info text", MultiLine = true))
  TArray<FText> m_sInfo;

};
