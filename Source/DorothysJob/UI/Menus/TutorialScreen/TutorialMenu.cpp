// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialMenu.h"
#include "DorothysJob/Controller/BasePlayerController.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Data/DataAssets/Tutorial/EventTutorialDataAsset.h"
#include "FMODEvent.h"

void UTutorialMenu::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pCanvasPanel)) m_lElements.Add(m_pCanvasPanel);
  if (IsValid(m_pLowerDarkBar)) m_lElements.Add(m_pLowerDarkBar);
  if (IsValid(m_pHigherDarkBar)) m_lElements.Add(m_pHigherDarkBar);
  if (IsValid(m_pImage)) m_lElements.Add(m_pImage);

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (pGameInstance)
  {
    UEventManager* pEventManager = pGameInstance->GetEventManager();
    pEventManager->OnPermanentEventShow.AddUObject(this, &UTutorialMenu::SetVideo);
  }
}

void UTutorialMenu::Hide()
{
  Super::Hide();
}

void UTutorialMenu::Show()
{
  Super::Show();
}

void UTutorialMenu::SetVideo(FName _sId)
{
  if (!m_lPermanentEventData.Contains(_sId))
  {
    return;
  }

  UTexture2D* pImage = m_lPermanentEventData[_sId];
  if (!pImage)
  {
    return;
  }

  FString IdStr = _sId.ToString();
  FString Left, Right, Second, Third;
  FString Prefix;

  if (IdStr.Split(TEXT("_"), &Left, &Right))
  {
    if (Right.Split(TEXT("_"), &Second, &Third))
    {
      Prefix = Left + TEXT("_") + Second; // "GIF_OB"
    }
    else
    {
      Prefix = Left; // "GIF" 
    }
  }
  else
  {
    Prefix = IdStr; // fallback, en caso de no haber "_"
  }

  if (!IsValid(m_pImage))
  {
    return;
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, Prefix == "GIF_OB" ? m_pRewardEvent : m_pLKNoteEvent);
    }
  }

  m_pImage->SetBrushFromTexture(pImage);
}