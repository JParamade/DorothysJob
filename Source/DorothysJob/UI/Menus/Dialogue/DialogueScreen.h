/************************************************************************
 * @description: Represents the UMG class for the dialogue screen.
 * @author: Jaime Paramo
 * @date: 30/08/2025
 * @edited_by: Josephine Esposito - 07/09/2025
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "DialogueScreen.generated.h"

// Audio
class UFMODEvent;
namespace FMOD {
  namespace Studio {
    class EventInstance;
    class System;
  }
}

// Component
class UCanvasPanel;
class UImage;
class URichTextBlock;
class UTextBlock;

// Data
struct FDialogData;

// Manager
class UBaseGameInstance;

// Widget
class UInputIcon;


UCLASS()
class DOROTHYSJOB_API UDialogueScreen : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief Parent canvas panel component. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pParentCanvasPanel;

  /**
   * @brief Cinematic dark bars animation. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pDarkBarsAnimation;

  /**
   * @brief Left character focus animation. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pLeftCharacterTalks;
  /**
   * @brief Right character focus animation. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pRightCharacterTalks;

  /**
   * @brief Reference to the lower dark bar image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pLowerDarkBar;
  /**
   * @brief Reference to the higher dark bar image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pHigherDarkBar;
  /**
   * @brief Reference to the dark background image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pDarkBackground;

  /**
   * @brief Reference to the left character image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_bLeftCharacterImage;
  /**
   * @brief Reference to the right character image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_bRightCharacterImage;

  /**
   * @brief Reference to the text box image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pTextBoxImage;
  /**
   * @brief Reference to the text box. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<URichTextBlock> m_pTextBox;

  /**
   * @brief Reference to the left emitter image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pLeftEmitterImage;
  /**
   * @brief Reference to the left emitter text. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pLeftEmitterText;

  /**
   * @brief Reference to the right emitter image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pRightEmitterImage;
  /**
   * @brief Reference to the right emitter text. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pRightEmitterText;

  /**
   * @brief Reference to the button icon. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pButtonIcon;

  /**
   * @brief Reference to the skip button icon. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pSkipButtonIcon;
  /**
   * @brief Reference to the skip button progress bar. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSkipButtonPB;
  /**
   * @brief Reference to the skip text. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pSkipText;

  /**
   * @brief Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;
  /**
   * @brief Hides the UMG elements of this Widget.
   */
  virtual void Hide() override;

  /**
   * @brief Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief Called every frame.
   * @param _rMyGeometry Geometry information of the widget.
   * @param _fInDeltaTime Time since the last tick.
   */
  virtual void NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) override;

  /**
   * @brief Audio event for the typewritting sound.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Audio", meta = (DisplayName = "Typewritting FMOD Event"))
  TObjectPtr<UFMODEvent> m_pTypewrittingEvent;

private:
  /**
   * @brief Updates the dialogue info based on the current line data.
   * @param _rDialogueData Data of the current dialogue line, containing the text to show, the characters talking, their sprites, etc.
   */
  UFUNCTION()
  void UpdateDialogueInfo(FDialogData _oDialogueData);

  /**
   * @brief Updates the left character info based on the dialogue data.
   * @param _rDialogueData Data of the current dialogue line.
   * @param _bSpeaking Indicates if the character is speaking.
   */
  void UpdateLeftCharacterInfo(FDialogData _oDialogueData, bool _bSpeaking);
  /**
   * @brief Updates the right character info based on the dialogue data.
   * @param _rDialogueData Data of the current dialogue line.
   * @param _bSpeaking Indicates if the character is speaking.
   */
  void UpdateRightCharacterInfo(FDialogData _oDialogueData, bool _bSpeaking);

  /**
   * @brief Delegate handle for the dialogue line update event.
   */
  FDelegateHandle m_oDialogueHandle;

  /**
   * @brief Called when the dialogue skip starts.
   */
  UFUNCTION()
  void OnDialogueSkipStarts();

  /**
   * @brief Called when the dialogue skip ends.
   */
  UFUNCTION()
  void OnDialogueSkipEnds();

  /**
   * @brief Handles the dialogue line update.
   */
  UFUNCTION()
  void HandleDialogueLine();

  /**
   * @brief Sets the text for the dialogue box.
   * @param _rText The text to display.
   */
  UFUNCTION()
  void SetText(const FString& _rText);

  /**
   * @brief Sets the next letter in the dialogue text to create a typewritter effect.
   */
  void SetLetter();

  /**
   * @brief String containing the current text being shown in the dialogue box, used for the typewritter effect.
   */
  FString m_sCurrentText;
  /**
   * @brief Index of the current character being shown in the dialogue text, used for the typewritter effect.
   */
  int m_iCharacterIndex;
  /**
   * @brief Length of the current text being shown in the dialogue box, used for the typewritter effect.
   */
  int m_iTextLength;

  /**
   * @brief Timer handle for the typewritter effect, used to control the timing of showing each character in the dialogue text.
   */
  FTimerHandle m_oTimerHandle;

  /**
   * @brief Time between characters for the typewritter effect.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|UI", meta = (DisplayName = "Time Between Characters", AllowPrivateAccess = true))
  float m_fTimeBetweenChararacters = .01f;

  /**
   * @brief The time at which the last character was shown, used for the typewritter effect.
   */
  float m_fLastCharTime = 0;

  /**
   * @brief Indicates if the dialogue is currently being typed.
   */
  bool m_bIsTyping = false;
  /**
   * @brief Indicates if the dialogue is currently being held.
   */
  bool m_bIsHolding = false;
  /**
   * @brief The time at which the hold started, used for the typewritter effect.
   */
  float m_fHoldStartTime = .0f;

  /**
   * @brief Reference to the game instance, used to access the dialogues manager and other game-related information.
   */
  TObjectPtr<UBaseGameInstance> m_pGameInstance;

  /**
   * @brief Index of the current character talking.
   */
  int32 m_iCurrentCharacter;
  /**
   * @brief Index of the last character talking.
   */
  int32 m_iLastCharacter;

  /**
   * @brief Name of the current sprite being shown for the character.
   */
  FName m_sCurrentSprite;
  /**
   * @brief Name of the last sprite being shown for the character.
   */
  FName m_sLastSprite;

  /**
   * @brief Plays the typewritter audio event based on the current character talking.
   * @param _sName Name of the parameter to set in the FMOD event, used to differentiate between characters.
   * @param _fValue Value of the parameter to set in the FMOD event, used to differentiate between characters (e.g., 0 for Dorothy, 1 for Lord Kaios, 2 for Gorch).
   */
  void PlayTypewritterEvent(FName _sName, float _fValue);

  /**
   * @brief Plays the silent killer audio event.
   */
  void PlaySilentKillerEvent();
};