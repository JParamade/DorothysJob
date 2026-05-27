#include "SelectConsumablesMenu.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/RichTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/UI/UMGPalette/Selector/Selector.h"
#include "DorothysJob/UI/Utils/InputIcon.h"
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/Data/DataAssets/ConsumableDataAsset.h"
#include "DorothysJob/Data/DataTables/ConsumablesMenuData.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Controller/BasePlayerController.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Utils/Random/RandomNumberGenerator.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

void USelectConsumablesMenu::NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime)
{
  Super::NativeTick(_rMyGeometry, _fInDeltaTime);

  m_fElapsedTime += _fInDeltaTime;
  if (m_fElapsedTime > m_fTimeBetweenYaps) {
    m_bCanGorchSpeak = true;
    m_fElapsedTime = 0.f;
  }
}

void USelectConsumablesMenu::NativeConstruct()
{
  Super::NativeConstruct();

  InitMenu();
}

void USelectConsumablesMenu::Show()
{
  OnMenuOpened.Broadcast();

  m_iSelectedIndex = 0;

  m_pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (!IsValid(m_pGameInstance))
  {
    return;
  }

  m_iTotalElements = m_pGameInstance->m_lConsumables.Num();
  m_iCurrentIndex = (m_iTotalElements > 0) ? 1 : 0;

  if (IsValid(m_pConsumablesCarousel))
  {
    TArray<FText> lConsumables;
    for (TObjectPtr<const UConsumableDataAsset> pConsumable : m_pGameInstance->m_lConsumables)
    {
      lConsumables.Add(pConsumable->m_sName);
    }
    m_pConsumablesCarousel->InitializeOptions(lConsumables, m_iCurrentIndex);
    m_pConsumablesCarousel->Show();
  }

  m_bCanGorchSpeak = false;

  UpdateCarouselImages();
  RestoreSelected();
  SetupFocus();

  if (IsValid(m_pExitIconButton))
  {
    m_pExitIconButton->Show();
  }
  
  if (IsValid(m_pExitText))
  {
    m_pExitText->SetVisibility(ESlateVisibility::Visible);
  }

  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem)
  {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Hub/SFX_HUB_shop_open", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }

  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    if (pWorld->GetTimerManager().IsTimerActive(m_tShowMenuTimerHandle))
    {
      pWorld->GetTimerManager().ClearTimer(m_tShowMenuTimerHandle);
    }
    pWorld->GetTimerManager().SetTimer(m_tShowMenuTimerHandle, this, &USelectConsumablesMenu::OnShowMenu, 1.2f, false);
  }
}

void USelectConsumablesMenu::Hide()
{
  SetPauseMode(false);
  OnMenuClosed.Broadcast();

  if (IsValid(m_pSCPanel))
  {
    m_pSCPanel->SetVisibility(ESlateVisibility::Collapsed);
  }

  if (IsValid(m_pConsumablesCarousel))
  {
    m_pConsumablesCarousel->Hide();
  }

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pGameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameInstance not found"));
    return;
  }

  UEventManager* pEventManager = pGameInstance->GetEventManager();
  if (!pEventManager)
  {
    return;
  }

  if (pGameInstance->m_iCurrentLevel == 3)
  {
    pEventManager->TriggerEvent(TEXT("LEV_3_CONSUMABLES_REVEAL"));
  }

  if (IsValid(m_pExitIconButton))
  {
    m_pExitIconButton->Hide();
  }
  if (IsValid(m_pExitText))
  {
    m_pExitText->SetVisibility(ESlateVisibility::Hidden);
  }

  RestoreSelected();
  m_bIsVisible = false;

  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    if (pWorld->GetTimerManager().IsTimerActive(m_tShowMenuTimerHandle))
    {
      pWorld->GetTimerManager().ClearTimer(m_tShowMenuTimerHandle);
    }
  }
}

void USelectConsumablesMenu::SetupFocus()
{
  Super::SetupFocus();

  m_lFocusableElements.Empty();
  m_pSpecificFocusManager.Empty();

  if (IsValid(m_pConsumablesCarousel))
  {
    m_lFocusableElements.Add(Cast<UObject>(m_pConsumablesCarousel));
  }

  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { 1 }, EFocusNavigationType::HORIZONTAL, false, false);
  m_pSpecificFocusManager[0]->ActivateFocus();

  ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  UGeneralFocusManager* pGeneralFocusManager = IsValid(pPlayerController) ? pPlayerController->GetGeneralFocusManager() : nullptr;
  if (IsValid(pGeneralFocusManager))
  {
    pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
  }
}

void USelectConsumablesMenu::InitMenu()
{
  if (IsValid(m_pConsumablesCarousel))
  {
    m_pConsumablesCarousel->OnSelectionChanged.AddUniqueDynamic(this, &USelectConsumablesMenu::OnConsumableChanged);
  }

  m_pSCPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void USelectConsumablesMenu::HandleReceivedInput()
{
  const UConsumableDataAsset* pConsumable = m_pGameInstance->m_lConsumables[m_iCurrentIndex];

  if (!IsValid(pConsumable))
  {
    UE_LOG(LogTemp, Warning, TEXT("Invalid consumable passed."));
    return;
  }

  if (m_iSelectedIndex == 0)
  {
    if (IsValid(m_pSecondConsumable) && pConsumable == m_pSecondConsumable)
    {
      m_pSecondConsumable = IsValid(m_pFirstConsumable) ? m_pFirstConsumable : nullptr;
    }
    m_pFirstConsumable = pConsumable;
    m_iSelectedIndex = 1;
  }
  else if (m_iSelectedIndex == 1)
  {
    if (IsValid(m_pFirstConsumable) && pConsumable == m_pFirstConsumable)
    {
      m_pFirstConsumable = IsValid(m_pSecondConsumable) ? m_pSecondConsumable : nullptr;
    }
    m_pSecondConsumable = pConsumable;
    m_iSelectedIndex = 0;
  }

  UpdateSelectedImages();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_acceptButton.SFX_UI_acceptButton")));
    }
  }
}

void USelectConsumablesMenu::SelectConsumable(const UConsumableDataAsset* _pConsumable)
{
  if (!IsValid(_pConsumable))
  {
    UE_LOG(LogTemp, Warning, TEXT("Invalid consumable passed."));
    return;
  }

  // we check if the consumable is already selected
  if (_pConsumable == m_pFirstConsumable)
  {
    UE_LOG(LogTemp, Warning, TEXT("Consumable already selected"));
    return;
  }
  // if it's on the second we interchange
  if (_pConsumable == m_pSecondConsumable)
  {
    m_pSecondConsumable = m_pFirstConsumable;
    m_pFirstConsumable = _pConsumable;
  }

  UpdateSelectedImages();
}

void USelectConsumablesMenu::RestoreSelected()
{
  if (IsValid(m_pGameInstance) && (!m_pGameInstance->m_lSelectedConsumables.IsEmpty()))
  {
    m_pFirstConsumable = (m_pGameInstance->m_lSelectedConsumables.Num() > 0) ? m_pGameInstance->m_lSelectedConsumables[0] : nullptr;
    m_pSecondConsumable = (m_pGameInstance->m_lSelectedConsumables.Num() > 1) ? m_pGameInstance->m_lSelectedConsumables[1] : nullptr;
  }

  m_iSelectedIndex = 0;
  UpdateSelectedImages();
}

void USelectConsumablesMenu::CancelAction()
{
  // cannot exit if neither of the consumables are selected
  if (!IsValid(m_pFirstConsumable) || !IsValid(m_pSecondConsumable))
  {
    UE_LOG(LogTemp, Warning, TEXT("Consumables not selected"));
    return;
  }

  // we save the selected consumables into the game instance
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (IsValid(pGameInstance))
  {
    pGameInstance->m_lSelectedConsumables.Empty();
    pGameInstance->m_lSelectedConsumables.Add(m_pFirstConsumable);
    pGameInstance->m_lSelectedConsumables.Add(m_pSecondConsumable);
    USaveGameManager* pSaveGameManager = pGameInstance->GetSubsystem<USaveGameManager>();
    if (pSaveGameManager && pGameInstance->m_iCurrentLevel >= pGameInstance->m_iHighestLevelAchieved)
    {
      pSaveGameManager->SaveConsumablesBuild();
    }
  }

  // Spawn the selected consumibles in the player
  ABasePlayerController* pController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  ABasePlayer* pPlayer = IsValid(pController) ? Cast<ABasePlayer>(pController->GetPawn()) : nullptr;
  if (IsValid(pPlayer))
  {
    pPlayer->SetSelectedConsumables();
  }

  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Hub/SFX_HUB_shop_close", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }

  GetWorld()->GetSubsystem<ULevelManager>()->SetCurrentGameState(EBaseGameState::PLAYING);
}

void USelectConsumablesMenu::UpdateSelectedImages()
{
  m_pFirstConsumableImage->SetBrushFromTexture(IsValid(m_pFirstConsumable) ? m_pFirstConsumable->m_pIcon : nullptr);
  m_pFirstConsumableImage->SetVisibility(IsValid(m_pFirstConsumable) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

  m_pSecondConsumableImage->SetBrushFromTexture(IsValid(m_pSecondConsumable) ? m_pSecondConsumable->m_pIcon : nullptr);
  m_pSecondConsumableImage->SetVisibility(IsValid(m_pSecondConsumable) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

  m_pFirstConsumableSelectedImage->SetVisibility(m_iSelectedIndex == 0 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
  m_pSecondConsumableSelectedImage->SetVisibility(m_iSelectedIndex == 1 ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void USelectConsumablesMenu::UpdateCarouselImages()
{
  if (!IsValid(m_pGameInstance))
  {
    return;
  }

  int32 iLeftIndex = (m_iCurrentIndex - 1 + m_iTotalElements) % m_iTotalElements;
  int32 iRightIndex = (m_iCurrentIndex + 1) % m_iTotalElements;

  if (IsValid(m_pLeftImage) && IsValid(m_pGameInstance->m_lConsumables[iLeftIndex]->m_pIcon))
  {
    m_pLeftImage->SetBrushFromTexture(m_pGameInstance->m_lConsumables[iLeftIndex]->m_pIcon);
    m_pLeftBanner->SetBrushFromTexture(m_pGameInstance->m_lConsumables[iLeftIndex]->m_pLittleBanner);
  }

  if (IsValid(m_pCentralImage) && IsValid(m_pGameInstance->m_lConsumables[m_iCurrentIndex]->m_pIcon))
  {
    m_pCentralImage->SetBrushFromTexture(m_pGameInstance->m_lConsumables[m_iCurrentIndex]->m_pIcon);
    m_pCentralBanner->SetBrushFromTexture(m_pGameInstance->m_lConsumables[m_iCurrentIndex]->m_pFocusBanner);

    FText sText = FText::FromString("???");
    TArray<FConsumablesMenuData*> lInfos;
    if (IsValid(m_pGameInstance->m_pConsumablesData))
    {
      m_pGameInstance->m_pConsumablesData->GetAllRows(TEXT("Info"), lInfos);
    }
    if (!lInfos.IsEmpty())
    {
      TSharedPtr<RandomNumberGenerator> RNG = MakeShared<RandomNumberGenerator>();
      int32 iIndex = RNG->GetRandomInRange(0, 2);
      TArray<FText> lSelectedInfo;
      for (FConsumablesMenuData* pData : lInfos)
      {
        if (pData->m_eConsumableType == m_pGameInstance->m_lConsumables[m_iCurrentIndex]->m_eConsumableClass)
        {
          lSelectedInfo = pData->m_sInfo;
          break;
        }
      }

      sText = lSelectedInfo[iIndex];
    }

    m_pInfoText->SetText(sText);
    m_pShortInfoText->SetText(m_pGameInstance->m_lConsumables[m_iCurrentIndex]->m_sShortInfo);
  }

  if (IsValid(m_pRightImage) && IsValid(m_pGameInstance->m_lConsumables[iRightIndex]->m_pIcon))
  {
    m_pRightImage->SetBrushFromTexture(m_pGameInstance->m_lConsumables[iRightIndex]->m_pIcon);
    m_pRightBanner->SetBrushFromTexture(m_pGameInstance->m_lConsumables[iRightIndex]->m_pLittleBanner);
  }

  if (m_bCanGorchSpeak) {
    FMOD::Studio::EventInstance* pEvent = nullptr;
    FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
    if (pStudioSystem) {
      FMOD::Studio::EventDescription* pEventDesc = nullptr;
      pStudioSystem->getEvent("event:/SoundFX/Characters/Dialogue/Gorch/VSFX_DIAL_gorch_neutral", &pEventDesc);

      if (pEventDesc) pEventDesc->createInstance(&pEvent);
      if (pEvent) pEvent->start();
    }

    m_bCanGorchSpeak = false;
  }
}

void USelectConsumablesMenu::OnScroll(int32 _iDirection)
{
  m_iCurrentIndex = (m_iCurrentIndex + _iDirection + m_iTotalElements) % m_iTotalElements;
  if (IsValid(m_pConsumablesCarousel))
  {
    m_pConsumablesCarousel->iCurrentIndex = m_iCurrentIndex;
  }
  UpdateCarouselImages();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_hover.SFX_UI_hover")));
    }
  }
}

void USelectConsumablesMenu::OnConsumableChanged(int32 _iIndex)
{
  m_iCurrentIndex = _iIndex;
  UpdateCarouselImages();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_hover.SFX_UI_hover")));
    }
  }
}

void USelectConsumablesMenu::OnShowMenu()
{
  if (IsValid(m_pSCPanel) && m_pSCPanel->GetVisibility() != ESlateVisibility::Visible)
  {
    m_pSCPanel->SetVisibility(ESlateVisibility::Visible);
  }
  SetPauseMode(true);
  m_bIsVisible = true;
  OnMenuShow.Broadcast();
}