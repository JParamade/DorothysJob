// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWidget.h"
#include "TextBox.generated.h"



class UTextBlock;

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UTextBox : public UBaseWidget
{
	GENERATED_BODY()

public:

  /**
   * @brief This is use to Print a text
   * @param _text The text you want to print
   */
  UFUNCTION(BlueprintCallable)
  void SetText(const FString& _text);

  UFUNCTION(BlueprintCallable)
  void SetKeyText(const FName& _idTable, const FString& _Key);

  /**
   * @brief The next letter in the text
   */
  void SetLetter();

  /**
   * @brief Text Box
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UTextBlock* m_currentTextBlock;

private:
  
  /**
   * @brief Text
   */
  FString m_text;
  /**
   * @brief The actual letter
   */
  int m_iCurrentLetter;
  /**
   * @brief The number of letters in the word
   */
  int m_iMaxLetters;

  UPROPERTY()
  FTimerHandle m_timeHanler;


  /**
   * @brief Time between letters
   */
  UPROPERTY(EditAnywhere)
  float m_fTimeBetweenLetters;

};
