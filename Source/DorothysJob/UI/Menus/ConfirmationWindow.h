// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "ConfirmationWindow.generated.h"

/**
 * 
 */
class UBaseButton;
class UCanvasPanel;
class UImage;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnConfirmDelegate, bool);

UCLASS()
class DOROTHYSJOB_API UConfirmationWindow : public UFocusableWidget
{
	GENERATED_BODY()
	
public:

#pragma region | Virtual Methods
  /**
   * @brief The default Constructor of the widget.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief This will activate the widget.
   */
  virtual void Show() override;

  /**
   * @brief This will deactivate the widget.
   */
  virtual void Hide() override;

  /**
   * @brief -> Configures the focus system for the current menu when it's opened.
   */
  virtual void SetupFocus() override;

#pragma endregion

  FOnConfirmDelegate OnConfirmResult;

  /**
   * Title text for the confirmation window.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Title"))
  FText m_sConfirmationTitleText;

protected:

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UBaseButton> m_bAcceptButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UBaseButton> m_bCancelButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pBackgroundImage;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pConfirmationText;

private:

  UFUNCTION()
  void Accept();

  UFUNCTION()
  void Cancel();
};
