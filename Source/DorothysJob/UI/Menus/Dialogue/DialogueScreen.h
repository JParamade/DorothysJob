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
   * Parent canvas panel component.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pParentCanvasPanel;
  
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pDarkBarsAnimation;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pLeftCharacterTalks;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pRightCharacterTalks;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pLowerDarkBar;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pHigherDarkBar;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pDarkBackground;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_bLeftCharacterImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_bRightCharacterImage;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pTextBoxImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<URichTextBlock> m_pTextBox;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pLeftEmitterImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pLeftEmitterText;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pRightEmitterImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pRightEmitterText;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pButtonIcon;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pSkipButtonIcon;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSkipButtonPB;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pSkipText;

  /**
   * @brief -> Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;
  /**
   * @brief -> Hides the UMG elements of this Widget.
   */
  virtual void Hide() override;

  /**
   * @brief -> Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief -> @TOFILL. 
   */
  virtual void NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) override;

  /**
   * @brief 
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Audio", meta = (DisplayName = "Typewritting FMOD Event"))
  TObjectPtr<UFMODEvent> m_pTypewrittingEvent;

private:
  /**
   * @brief 
   * @param _rDialogueData 
   */
  UFUNCTION()
  void UpdateDialogueInfo(FDialogData _oDialogueData);

  /**
   * @brief 
   * @param _bSpeaking 
   */
  void UpdateLeftCharacterInfo(FDialogData _oDialogueData, bool _bSpeaking);
  /**
   * @brief
   * @param _bSpeaking
   */
  void UpdateRightCharacterInfo(FDialogData _oDialogueData, bool _bSpeaking);

  /**
   * @brief -> @TOFILL
   */
  FDelegateHandle m_oDialogueHandle;

  /**
   * @brief
   */
  UFUNCTION()
  void OnDialogueSkipStarts();

  /**
   * @brief
   */
  UFUNCTION()
  void OnDialogueSkipEnds();

  /**
   * @brief 
   */
  UFUNCTION()
  void HandleDialogueLine();

  /**
   * @brief 
   * @param _text 
   */
  UFUNCTION()
  void SetText(const FString& _rText);

  /**
   * @brief 
   */
  void SetLetter();

  /**
   * @brief -> @TOFILL
   */
  FString m_sCurrentText;
  /**
   * @brief -> @TOFILL
   */
  int m_iCharacterIndex;
  /**
   * @brief -> @TOFILL
   */
  int m_iTextLength;
  
  /**
   * @brief -> @TOFILL
   */
  FTimerHandle m_oTimerHandle;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|UI", meta = (DisplayName = "Time Between Characters", AllowPrivateAccess = true))
  float m_fTimeBetweenChararacters = .01f;

  float m_fLastCharTime = 0;

  bool m_bIsTyping = false;

  bool m_bIsHolding = false;
  float m_fHoldStartTime = .0f;

  /**
   * @brief The reference of the game instance
   */
  TObjectPtr<UBaseGameInstance> m_pGameInstance;

  /**
   * @brief 
   */
  int32 m_iCurrentCharacter;
  /**
   * @brief 
   */
  int32 m_iLastCharacter;

  /**
   * @brief
   */
  FName m_sCurrentSprite;
  /**
   * @brief
   */
  FName m_sLastSprite;

  /**
   * @brief 
   * @param _sName 
   * @param _fValue 
   */
  void PlayTypewritterEvent(FName _sName, float _fValue);

  /**
   * @brief 
   */
  void PlaySilentKillerEvent();
};