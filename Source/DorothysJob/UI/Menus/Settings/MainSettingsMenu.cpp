#include "MainSettingsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Animation/WidgetAnimation.h"
#include "Internationalization/Culture.h"
#include "DorothysJob/UI/Menus/Main/SettingsMenu.h"
#include "ControlsTab.h"
#include "DorothysJob/UI/Menus/ConfirmationWindow.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

void UMainSettingsMenu::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pSettingsMenu))
  {
    m_pSettingsMenu->OnMenuClosed.AddUniqueDynamic(this, &UMainSettingsMenu::Hide);
  }

  if (IsValid(m_pControlsScreen))
  {
    m_pControlsScreen->OnMenuClosed.AddUniqueDynamic(this, &UMainSettingsMenu::Hide);
  }

  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
  }

  m_bIsVisible = false;
}

void UMainSettingsMenu::NativeDestruct()
{
  if (IsValid(m_pSettingsMenu))
  {
    m_pSettingsMenu->OnMenuClosed.RemoveAll(this);
  }

  if (IsValid(m_pControlsScreen))
  {
    m_pControlsScreen->OnMenuClosed.RemoveAll(this);
  }

  Super::NativeDestruct();
}

void UMainSettingsMenu::Show()
{
  m_bIsVisible = true;
  m_sCurrentLanguage = FInternationalization::Get().GetCurrentCulture()->GetName();

  HideControlsScreen();
  ShowGeneralSettingsScreen();

  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Visible);
  }
}

void UMainSettingsMenu::Hide()
{
  if (!m_bIsVisible)
  {
    return;
  }

  FString sSelectedLanguage = FInternationalization::Get().GetCurrentCulture()->GetName();
  if (m_bReloadLevelEnabled && IsValid(m_pConfirmationWindow) && (!m_sCurrentLanguage.Equals(sSelectedLanguage)))
  {
    m_pConfirmationWindow->OnConfirmResult.AddUObject(this, &UMainSettingsMenu::ConfirmationResult);
    m_pConfirmationWindow->Show();
  }
  else
  {
    FinishToHide();

    if (IsValid(m_pCanvasPanel))
    {
      m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
    }

    OnMenuClosed.ExecuteIfBound();
  }
}

bool UMainSettingsMenu::CanExit()
{
  if (!m_bIsVisible)
  {
    return true;
  }

  FString sSelectedLanguage = FInternationalization::Get().GetCurrentCulture()->GetName();
  if (m_bReloadLevelEnabled && IsValid(m_pConfirmationWindow) && (!m_pConfirmationWindow->IsWidgetVisible()) && (!m_sCurrentLanguage.Equals(sSelectedLanguage)))
  {
    m_pConfirmationWindow->OnConfirmResult.AddUObject(this, &UMainSettingsMenu::ConfirmationResult);
    m_pConfirmationWindow->Show();
  }

  return m_bReloadLevelEnabled && IsValid(m_pConfirmationWindow) ? !m_pConfirmationWindow->IsWidgetVisible() : true;
}

void UMainSettingsMenu::ChangeTab(bool _bGoRight)
{
  if (IsValid(m_pConfirmationWindow) && m_pConfirmationWindow->IsWidgetVisible())
  {
    return;
  }

  if (_bGoRight)
  {
    HideGeneralSettingsScreen();
    ShowControlsScreen();
  }
  else
  {
    HideControlsScreen();
    ShowGeneralSettingsScreen();
  }
}

void UMainSettingsMenu::FinishToHide()
{
  if (IsValid(m_pSettingsMenu))
  {
    m_pSettingsMenu->SaveSettings();
  }
  HideGeneralSettingsScreen();
  HideControlsScreen();
  m_bIsVisible = false;
}

void UMainSettingsMenu::ShowGeneralSettingsScreen()
{
  if (IsValid(m_pSettingsMenu) && (!m_pSettingsMenu->IsWidgetVisible()))
  {
    m_pSettingsMenu->Show();
  }
}

void UMainSettingsMenu::HideGeneralSettingsScreen()
{
  if (IsValid(m_pSettingsMenu) && m_pSettingsMenu->IsWidgetVisible())
  {
    m_pSettingsMenu->Hide();
  }
}

void UMainSettingsMenu::ShowControlsScreen()
{
  if (IsValid(m_pControlsScreen) && (!m_pControlsScreen->IsWidgetVisible()))
  {
    m_pControlsScreen->Show();
  }
}

void UMainSettingsMenu::HideControlsScreen()
{
  if (IsValid(m_pControlsScreen) && m_pControlsScreen->IsWidgetVisible())
  {
    m_pControlsScreen->Hide();
  }
}

void UMainSettingsMenu::ConfirmationResult(bool _bResult)
{
  if (IsValid(m_pConfirmationWindow))
  {
    m_pConfirmationWindow->OnConfirmResult.RemoveAll(this);
  }

  if (!_bResult)
  {
    if (IsValid(m_pSettingsMenu) && m_pSettingsMenu->IsWidgetVisible())
    {
      m_pSettingsMenu->SetupFocus();
    }
    else if (IsValid(m_pControlsScreen) && m_pControlsScreen->IsWidgetVisible())
    {
      m_pControlsScreen->SetupFocus();
    }
    return;
  }

  if (IsValid(m_pFadeOutAnimation))
  {
    PlayAnimation(m_pFadeOutAnimation);
  }

  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
    UAudioManager* pAudioManager = IsValid(pGameInstance) ? pGameInstance->GetSubsystem<UAudioManager>() : nullptr;
    if (IsValid(pAudioManager))
    {
      pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
    }
    UGameplayStatics::OpenLevelBySoftObjectPtr(pWorld, pWorld->GetCurrentLevel());
  }

  FinishToHide();
}