/************************************************************************
 * @description: Represents the Focusable Widgets in the User's Interface
 * @author: Jaime Paramo
 * @date: 20/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "FocusableWidget.generated.h"

// Manager
class USpecificFocusManager;

DECLARE_MULTICAST_DELEGATE(FMenuShow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMenuInteract);

UCLASS()
class DOROTHYSJOB_API UFocusableWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:

	FMenuShow OnMenuShow;

	/**
	 * @brief -> Displays the UMG elements of this Widget on screen.
	 */
	virtual void Show();
	/**
	 * @brief -> Hides the UMG elements of this Widget.
	 */
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

	/**
	 * @brief -> Sets up the focusable elements and specific focus managers of the widget.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetupFocus();

	/**
	 * @brief -> Returns the array of SpecificFocusManager references related to this widget. 
	 * @return -> Reference to the array of focus manager instances.
	 */
	virtual TArray<TObjectPtr<USpecificFocusManager>>& GetSpecificFocusManager();
	/**
	 * @brief -> Returns the array of focusable UI elements within the widget.
	 * @return -> Reference to the array of focusable elements.
	 */
	virtual TArray<TObjectPtr<UObject>>& GetFocusableElements();


  UPROPERTY(BlueprintAssignable, Category = "Delegates")
  FOnMenuInteract OnMenuOpened;

  UPROPERTY(BlueprintAssignable, Category = "Delegates")
  FOnMenuInteract OnMenuClosed;

protected:
  /**
   * @brief It will set the pause mode.
   * @param _bStatus -> Pause mode state.
   */
  void SetPauseMode(bool _bStatus);

	/**
	 * @brief -> List of focus manager objects responsible for managing specific focus areas.
	 */
	UPROPERTY()
	TArray<TObjectPtr<USpecificFocusManager>> m_pSpecificFocusManager;

	/**
	 * @brief -> List of UI elements that can receive focus.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UObject>> m_lFocusableElements;
};