/************************************************************************
 * @description: It represents the replay menu.
 * @author: David Gonzalez
 * @date: 20/10/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "ReplayMenu.generated.h"

class UCanvasPanel;
class UGridPanel;
class UBaseButton;
class UImage;
class UInputIcon;
class UTextBlock;
class UConfirmationWindow;
class UReplayResultsWidget;

UCLASS()
class DOROTHYSJOB_API UReplayMenu : public UFocusableWidget
{
	GENERATED_BODY()
	
public:

  /**
   * @brief The default Constructor of the widget.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief The default destructor of the widget.
   */
  virtual void NativeDestruct() override;

  /**
   * @brief This will activate the widget.
   */
  virtual void Show() override;

  /**
   * @brief This will deactivate the widget.
   */
  virtual void Hide() override;

  /**
   * @brief This will setup the focus for the buttons.
   */
  virtual void SetupFocus() override;

  /**
   * @brief It hides the replay menu.
   */
  void CancelAction();

private:

  /**
   * @brief It will manage the button clicked to open the _iLevel.
   * @param _iLevel -> Level to open.
   */
  UFUNCTION()
  void OnLevelClicked(int32 _iLevel);

  /**
   * @brief It initialises the focus manager.
   */
  void InitFocusManager();

  /**
   * @brief It shows the available score levels in the menu.
   */
  void ShowScores();

  /**
   * The unlocked slate style asset for the level buttons.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|UI", meta = (DisplayName = "Level Unlocked Style", AllowPrivateAccess = "true"))
  TObjectPtr<const USlateWidgetStyleAsset> m_pUnlockedButtonStyleAsset;

  /**
   * The custom slate style asset for the level buttons.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|UI", meta = (DisplayName = "Level Score Styles", AllowPrivateAccess = "true"))
  TArray<TObjectPtr<const USlateWidgetStyleAsset>> m_lScoreButtonStyleAsset;

  /**
   * The unlocked slate style asset for the endless button.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|UI", meta = (DisplayName = "Endless Unlocked Style", AllowPrivateAccess = "true"))
  TObjectPtr<const USlateWidgetStyleAsset> m_pUnlockedEndlessButtonStyleAsset;

  /**
   * The custom slate style asset for the endless button.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|UI", meta = (DisplayName = "Endless Score Styles", AllowPrivateAccess = "true"))
  TArray<TObjectPtr<const USlateWidgetStyleAsset>> m_lEndlessScoreButtonStyleAsset;

  

  /**
   * The levels title to show.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Levels Title", AllowPrivateAccess = "true"))
  FText m_sLevelsTitle;

  /**
   * This represents the widget main panel.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  /**
   * This represents the widget main panel.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UGridPanel> m_pLevelPanel;

  /**
   * This represents the exit button icon.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pExitIconButton;

  /**
   * This represents the text associated to the exit button icon.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pExitText;

  /**
   * It is the confirmation windows to confirm the selected language.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UConfirmationWindow> m_pConfirmationWindow;

  /**
   * The level button list.
   */
  TArray<TObjectPtr<UBaseButton>> m_lLevelButton;

  /**
   * The level score widgets.
   */
  TArray<TObjectPtr<UReplayResultsWidget>> m_lLevelScore;
};
