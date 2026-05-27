/************************************************************************
 * @description: Represents the UMG class for the dialogue screen.
 * @author: Jaime Paramo
 * @date: 30/08/2025
 * @edited_by: Josephine - 07/09/2025
 ************************************************************************/

#include "DialogueScreen.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"

// Data
#include "DorothysJob/Data/DataTables/DialogData.h"
#include "DorothysJob/Data/DataAssets/Dialogs/SpriteDataAsset.h"

// Gameplay
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/Dialogs/DialogsManager.h"

// Widget
#include "DorothysJob/UI/Utils/InputIcon.h"

void UDialogueScreen::Show() {
  
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UDialogsManager* pDialogueManager = pGameInstance->GetDialoguesManager()) {
      m_oDialogueHandle = pDialogueManager->OnLineStarted.AddUObject(this, &UDialogueScreen::HandleDialogueLine);
    }
  }

  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    pPlayerController->m_oOnDialogueSkipStarts.BindUObject(this, &UDialogueScreen::OnDialogueSkipStarts);
    pPlayerController->m_oOnDialogueSkipEnds.BindUObject(this, &UDialogueScreen::OnDialogueSkipEnds);
  }

  if (IsValid(m_pButtonIcon)) m_pButtonIcon->Show();
  if (IsValid(m_pSkipButtonIcon)) m_pSkipButtonIcon->Show();

  Super::Show();
}

void UDialogueScreen::Hide() {
  Super::Hide();

  // Remove bindings.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UDialogsManager* pDialogueManager = pGameInstance->GetDialoguesManager()) {
      pDialogueManager->OnLineStarted.Remove(m_oDialogueHandle);
    }
  }
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    pPlayerController->m_oOnDialogueSkipStarts.Unbind();
    pPlayerController->m_oOnDialogueSkipEnds.Unbind();
  }

  if (IsValid(m_pButtonIcon)) m_pButtonIcon->Hide();
  if (IsValid(m_pSkipButtonIcon)) m_pSkipButtonIcon->Hide();
}

void UDialogueScreen::NativeConstruct() {
  Super::NativeConstruct();

  m_pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
  if (!IsValid(m_pGameInstance)) {
    UE_LOG(LogTemp, Warning, TEXT("Game Instance not found."));
    m_pGameInstance = nullptr;
  }

  // Canvas Panel
  if (IsValid(m_pParentCanvasPanel)) m_lElements.Add(m_pParentCanvasPanel);

  // Dark Bars
  if (IsValid(m_pLowerDarkBar)) m_lElements.Add(m_pLowerDarkBar);
  if (IsValid(m_pHigherDarkBar)) m_lElements.Add(m_pHigherDarkBar);

  // Background
  if (IsValid(m_pDarkBackground)) m_lElements.Add(m_pDarkBackground);

  // Dialogue
  if (IsValid(m_bLeftCharacterImage)) m_lElements.Add(m_bLeftCharacterImage);
  if (IsValid(m_pLeftEmitterText)) m_lElements.Add(m_pLeftEmitterText);
  if (IsValid(m_pLeftEmitterImage)) m_lElements.Add(m_pLeftEmitterImage);
  if (IsValid(m_bRightCharacterImage)) m_lElements.Add(m_bRightCharacterImage);
  if (IsValid(m_pRightEmitterText)) m_lElements.Add(m_pRightEmitterText);
  if (IsValid(m_pRightEmitterImage)) m_lElements.Add(m_pRightEmitterImage);
  if (IsValid(m_pTextBoxImage)) m_lElements.Add(m_pTextBoxImage);
  if (IsValid(m_pTextBox)) m_lElements.Add(m_pTextBox);

  // Skip Info
  if (IsValid(m_pSkipText)) m_lElements.Add(m_pSkipText);
}

void UDialogueScreen::NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) {
  Super::NativeTick(_rMyGeometry, _fInDeltaTime);

  if (m_bIsTyping) {
    double dCurrentTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
    if (dCurrentTime - m_fLastCharTime >= m_fTimeBetweenChararacters) {
      m_fLastCharTime = dCurrentTime;
      SetLetter();
    }
  }

  if (m_bIsHolding && m_fHoldStartTime > .0f) {
    float fElapsedTime = UGameplayStatics::GetRealTimeSeconds(GetWorld()) - m_fHoldStartTime;
    float fPercent = FMath::Clamp(fElapsedTime, .0f, 1.f);

    if (IsValid(m_pSkipButtonPB)) m_pSkipButtonPB->GetDynamicMaterial()->SetScalarParameterValue(TEXT("fPercent"), fPercent);
  }
}

void UDialogueScreen::UpdateDialogueInfo(FDialogData _oDialogueData) {
  m_iCurrentCharacter = _oDialogueData.m_iCharacterTalking;
  m_sCurrentSprite = _oDialogueData.m_iCharacterTalking == 1 ? _oDialogueData.m_sLeftCharacterSprite : _oDialogueData.m_sRightCharacterSprite;

  // Update Info
  UpdateLeftCharacterInfo(_oDialogueData, _oDialogueData.m_iCharacterTalking == 1 ? true : false);
  UpdateRightCharacterInfo(_oDialogueData, _oDialogueData.m_iCharacterTalking == 2 ? true : false);

  m_iLastCharacter = m_iCurrentCharacter;
  m_sLastSprite = m_sCurrentSprite;

  if (_oDialogueData.m_iCharacterTalking == 1) {
    if (_oDialogueData.m_sLeftCharacterName == "Dorothy") PlayTypewritterEvent("typewriter_characterIndex", .0f);
    else if (_oDialogueData.m_sLeftCharacterName == "Lord Kaios") PlayTypewritterEvent("typewriter_characterIndex", 1.f);
    else if (_oDialogueData.m_sLeftCharacterName == "Gorch") PlayTypewritterEvent("typewriter_characterIndex", 2.f);
  }
  else if (_oDialogueData.m_iCharacterTalking == 2) {
    if (_oDialogueData.m_sRightCharacterName == "Dorothy") PlayTypewritterEvent("typewriter_characterIndex", .0f);
    else if (_oDialogueData.m_sRightCharacterName == "Lord Kaios") PlayTypewritterEvent("typewriter_characterIndex", 1.f);
    else if (_oDialogueData.m_sRightCharacterName == "Gorch") PlayTypewritterEvent("typewriter_characterIndex", 2.f);
  }

  // Dialogue Text
  SetText(_oDialogueData.m_sDialogText.ToString());
}

void UDialogueScreen::UpdateLeftCharacterInfo(FDialogData _oDialogueData, bool _bSpeaking) {
  if (!IsValid(m_pGameInstance)) {
    UE_LOG(LogTemp, Error, TEXT("GameInstance is nullptr."));
    return;
  }

  if (IsValid(m_pLeftCharacterTalks) && m_iLastCharacter != m_iCurrentCharacter) {
    PlayAnimation(
      m_pLeftCharacterTalks,
      0.f,
      1,
      m_iCurrentCharacter == 1 ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
  }

  if (_bSpeaking && m_sLastSprite != m_sCurrentSprite) {
    TObjectPtr<const USpriteDataAsset> pDataAsset;
    TObjectPtr<const USpriteDataAsset>* pFoundData = m_pGameInstance->m_mSprites.Find(_oDialogueData.m_sLeftCharacterName);
    if (!pFoundData || !IsValid(*pFoundData)) {
      UE_LOG(LogTemp, Error, TEXT("No asset found with <%s>?"), *_oDialogueData.m_sLeftCharacterName.ToString());
      pDataAsset = nullptr;
    }
    else {
      pDataAsset = *pFoundData;
      const FSpriteAsset* pFoundSprite = pDataAsset->m_mSprite.Find(_oDialogueData.m_sLeftCharacterSprite);
      if (pFoundSprite) {
        if (UAudioManager* pAudioManager = m_pGameInstance->GetSubsystem<UAudioManager>()) {
          pAudioManager->PlaySound2D(this, pFoundSprite->m_pAudio);
        }
      }
    }
  }

  // Character Sprites
  if (IsValid(m_bLeftCharacterImage)) {
    UTexture2D* pTexture = nullptr;
    TObjectPtr<const USpriteDataAsset> pDataAsset;
    TObjectPtr<const USpriteDataAsset>* pFoundData = m_pGameInstance->m_mSprites.Find(_oDialogueData.m_sLeftCharacterName);
    if (!pFoundData || !IsValid(*pFoundData)) {
      UE_LOG(LogTemp, Error, TEXT("No asset found with <%s>?"), *_oDialogueData.m_sLeftCharacterName.ToString());
      pDataAsset = nullptr;
    }
    else {
      pDataAsset = *pFoundData;
      const FSpriteAsset* pFoundTexture = pDataAsset->m_mSprite.Find(_oDialogueData.m_sLeftCharacterSprite);
      if (!pFoundTexture) {
        UE_LOG(LogTemp, Error, TEXT("No asset found with <%s>?"), *_oDialogueData.m_sLeftCharacterSprite.ToString());
        pTexture = nullptr;
      }
      else pTexture = pFoundTexture->m_pTexture;
    }

    m_bLeftCharacterImage->SetBrushFromTexture(IsValid(pTexture) ? pTexture : nullptr);
    m_bLeftCharacterImage->SetVisibility(
      IsValid(pTexture) ?
      ESlateVisibility::SelfHitTestInvisible :
      ESlateVisibility::Hidden
    );
    m_bLeftCharacterImage->SetColorAndOpacity(
      _oDialogueData.m_iCharacterTalking == 1 ?
      FLinearColor(1.f, 1.f, 1.f, 1.f) :
      FLinearColor(.125f, .125f, .125f, 1.f)
    );
  }

  ESlateVisibility oVisibility = _bSpeaking ?
    ESlateVisibility::SelfHitTestInvisible :
    ESlateVisibility::Hidden;

  // Emitter Name
  if (IsValid(m_pLeftEmitterImage)) m_pLeftEmitterImage->SetVisibility(oVisibility);
  if (IsValid(m_pLeftEmitterImage)) {
    m_pLeftEmitterText->SetVisibility(oVisibility);
    m_pLeftEmitterText->SetText(FText::FromName(_oDialogueData.m_sLeftCharacterName));
  }
}

void UDialogueScreen::UpdateRightCharacterInfo(FDialogData _oDialogueData, bool _bSpeaking) {
  if (!IsValid(m_pGameInstance)) {
    UE_LOG(LogTemp, Error, TEXT("GameInstance is nullptr."));
    return;
  }

  if (IsValid(m_pRightCharacterTalks) && m_iLastCharacter != m_iCurrentCharacter) {
    PlayAnimation(
      m_pRightCharacterTalks,
      0.f,
      1,
      m_iCurrentCharacter == 2 ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
  }

  if (_bSpeaking && m_sLastSprite != m_sCurrentSprite) {
    TObjectPtr<const USpriteDataAsset> pDataAsset;
    TObjectPtr<const USpriteDataAsset>* pFoundData = m_pGameInstance->m_mSprites.Find(_oDialogueData.m_sRightCharacterName);
    if (!pFoundData || !IsValid(*pFoundData)) {
      UE_LOG(LogTemp, Error, TEXT("No asset found with <%s>?"), *_oDialogueData.m_sRightCharacterName.ToString());
      pDataAsset = nullptr;
    }
    else {
      pDataAsset = *pFoundData;
      const FSpriteAsset* pFoundSprite = pDataAsset->m_mSprite.Find(_oDialogueData.m_sRightCharacterSprite);
      if (pFoundSprite) {
        if (UAudioManager* pAudioManager = m_pGameInstance->GetSubsystem<UAudioManager>()) {
          pAudioManager->PlaySound2D(this, pFoundSprite->m_pAudio);
        }
      }
    }
  }

  // Character Sprites
  if (IsValid(m_bRightCharacterImage)) {
    UTexture2D* pTexture = nullptr;
    TObjectPtr<const USpriteDataAsset> pDataAsset;
    TObjectPtr<const USpriteDataAsset>* pFoundData = m_pGameInstance->m_mSprites.Find(_oDialogueData.m_sRightCharacterName);
    if (!pFoundData || !IsValid(*pFoundData)) {
      UE_LOG(LogTemp, Error, TEXT("No asset found with <%s>?"), *_oDialogueData.m_sRightCharacterName.ToString());
      pDataAsset = nullptr;
    }
    else {
      pDataAsset = *pFoundData;
      const FSpriteAsset* pFoundTexture = pDataAsset->m_mSprite.Find(_oDialogueData.m_sRightCharacterSprite);
      if (!pFoundTexture) {
        UE_LOG(LogTemp, Error, TEXT("No asset found with <%s>?"), *_oDialogueData.m_sRightCharacterSprite.ToString());
        pTexture = nullptr;
      }
      else pTexture = pFoundTexture->m_pTexture;
    }

    m_bRightCharacterImage->SetBrushFromTexture(pTexture);
    m_bRightCharacterImage->SetVisibility(
      IsValid(pTexture) ?
      ESlateVisibility::SelfHitTestInvisible :
      ESlateVisibility::Hidden
    );
    m_bRightCharacterImage->SetColorAndOpacity(
      _oDialogueData.m_iCharacterTalking == 2 ?
      FLinearColor(1.f, 1.f, 1.f, 1.f) :
      FLinearColor(.125f, .125f, .125f, 1.f)
    );
  }

  ESlateVisibility oVisibility = _bSpeaking ?
    ESlateVisibility::SelfHitTestInvisible :
    ESlateVisibility::Hidden;

  // Emitter Name
  if (IsValid(m_pRightEmitterImage)) m_pRightEmitterImage->SetVisibility(oVisibility);
  if (IsValid(m_pRightEmitterImage)) {
    m_pRightEmitterText->SetVisibility(oVisibility);
    m_pRightEmitterText->SetText(FText::FromName(_oDialogueData.m_sRightCharacterName));
  }
}

void UDialogueScreen::OnDialogueSkipStarts() {
  m_fHoldStartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
  m_bIsHolding = true;

  if (IsValid(m_pSkipButtonPB)) m_pSkipButtonPB->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UDialogueScreen::OnDialogueSkipEnds() {
  m_fHoldStartTime = -1.f;
  m_bIsHolding = false;

  if (IsValid(m_pSkipButtonPB)) {
    m_pSkipButtonPB->GetDynamicMaterial()->SetScalarParameterValue(TEXT("fPercent"), 0.f);
    m_pSkipButtonPB->SetVisibility(ESlateVisibility::Hidden);
  }
}

void UDialogueScreen::HandleDialogueLine() {
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UDialogsManager* pDialogueManager = pGameInstance->GetDialoguesManager()) {
      if (pDialogueManager->m_bLineIsPrinted) {
        UpdateDialogueInfo(pDialogueManager->GetCurrentDialogToDisplay());

        pDialogueManager->m_bLineIsPrinted = false;
      }
      else {
        if (IsValid(m_pTextBox)) m_pTextBox->SetText(pDialogueManager->GetCurrentDialogToDisplay().m_sDialogText);

        m_bIsTyping = false;
        PlaySilentKillerEvent();
        pDialogueManager->m_bLineIsPrinted = true;
      }
    }
  }
}

void UDialogueScreen::SetText(const FString& _rText) {
  m_iCharacterIndex = 0;
  m_iTextLength = _rText.Len();
  m_sCurrentText = _rText;

  m_bIsTyping = true;
}

void UDialogueScreen::SetLetter() {
  FString sCurrentText = m_sCurrentText;
  FString sResultText;
  int32 visibleCharCount = 0;

  int32 iOpenTagCount = 0;
  bool bInsideTag = false;
  FString sCurrentTag;

  for (int32 iIndex = 0; iIndex < sCurrentText.Len(); ++iIndex) {
    TCHAR cCurrentCharacter = sCurrentText[iIndex];

    if (cCurrentCharacter == '<') {
      bInsideTag = true;
      sCurrentTag = "<";
    }
    else if (cCurrentCharacter == '>' && bInsideTag) {
      bInsideTag = false;
      sCurrentTag += ">";

      if (!sCurrentTag.StartsWith("</")) iOpenTagCount++;
      else iOpenTagCount = FMath::Max(0, iOpenTagCount - 1);

      sResultText += sCurrentTag;
    }
    else if (bInsideTag) sCurrentTag += cCurrentCharacter;
    else {
      if (visibleCharCount <= m_iCharacterIndex) {
        sResultText.AppendChar(cCurrentCharacter);
        visibleCharCount++;
      }
      else break;
    }
  }

  for (int32 i = 0; i < iOpenTagCount; ++i) {
    sResultText += "</>";
  }

  m_pTextBox->SetText(FText::FromString(sResultText));
  m_iCharacterIndex++;

  if (m_iCharacterIndex > m_iTextLength) {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UDialogsManager* pDialogueManager = pGameInstance->GetDialoguesManager()) {
        pDialogueManager->m_bLineIsPrinted = true;
        pDialogueManager->CheckNextDialogue();
        m_bIsTyping = false;
        PlaySilentKillerEvent();
      }
    }
  }
}

void UDialogueScreen::PlayTypewritterEvent(FName _sName, float _fValue) {
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlayEventInstanceWithParameters(m_pTypewrittingEvent, { {_sName, _fValue} });
    }
  }
}

void UDialogueScreen::PlaySilentKillerEvent() {
  if (!IsValid(m_pGameInstance)) return;

  if (UAudioManager* pAudioManager = m_pGameInstance->GetSubsystem<UAudioManager>()) {
    pAudioManager->PlaySound2D(this, m_pGameInstance->m_pBabyKillerEvent);
  }
}