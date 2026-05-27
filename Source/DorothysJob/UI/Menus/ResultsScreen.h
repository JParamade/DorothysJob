/************************************************************************
 * @description: It represents the results screen for win / game over.
 * @author: David Gonzalez
 * @date: 02/04/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "ResultsScreen.generated.h"

class UTextBlock;
class UImage;
class UCanvasPanel;
class UReloadLevelButton;
class UBaseButton;
class USpacer;
class USaveGameManager;
enum class EBaseGameState : uint8;

UCLASS()
class DOROTHYSJOB_API UResultsScreen : public UFocusableWidget
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
   * @brief This will setup the focus for the buttons.
   */
  virtual void SetupFocus() override;
#pragma endregion

  /**
   * @brief It sets the level results state.
   * @param _eState -> Game state to show (WIN or GAMEOVER)
   */
  void SetWidgetState(EBaseGameState _eState);

protected:

#pragma region | OnClick Functions
  /**
   * @brief This method will load the next level/panel.
   */
  UFUNCTION(BlueprintCallable)
  void OnClickContinue();
#pragma endregion

private:

  /**
   * @brief It recover and shows the level results.
   */
  void ShowResults();

#pragma region | Widget Parameters

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Win Title", AllowPrivateAccess = "true"))
  FText m_sWinTitle;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Game Over Title", AllowPrivateAccess = "true"))
  FText m_sGameOverTitle;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Endless Title", AllowPrivateAccess = "true"))
  FText m_sEndlessTitle;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "First stat", AllowPrivateAccess = "true"))
  FText m_sFirstStatText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Second stat", AllowPrivateAccess = "true"))
  FText m_sSecondStatText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Third stat", AllowPrivateAccess = "true"))
  FText m_sThirdStatText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Endless First stat", AllowPrivateAccess = "true"))
  FText m_sEndlessFirstStatText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Endless Second stat", AllowPrivateAccess = "true"))
  FText m_sEndlessSecondStatText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Endless Third stat", AllowPrivateAccess = "true"))
  FText m_sEndlessThirdStatText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Dorothy Win", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pDorothyWin;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Dorothy Game Over", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pDorothyGameOver;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Star Empty", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pStarEmptyImage;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Star Full", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pStarFullImage;

  /**
   * Hub map used to load the level after the win screen.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Maps", meta = (DisplayName = "HUB", AllowPrivateAccess = "true"))
  TSoftObjectPtr<UWorld> m_pHubMap;

  /**
   * @brief This represents the widget main panel.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  /**
   * @brief This represents the title of the panel.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pTitle;

  /**
   * This is the first results text.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pResultDetailsText1;

  /**
   * This is the second results text.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pResultDetailsText2;

  /**
   * This is the third results text.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pResultDetailsText3;

  /**
   * @brief The level results.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USpacer> m_pButtonSpacer;

  /**
   * @brief It is to continue to the next level when the player wins
   *        and return to hub when the player loses.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pContinueButton;

  /**
   * @brief It is to retry the current level when the player loses.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UReloadLevelButton> m_pRetryButton;

  /**
   * @brief It is the Dorothy image in the screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pDorothyImage;

  /**
   * @brief It is the first star image in the screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pFirstStarImage;

  /**
   * @brief It is the first star image in the screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondStarImage;

  /**
   * @brief It is the score image in the screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pScoreImage;

  /**
   * @brief It is the fade in animation.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeInAnimation;

  /**
   * @brief Widget state to show.
   */
  EBaseGameState m_eState;

  UPROPERTY()
  TObjectPtr<USaveGameManager> m_pSaveGameManager;
#pragma endregion
};