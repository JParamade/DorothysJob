#include "LoadLevelButton.h"
#include "Kismet/GameplayStatics.h"
#include "FMODEvent.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

void ULoadLevelButton::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  this->OnClicked.AddUniqueDynamic(this, &ULoadLevelButton::OnClick);
}

void ULoadLevelButton::Show()
{
  SetVisibility(ESlateVisibility::Visible);
}

void ULoadLevelButton::Hide()
{
  SetVisibility(ESlateVisibility::Collapsed);
}

void ULoadLevelButton::OnClick()
{
  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    UGameplayStatics::OpenLevelBySoftObjectPtr(pWorld, m_pLevel);
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
        pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/Hub/SFX_HUB_exitDoor_startGame.SFX_HUB_exitDoor_startGame")));
      }
    }
  }

  if (m_pLevel.GetAssetName() == "L_MainMenu")
  {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        pAchSubsystem->Save2hTimeAchievement();
      }
    }
  }
}