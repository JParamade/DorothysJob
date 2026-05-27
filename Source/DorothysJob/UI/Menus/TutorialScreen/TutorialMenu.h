// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../BaseWidget.h"
#include "Components/Image.h"

#include "TutorialMenu.generated.h"

class UCanvasPanel;
class UImage;
class UFMODEvent;
class UEventTutorialDataAsset;

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UTutorialMenu : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  
	virtual void NativeConstruct() override;

	virtual void Hide() override;
	virtual void Show() override;

	UFUNCTION()
	void SetVideo(FName _sId);

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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> m_pCanvasPanel;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UImage> m_pImage;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Weapon Reward FMOD Event"))
	TObjectPtr<UFMODEvent> m_pRewardEvent;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Lord Kaios Note FMOD Event"))
	TObjectPtr<UFMODEvent> m_pLKNoteEvent;

	/**
	 * @brief The list of permanent registered events
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13", meta = (DisplayName = "Permanent Events"))
	TMap<FName, TObjectPtr<UTexture2D>> m_lPermanentEventData;
};
