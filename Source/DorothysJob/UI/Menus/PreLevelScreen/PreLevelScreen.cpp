/************************************************************************
 * @description: Represents the UMG Pre-Level Screen class.
 * @author: Jaime Paramo
 * @date: 23/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "PreLevelScreen.h"

// Component
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"

// Data
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/ConsumableDataAsset.h"
#include "DorothysJob/Data/DataTables/LevelData.h"
#include "Engine/DataTable.h"

// Gameplay
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "FMODBlueprintStatics.h"

void UPreLevelScreen::Show()
{
  OnMenuOpened.Broadcast();

  SetupFocus();

  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem)
  {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Hub/SFX_HUB_exitDoor_open", &pEventDesc);

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
    pWorld->GetTimerManager().SetTimer(m_tShowMenuTimerHandle, this, &UPreLevelScreen::OnShowMenu, 1.2f, false);
  }
}

void UPreLevelScreen::Hide()
{
  Super::Hide();

  OnMenuClosed.Broadcast();

  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    if (pWorld->GetTimerManager().IsTimerActive(m_tShowMenuTimerHandle))
    {
      pWorld->GetTimerManager().ClearTimer(m_tShowMenuTimerHandle);
    }
  }

  SetPauseMode(false);
}

void UPreLevelScreen::NativeConstruct()
{
  Super::NativeConstruct();

  // Non Focusable Elements
    // Background
  if (IsValid(m_pBackgroundImage)) m_lElements.Add(m_pBackgroundImage);
  if (IsValid(m_pBackgroundBarsImage)) m_lElements.Add(m_pBackgroundBarsImage);

  // Dorothy
  if (IsValid(m_pDorothyImage)) m_lElements.Add(m_pDorothyImage);

  // Weapons
  if (IsValid(m_pWeaponsBanner)) m_lElements.Add(m_pWeaponsBanner);
  if (IsValid(m_pWeaponsText)) m_lElements.Add(m_pWeaponsText);
  if (IsValid(m_pPrimaryWeaponImage)) m_lElements.Add(m_pPrimaryWeaponImage);
  if (IsValid(m_pPrimaryWeaponBG)) m_lElements.Add(m_pPrimaryWeaponBG);
  if (IsValid(m_pSecondaryWeaponImage)) m_lElements.Add(m_pSecondaryWeaponImage);
  if (IsValid(m_pSecondaryWeaponBG)) m_lElements.Add(m_pSecondaryWeaponBG);

  // Consumables
  if (IsValid(m_pConsumablesBanner)) m_lElements.Add(m_pConsumablesBanner);
  if (IsValid(m_pConsumablesText)) m_lElements.Add(m_pConsumablesText);
  if (IsValid(m_pPrimaryConsumableImage)) m_lElements.Add(m_pPrimaryConsumableImage);
  if (IsValid(m_pPrimaryConsumableBG)) m_lElements.Add(m_pPrimaryConsumableBG);
  if (IsValid(m_pSecondaryConsumableImage)) m_lElements.Add(m_pSecondaryConsumableImage);
  if (IsValid(m_pSecondaryConsumableBG)) m_lElements.Add(m_pSecondaryConsumableBG);

  // Level
  if (IsValid(m_pTimerText)) m_lElements.Add(m_pTimerText);
  if (IsValid(m_pLevelDescriptionImage)) m_lElements.Add(m_pLevelDescriptionImage);
  if (IsValid(m_pLevelDescriptionText)) m_lElements.Add(m_pLevelDescriptionText);

  // Current day
  if (IsValid(m_pCurrentDayText)) m_lElements.Add(m_pCurrentDayText);
  if (IsValid(m_pCurrentDayImage)) m_lElements.Add(m_pCurrentDayImage);

  // Focusable Elements
  if (IsValid(m_pCancelButton))
  {
    m_lFocusableElements.Add(m_pCancelButton);
    m_pCancelButton->OnClicked.AddUniqueDynamic(this, &UPreLevelScreen::OnCancel);
  }
  if (IsValid(m_pConfirmButton))
  {
    m_lFocusableElements.Add(m_pConfirmButton);
    m_pConfirmButton->OnClicked.AddUniqueDynamic(this, &UPreLevelScreen::OnConfirm);
  }
}

void UPreLevelScreen::SetupFocus()
{
  Super::SetupFocus();

  // Setup SpecificFocusManager
  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  m_pSpecificFocusManager[0]->Initialize(
    m_lFocusableElements,
    { 2 },
    EFocusNavigationType::HORIZONTAL,
    true,
    false
  );

  // Set this menu as the currently active focus manager.
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
  {
    if (UGeneralFocusManager* pGeneralFocusManager = pPlayerController->GetGeneralFocusManager())
    {
      pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
    }
  }
}

void UPreLevelScreen::CancelAction()
{
  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem)
  {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Hub/SFX_HUB_door_close", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }
  GetWorld()->GetSubsystem<ULevelManager>()->SetCurrentGameState(EBaseGameState::PLAYING);
}

void UPreLevelScreen::UpdateArsenalInfo()
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (!IsValid(pGameInstance)) return;

  if (pGameInstance->m_lSelectedWeapons.IsValidIndex(0)) SetBrushFromDataAsset(pGameInstance->m_lSelectedWeapons[0], m_pPrimaryWeaponImage);
  else m_pPrimaryWeaponImage->SetVisibility(ESlateVisibility::Hidden);

  if (pGameInstance->m_lSelectedWeapons.IsValidIndex(1)) SetBrushFromDataAsset(pGameInstance->m_lSelectedWeapons[1], m_pSecondaryWeaponImage);
  else m_pSecondaryWeaponImage->SetVisibility(ESlateVisibility::Hidden);

  if (pGameInstance->m_lSelectedConsumables.IsValidIndex(0)) SetBrushFromDataAsset(pGameInstance->m_lSelectedConsumables[0], m_pPrimaryConsumableImage);
  else 
  {
    m_pPrimaryConsumableImage->SetVisibility(ESlateVisibility::Hidden);
    m_pPrimaryConsumableBG->SetVisibility(ESlateVisibility::Hidden);
    m_pConsumablesText->SetVisibility(ESlateVisibility::Hidden);
    m_pConsumablesBanner->SetVisibility(ESlateVisibility::Hidden);
  }

  if (pGameInstance->m_lSelectedConsumables.IsValidIndex(1)) SetBrushFromDataAsset(pGameInstance->m_lSelectedConsumables[1], m_pSecondaryConsumableImage);
  else 
  {
    m_pSecondaryConsumableImage->SetVisibility(ESlateVisibility::Hidden);
    m_pSecondaryConsumableBG->SetVisibility(ESlateVisibility::Hidden);
    m_pConsumablesText->SetVisibility(ESlateVisibility::Hidden);
    m_pConsumablesBanner->SetVisibility(ESlateVisibility::Hidden);
  }
}

void UPreLevelScreen::UpdateLevelInfo()
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (!IsValid(pGameInstance)) return;

  if (IsValid(pGameInstance->m_pLevelData))
  {
    TArray<FLevelData*> lAllRows;
    pGameInstance->m_pLevelData->GetAllRows(FString("FindRowByID"), lAllRows);

    for (FLevelData* pRow : lAllRows)
    {
      if (pRow && pRow->m_iLevelID == pGameInstance->m_iCurrentLevel)
      {
        if (IsValid(m_pLevelDescriptionText)) m_pLevelDescriptionText->SetText(pRow->m_sDescription);

        if (IsValid(m_pTimerText))
        {
          int32 iMinutes = pRow->Minutes;
          int32 iSeconds = pRow->Seconds;

          FString sTimeString = FString::Printf(TEXT("%02d:%02d"), iMinutes, iSeconds);
          FText sTimeText = FText::FromString(sTimeString);

          m_pTimerText->SetText(sTimeText);
        }
      }
    }
  }
}

template<typename T>
void UPreLevelScreen::SetBrushFromDataAsset(const TObjectPtr<T> _pDataAsset, TObjectPtr<UImage> _pImageWidget)
{
  if (!IsValid(_pImageWidget)) return;

  if (IsValid(_pDataAsset)) _pImageWidget->SetBrushFromTexture(_pDataAsset->m_pIcon);
  else _pImageWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UPreLevelScreen::OnCancel()
{
  if (m_bIsVisible)
  {
    if (ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>()) pLevelManager->SetCurrentGameState(EBaseGameState::PLAYING);

    FMOD::Studio::EventInstance* pEvent = nullptr;
    FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
    if (pStudioSystem)
    {
      FMOD::Studio::EventDescription* pEventDesc = nullptr;
      pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Hub/SFX_HUB_door_close", &pEventDesc);

      if (pEventDesc) pEventDesc->createInstance(&pEvent);
      if (pEvent) pEvent->start();
    }

    OnPreLevelFinish.Broadcast(false);
  }
}

void UPreLevelScreen::OnConfirm()
{

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

  if (pGameInstance)
  {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>())
    {
      pAudioManager->PlaySound2D(this, m_pStepsEvent);
    }
  }

  if (pGameInstance->m_iCurrentLevel == 1)
  {
    pEventManager->TriggerEvent(TEXT("INIT_LEVEL_1_RevealClock"));
  }
  
  OnPreLevelFinish.Broadcast(true);
}

void UPreLevelScreen::UpdateCurrentDay()
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!IsValid(pGameInstance) || !IsValid(m_pCurrentDayText))
  {
    return;
  }

  m_pCurrentDayText->SetText(FText::Format(m_sDayText, FText::AsNumber(pGameInstance->m_iCurrentLevel)));
}

void UPreLevelScreen::OnShowMenu()
{
  UFocusableWidget::Show();

  // Update UMG element's information.
  UpdateArsenalInfo();
  UpdateLevelInfo();
  UpdateCurrentDay();

  SetPauseMode(true);
  OnMenuShow.Broadcast();
}