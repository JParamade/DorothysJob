/************************************************************************
 * @description: Represents the widget for the mouse indicator
 * @author: Josephine Esposito
 * @date: 09/07/2025
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "MouseIndicator.generated.h"

#pragma region | Forward Declaration
class UImage;
#pragma endregion

/**
 * Represents the widget for the mouse indicator
 */
UCLASS()
class DOROTHYSJOB_API UMouseIndicator : public UBaseWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief The override of the show method
	 */
	virtual void Show() override;

	/**
	 * @brief The override of the hide method
	 */
	virtual void Hide() override;

	/**
	 * @brief The icon of the indicator
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pIcon;
};
//EOF