/************************************************************************
 * @description: Represents the Base Widget in the User's Interface
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Jaime Paramo - 20/04/25
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

UCLASS()
class DOROTHYSJOB_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief -> Displays the UMG elements of this Widget on screen.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Show();
	/**
	 * @brief -> Hides the UMG elements of this Widget.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Hide();

	/**
	 * @brief -> Called when the widget is constructed.
	 */
	virtual void NativeConstruct() override;
	/**
	 * @brief -> Called every frame. Can be used for animations, updates, etc.
	 * @param _rMyGeometry -> Geometry information of the widget.
	 * @param _fDeltaTime -> Time since the last tick.
	 */
	virtual void NativeTick(const FGeometry& _rMyGeometry, float _fDeltaTime) override;

  UFUNCTION(BlueprintCallable)
	bool IsWidgetVisible();

protected:
	/**
	 * @brief -> List of non focusable UI elements contained in the widget.
	 */
	TArray<TObjectPtr<UObject>> m_lElements;

	bool m_bIsVisible = false;
};