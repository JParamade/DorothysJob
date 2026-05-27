// Fill out your copyright notice in the Description page of Project Settings.
#include "TextBox.h"

#include "UMG.h"

void UTextBox::SetText(const FString& _text)
{
  m_iCurrentLetter = 0;
  m_iMaxLetters = _text.Len();
  m_text = _text;

  GetGameInstance()->GetTimerManager().SetTimer(m_timeHanler, this, &UTextBox::SetLetter, m_fTimeBetweenLetters, true);
}

void UTextBox::SetKeyText(const FName& _idTable, const FString& _Key)
{
  FTextKey textkey = _Key;
  FText text = FText::FromStringTable(_idTable, textkey);
  FString StringText = text.ToString();


  m_iCurrentLetter = 0;
  m_iMaxLetters = StringText.Len();
  m_text = StringText;


  GetGameInstance()->GetTimerManager().SetTimer(m_timeHanler, this, &UTextBox::SetLetter, m_fTimeBetweenLetters, true);
}

void UTextBox::SetLetter()
{
  //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, FString::Printf(TEXT("MaxLetter: %i, CurrentLetter: %i"), m_iMaxLetters, m_iCurrentLetter));   Dejar para debug
  FText text;
  FString string = m_text;
  int index = m_iCurrentLetter;
  int value = m_iMaxLetters - m_iCurrentLetter;

  string.RemoveAt(index, value, EAllowShrinking::No);

  text = text.FromString(string);

  m_currentTextBlock->SetText(text);
  //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Blue, string); Dejar para debug

  m_iCurrentLetter++;
  if (m_iCurrentLetter > m_iMaxLetters)
  {

    GetGameInstance()->GetTimerManager().ClearTimer(m_timeHanler);
  }
}
