// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "MediaSoundComponent.h"
#include "CinematicScreen.generated.h"

class UMediaPlayer;
class UMediaSource;
class UImage;
class UCanvasPanel;
class UTextBlock;
class UInputIcon;
class USaveGameManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCinematicFinished);

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UCinematicScreen : public UBaseWidget
{
	GENERATED_BODY()

public:

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

  FOnCinematicFinished OnCinematicFinished;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13 | Video")
  TObjectPtr<UMediaPlayer> m_pMediaPlayer;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13 | Video")
  TObjectPtr<UMediaSource> m_pMediaSource;

  UMediaSoundComponent* m_pMediaSound;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pVideoImage;

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

  UFUNCTION()
  void PlayIntro();

private:

  bool m_bIsHolding = false;
  float m_fHoldStartTime = .0f;

  void OnDialogueSkipStarts();

  void OnDialogueSkipEnds();

  UFUNCTION()
  void OnVideoEnd();

  UFUNCTION()
  void OnMediaOpened(FString OpenedUrl);

  void SkipIntro();

  /**
  * @brief Reference to the SaveGame manager
  */
  TObjectPtr<USaveGameManager> m_pSaveGameManager;
};
