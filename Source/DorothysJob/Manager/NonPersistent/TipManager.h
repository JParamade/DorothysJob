/************************************************************************
 * @description: Represents the manager class for tip handling in the loading screen.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "TipManager.generated.h"

UCLASS()
class DOROTHYSJOB_API UTipManager : public UObject {
	GENERATED_BODY()
	
public:
  /**
   * @brief Initializes the tip manager with a DataTable containing all available tips.
   * @param _pTipDT The DataTable containing the tips.
   */
  void Initialize(UDataTable* _pTipDT);

  /**
   * @brief Retrieves the next tip from the available tips list.
   * @return The next tip as a FText object.
   */
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Tips", meta = (DisplayName = "Get Next Tip"))
  FText GetNextTip();

private:
  /**
   * @brief DataTable containing all available tips.
   */
  UPROPERTY()
  TObjectPtr<UDataTable> m_pTipTable;

  /**
   * @brief List of remaining tips to display before refilling from the DataTable.
   */
  UPROPERTY()
  TArray<FName> m_pAvailableTips;
};
