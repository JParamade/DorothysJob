/************************************************************************
 * @description: Represents the tip information class shown on loading screen.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "TipData.generated.h"

USTRUCT(BlueprintType)
struct FTipData : public FTableRowBase
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Tip Text"))
  FText m_sTipText;
};