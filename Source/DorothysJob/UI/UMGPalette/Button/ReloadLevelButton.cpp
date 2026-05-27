/************************************************************************
 * @description: Implementation of the Reload Level button.
 * @author: Josephine Esposito
 * @date: 25/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/
#include "ReloadLevelButton.h"
#include "Kismet/GameplayStatics.h"
 // Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

// FMOD
#include "FMODEvent.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

void UReloadLevelButton::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  this->OnClicked.AddUniqueDynamic(this, &UReloadLevelButton::OnClick);
}

void UReloadLevelButton::Show()
{
  SetVisibility(ESlateVisibility::Visible);
}

void UReloadLevelButton::Hide()
{
  SetVisibility(ESlateVisibility::Collapsed);
}

void UReloadLevelButton::OnClick()
{
  // reload the current level here
  UWorld* pWorld = GetWorld();
  if (pWorld)
  {
    // get the current level name
    FName LevelName = pWorld->GetCurrentLevel()->GetOuter()->GetFName();

    // reload the level
    UGameplayStatics::OpenLevel(pWorld, LevelName);
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) 
    {
      if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) 
      {
        pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/Hub/SFX_HUB_exitDoor_startGame.SFX_HUB_exitDoor_startGame")));
        pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
      }
      pGameInstance->m_iCurrentEndlessLevel = -1;
    }
  }
}

//EOF