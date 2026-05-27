#include "SelectWeaponsMenu.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/Controller/BasePlayerController.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/UI/Utils/InputIcon.h"

void USelectWeaponsMenu::NativeConstruct()
{
  Super::NativeConstruct();

  InitMenu();
}

void USelectWeaponsMenu::Show()
{
  OnMenuOpened.Broadcast();

  SetupFocus();

  if (IsValid(m_pFadeInAnimation))
  {
    PlayAnimation(m_pFadeInAnimation);
  }

  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem)
  {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Hub/SFX_HUB_chest_open", &pEventDesc);
    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }

  if (IsValid(m_pExitIconButton)) m_pExitIconButton->Show();
  if (IsValid(m_pExitText)) m_pExitText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    if (pWorld->GetTimerManager().IsTimerActive(m_tShowMenuTimerHandle))
    {
      pWorld->GetTimerManager().ClearTimer(m_tShowMenuTimerHandle);
    }
    pWorld->GetTimerManager().SetTimer(m_tShowMenuTimerHandle, this, &USelectWeaponsMenu::OnShowMenu, 0.45f, false);
  }
}

void USelectWeaponsMenu::Hide()
{
  SetPauseMode(false);
  OnMenuClosed.Broadcast();
  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
  }
  RestoreSelected();

  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    if (pWorld->GetTimerManager().IsTimerActive(m_tShowMenuTimerHandle))
    {
      pWorld->GetTimerManager().ClearTimer(m_tShowMenuTimerHandle);
    }
  }

  if (IsValid(m_pExitIconButton)) m_pExitIconButton->Hide();
  if (IsValid(m_pExitText)) m_pExitText->SetVisibility(ESlateVisibility::Hidden);

  m_bIsVisible = false;
}

void USelectWeaponsMenu::SetupFocus()
{
  Super::SetupFocus();

  m_lFocusableElements.Empty();
  m_pSpecificFocusManager.Empty();

  if (IsValid(m_pWasheeFlasheeWeapon))
  {
    m_lFocusableElements.Add(Cast<UObject>(m_pWasheeFlasheeButton));
    ++m_iFirtsFocusLineSize;
  }
  if (IsValid(m_pMopressorWeapon))
  {
    m_lFocusableElements.Add(Cast<UObject>(m_pMopressorButton));
    ++m_iFirtsFocusLineSize;
  }
  if (IsValid(m_pDustyWeapon))
  {
    m_lFocusableElements.Add(Cast<UObject>(m_pDustyButton));
    ++m_iFirtsFocusLineSize;
  }
  if (IsValid(m_pDirtminatorWeapon))
  {
    m_lFocusableElements.Add(Cast<UObject>(m_pDirtminatorButton));
    ++m_iFirtsFocusLineSize;
  }

  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  InitFocusManager();

  UpdateCanExitState();

  ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  UGeneralFocusManager* pGeneralFocusManager = IsValid(pPlayerController) ? pPlayerController->GetGeneralFocusManager() : nullptr;
  if (IsValid(pGeneralFocusManager))
  {
    pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
  }
}

void USelectWeaponsMenu::InitMenu()
{
  m_pMopressorWeapon = nullptr;
  m_pWasheeFlasheeWeapon = nullptr;
  m_pDustyWeapon = nullptr;
  m_pDirtminatorWeapon = nullptr;
  m_pMopressorButton->SetVisibility(ESlateVisibility::Collapsed);
  m_pWasheeFlasheeButton->SetVisibility(ESlateVisibility::Collapsed);
  m_pDustyButton->SetVisibility(ESlateVisibility::Collapsed);
  m_pDirtminatorButton->SetVisibility(ESlateVisibility::Collapsed);
  m_pMopressorButton->OnClicked.RemoveAll(this);
  m_pWasheeFlasheeButton->OnClicked.RemoveAll(this);
  m_pDustyButton->OnClicked.RemoveAll(this);
  m_pDirtminatorButton->OnClicked.RemoveAll(this);
  m_lFocusableElements.Empty();
  m_pSpecificFocusManager.Empty();
  m_iFirtsFocusLineSize = 0;
  m_bCanSaveExit = false;

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (!IsValid(pGameInstance))
  {
    return;
  }

  for (const UWeaponDataAsset* pWeaponAsset : pGameInstance->m_lWeapons)
  {
    if (pWeaponAsset->m_eWeaponCleanType == EDirtType::Liquid)
    {
      if (pWeaponAsset->m_eWeaponAttackType == EWeaponAttackType::Melee)
      {
        m_pMopressorButton->SetVisibility(ESlateVisibility::Visible);
        m_pMopressorWeapon = pWeaponAsset;
        m_pMopressorButton->OnClicked.AddUniqueDynamic(this, &USelectWeaponsMenu::OnMopressorClicked);
        m_pMopressorButton->OnHoveredAction.AddUniqueDynamic(this, &USelectWeaponsMenu::OnMopressorHovered);
      }
      else if (pWeaponAsset->m_eWeaponAttackType == EWeaponAttackType::Distance)
      {
        m_pWasheeFlasheeButton->SetVisibility(ESlateVisibility::Visible);
        m_pWasheeFlasheeWeapon = pWeaponAsset;
        m_pWasheeFlasheeButton->OnClicked.AddUniqueDynamic(this, &USelectWeaponsMenu::OnWasheeFlasheeClicked);
        m_pWasheeFlasheeButton->OnHoveredAction.AddUniqueDynamic(this, &USelectWeaponsMenu::OnWasheeFlasheeHovered);
      }
    }
    else if (pWeaponAsset->m_eWeaponCleanType == EDirtType::Dust)
    {
      if (pWeaponAsset->m_eWeaponAttackType == EWeaponAttackType::Melee)
      {
        m_pDustyButton->SetVisibility(ESlateVisibility::Visible);
        m_pDustyWeapon = pWeaponAsset;
        m_pDustyButton->OnClicked.AddUniqueDynamic(this, &USelectWeaponsMenu::OnDustyClicked);
        m_pDustyButton->OnHoveredAction.AddUniqueDynamic(this, &USelectWeaponsMenu::OnDustyHovered);
      }
      else if (pWeaponAsset->m_eWeaponAttackType == EWeaponAttackType::Distance)
      {
        m_pDirtminatorButton->SetVisibility(ESlateVisibility::Visible);
        m_pDirtminatorWeapon = pWeaponAsset;
        m_pDirtminatorButton->OnClicked.AddUniqueDynamic(this, &USelectWeaponsMenu::OnDirtminatorClicked);
        m_pDirtminatorButton->OnHoveredAction.AddUniqueDynamic(this, &USelectWeaponsMenu::OnDirtminatorHovered);
      }
    }
  }
  RestoreSelected();
  m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void USelectWeaponsMenu::CancelAction()
{
  if (m_bCanSaveExit)
  {
    // Add the selected weapons in the game instance selected weapons
    UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
    if (IsValid(pGameInstance))
    {
      pGameInstance->m_lSelectedWeapons.Empty();
      pGameInstance->m_lSelectedWeapons.Add(m_pLiquidWeapon);
      if (IsValid(m_pDustWeapon))
      {
        pGameInstance->m_lSelectedWeapons.Add(m_pDustWeapon);
      }
      USaveGameManager* pSaveGameManager = pGameInstance->GetSubsystem<USaveGameManager>();
      if (IsValid(pSaveGameManager) && pGameInstance->m_iCurrentLevel >= pGameInstance->m_iHighestLevelAchieved)
      {
        pSaveGameManager->SaveWeaponsBuild();
      }
    }
    // Spawn the selected weapons in the player
    ABasePlayerController* pController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    ABasePlayer* pPlayer = IsValid(pController) ? Cast<ABasePlayer>(pController->GetPawn()) : nullptr;
    if (IsValid(pPlayer))
    {
      pPlayer->SpawnSelectedWeapons();
    }

    FMOD::Studio::EventInstance* pEvent = nullptr;
    FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
    if (pStudioSystem)
    {
      FMOD::Studio::EventDescription* pEventDesc = nullptr;
      pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Hub/SFX_HUB_chest_close", &pEventDesc);
      if (pEventDesc) pEventDesc->createInstance(&pEvent);
      if (pEvent) pEvent->start();
    }

    // Close the menu and return to the game
    GetWorld()->GetSubsystem<ULevelManager>()->SetCurrentGameState(EBaseGameState::PLAYING);
  }
}

void USelectWeaponsMenu::SelectWeapon(const UWeaponDataAsset* _pWeapon)
{
  if (_pWeapon->m_eWeaponCleanType == EDirtType::Liquid)
  {
    m_pLiquidWeapon = _pWeapon;
  }
  else if (_pWeapon->m_eWeaponCleanType == EDirtType::Dust)
  {
    m_pDustWeapon = _pWeapon;
  }
  UpdateSelectedImages();
  UpdateCanExitState();
}

void USelectWeaponsMenu::HoverWeapon(const TArray<FText>& _pWeapon)
{
  if (_pWeapon.Num() == 3)
  {
    m_pWeaponDetailsAttack->SetText(_pWeapon[0]);
    m_pWeaponDetailsClean->SetText(_pWeapon[1]);
    m_pWeaponDetailsSpecial->SetText(_pWeapon[2]);
  }
}

void USelectWeaponsMenu::RestoreSelected()
{
  m_pLiquidWeapon = nullptr;
  m_pDustWeapon = nullptr;

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (IsValid(pGameInstance))
  {
    for (const UWeaponDataAsset* pWeaponAsset : pGameInstance->m_lSelectedWeapons)
    {
      if (IsValid(pWeaponAsset))
      {
        if (pWeaponAsset->m_eWeaponCleanType == EDirtType::Liquid)
        {
          m_pLiquidWeapon = pWeaponAsset;
        }
        else if (pWeaponAsset->m_eWeaponCleanType == EDirtType::Dust)
        {
          m_pDustWeapon = pWeaponAsset;
        }
      }
    }
  }

  UpdateSelectedImages();
  UpdateCanExitState();
}

void USelectWeaponsMenu::UpdateSelectedImages()
{
  m_pMopressorButton->SetButtonSelected(false);
  m_pWasheeFlasheeButton->SetButtonSelected(false);
  m_pDustyButton->SetButtonSelected(false);
  m_pDirtminatorButton->SetButtonSelected(false);

  if (IsValid(m_pLiquidWeapon))
  {
    if (m_pLiquidWeapon->m_eWeaponAttackType == EWeaponAttackType::Melee)
    {
      m_pMopressorButton->SetButtonSelected(true);
    }
    else if (m_pLiquidWeapon->m_eWeaponAttackType == EWeaponAttackType::Distance)
    {
      m_pWasheeFlasheeButton->SetButtonSelected(true);
    }
  }

  if (IsValid(m_pDustWeapon))
  {
    if (m_pDustWeapon->m_eWeaponAttackType == EWeaponAttackType::Melee)
    {
      m_pDustyButton->SetButtonSelected(true);
    }
    else if (m_pDustWeapon->m_eWeaponAttackType == EWeaponAttackType::Distance)
    {
      m_pDirtminatorButton->SetButtonSelected(true);
    }
  }
}

void USelectWeaponsMenu::UpdateCanExitState()
{
  bool bLiquidWeaponAvailableInDay = true;
  bool bDustWeaponAvailableInDay = true;
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (IsValid(pGameInstance))
  {
    bLiquidWeaponAvailableInDay = pGameInstance->m_lWeapons.Contains(m_pLiquidWeapon);
    bDustWeaponAvailableInDay = pGameInstance->m_lWeapons.Contains(m_pDustWeapon);
  }
  bool bIsDustWeaponsAvailable = IsValid(m_pDustyWeapon) || IsValid(m_pDirtminatorWeapon);
  m_bCanSaveExit = (IsValid(m_pLiquidWeapon) && bLiquidWeaponAvailableInDay) && ((!bIsDustWeaponsAvailable) || (IsValid(m_pDustWeapon) && bDustWeaponAvailableInDay));
}

void USelectWeaponsMenu::InitFocusManager()
{
  m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { m_iFirtsFocusLineSize }, EFocusNavigationType::HORIZONTAL, true, false);
}

void USelectWeaponsMenu::OnShowMenu()
{
  if (IsValid(m_pCanvasPanel) && m_pCanvasPanel->GetVisibility() != ESlateVisibility::Visible)
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Visible);
  }
  SetPauseMode(true);
  m_bIsVisible = true;
  OnMenuShow.Broadcast();
}

void USelectWeaponsMenu::OnMopressorClicked()
{
  SelectWeapon(m_pMopressorWeapon);
}

void USelectWeaponsMenu::OnMopressorHovered()
{
  m_pWeaponNameImage->SetBrushFromTexture(m_pMopressorName);
  HoverWeapon(m_lMopressorDetailsText);
}

void USelectWeaponsMenu::OnWasheeFlasheeClicked()
{
  SelectWeapon(m_pWasheeFlasheeWeapon);
}

void USelectWeaponsMenu::OnWasheeFlasheeHovered()
{
  m_pWeaponNameImage->SetBrushFromTexture(m_pWasheeFlasheeName);
  HoverWeapon(m_lWasheeFlasheeDetailsText);
}

void USelectWeaponsMenu::OnDustyClicked()
{
  SelectWeapon(m_pDustyWeapon);
}

void USelectWeaponsMenu::OnDustyHovered()
{
  m_pWeaponNameImage->SetBrushFromTexture(m_pDustyName);
  HoverWeapon(m_lDustyDetailsText);
}

void USelectWeaponsMenu::OnDirtminatorClicked()
{
  SelectWeapon(m_pDirtminatorWeapon);
}

void USelectWeaponsMenu::OnDirtminatorHovered()
{
  m_pWeaponNameImage->SetBrushFromTexture(m_pDirtminatorName);
  HoverWeapon(m_lDirtminatorDetailsText);
}