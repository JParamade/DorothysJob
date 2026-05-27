// Fill out your copyright notice in the Description page of Project Settings.


#include "CinematicScreen.h"
#include "MediaPlayer.h"
#include "MediaSource.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "DorothysJob/UI/Utils/InputIcon.h"
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include <DorothysJob/Manager/BaseGameInstance.h>

void UCinematicScreen::Show()
{
  Super::Show();
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    pPlayerController->m_oOnCinematicSkip.BindUObject(this, &UCinematicScreen::SkipIntro);
    pPlayerController->m_oOnCinematicSkipStarts.BindUObject(this, &UCinematicScreen::OnDialogueSkipStarts);
    pPlayerController->m_oOnCinematicSkipEnds.BindUObject(this, &UCinematicScreen::OnDialogueSkipEnds);
  }

  if (IsValid(m_pSkipButtonIcon)) m_pSkipButtonIcon->Show();

  PlayIntro();
}

void UCinematicScreen::Hide()
{
  Super::Hide();
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    pPlayerController->m_oOnCinematicSkipEnds.Unbind();
  }

  if (IsValid(m_pSkipButtonIcon)) m_pSkipButtonIcon->Hide();
}

void UCinematicScreen::NativeConstruct()
{
  Super::NativeConstruct();
  
  if (IsValid(m_pCanvasPanel)) m_lElements.Add(m_pCanvasPanel);

  // Skip intro text
  if (IsValid(m_pSkipText)) m_lElements.Add(m_pSkipText);

  // Get Save Game Manager.
  if (UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) m_pSaveGameManager = pBaseGameInstance->GetSubsystem<USaveGameManager>();
}

void UCinematicScreen::NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) {
  Super::NativeTick(_rMyGeometry, _fInDeltaTime);

  if (m_bIsHolding && m_fHoldStartTime > .0f) {
    float fElapsedTime = UGameplayStatics::GetRealTimeSeconds(GetWorld()) - m_fHoldStartTime;
    float fPercent = FMath::Clamp(fElapsedTime, .0f, 1.f);

    if (IsValid(m_pSkipButtonPB)) m_pSkipButtonPB->GetDynamicMaterial()->SetScalarParameterValue(TEXT("fPercent"), fPercent);
  }
}

void UCinematicScreen::PlayIntro()
{
  if (!m_pMediaPlayer || !m_pMediaSource)
  {
    UE_LOG(LogTemp, Warning, TEXT("MediaPlayer o MediaSource no asignados"));
    OnVideoEnd();
    return;
  }

  if (!m_pMediaSource->Validate())
  {
    UE_LOG(LogTemp, Error, TEXT("MediaSource inválido o no se pudo validar: %s"), *m_pMediaSource->GetName());
    OnVideoEnd();
    return;
  }

  if (!m_pMediaSound)
  {
    m_pMediaSound = NewObject<UMediaSoundComponent>(GetOwningPlayerPawn());
    m_pMediaSound->RegisterComponent();
    m_pMediaSound->SetMediaPlayer(m_pMediaPlayer);
    m_pMediaSound->Activate();
  }

  // Vincular evento de fin de reproducción
  m_pMediaPlayer->OnEndReached.AddDynamic(this, &UCinematicScreen::OnVideoEnd);
  m_pMediaPlayer->OnMediaOpened.AddDynamic(this, &UCinematicScreen::OnMediaOpened);

  // Abrir el MediaSource y reproducir
  if (m_pMediaSource)
  {
    m_pMediaPlayer->OpenSource(m_pMediaSource);
  }
}

void UCinematicScreen::OnDialogueSkipStarts() {
  m_fHoldStartTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
  m_bIsHolding = true;

  if (IsValid(m_pSkipButtonPB)) m_pSkipButtonPB->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UCinematicScreen::OnDialogueSkipEnds() {
  m_fHoldStartTime = -1.f;
  m_bIsHolding = false;

  if (IsValid(m_pSkipButtonPB)) {
    m_pSkipButtonPB->GetDynamicMaterial()->SetScalarParameterValue(TEXT("fPercent"), 0.f);
    m_pSkipButtonPB->SetVisibility(ESlateVisibility::Hidden);
  }
}

void UCinematicScreen::OnMediaOpened(FString OpenedUrl)
{
  if (m_pMediaPlayer)
  {
    m_pMediaPlayer->Play();
    UE_LOG(LogTemp, Log, TEXT("Video iniciado correctamente con audio"));
  }
}

void UCinematicScreen::OnVideoEnd()
{
  OnCinematicFinished.Broadcast();


  ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  if (pPlayerController)
  {
    pPlayerController->SetCinematicController(false);
  }

  if (IsValid(m_pSaveGameManager)) {
    m_pSaveGameManager->DeleteSave();
    m_pSaveGameManager->LoadAllGame();
  }

  UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Procedural"));
}

void UCinematicScreen::SkipIntro()
{
  if (m_pMediaPlayer && m_pMediaPlayer->IsPlaying())
  {
    m_pMediaPlayer->Pause();
    OnVideoEnd();
  }
}
