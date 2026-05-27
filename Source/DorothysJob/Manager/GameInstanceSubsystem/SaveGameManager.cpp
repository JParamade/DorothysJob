
#include "SaveGameManager.h"

#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Manager/DorothysJobSaveGame.h"
#include "DorothysJob/Manager/SettingsSaveGame.h"
#include "DorothysJob/Manager/AchievementsSaveGame.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"
#include "DorothysJob/Data/DataTables/LevelData.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Internationalization/Culture.h"


void USaveGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  m_sSaveSlotName = TEXT("DorothysJob");
  m_sSettingsSlotName = TEXT("Settings");
  m_sAchievementsName = TEXT("Achievements");
  m_pCurrentSaveGame = nullptr;
  m_pCurrentSaveSettings = nullptr;
  m_pCurrentSaveAchievements = nullptr;
}

bool USaveGameManager::SaveCurrentLevel(int32 _iCurrentLevel, int32 _iMaxLevel)
{
  if (IsValid(m_pCurrentSaveGame))
  {
    m_pCurrentSaveGame->m_iSavedCurrentLevel = _iCurrentLevel;
    if (_iMaxLevel >= 0) m_pCurrentSaveGame->m_iSavedHighestLevelAchieved = _iMaxLevel;
    return UGameplayStatics::SaveGameToSlot(m_pCurrentSaveGame, m_sSaveSlotName, 0);
  }
  return false;
}

bool USaveGameManager::LoadCurrentLevel()
{
  if (!IsValid(GetWorld()))
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager LoadCurrentLevel: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance)
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager LoadCurrentLevel: pBaseGameInstance is null"));
    return false;
  }

  if (UGameplayStatics::DoesSaveGameExist(m_sSaveSlotName, 0))
  {
    m_pCurrentSaveGame = Cast<UDorothysJobSaveGame>(UGameplayStatics::LoadGameFromSlot(m_sSaveSlotName, 0));

    if (IsValid(m_pCurrentSaveGame))
    {
      pBaseGameInstance->m_iCurrentLevel = m_pCurrentSaveGame->m_iSavedCurrentLevel;
      pBaseGameInstance->m_iHighestLevelAchieved = m_pCurrentSaveGame->m_iSavedHighestLevelAchieved;
      pBaseGameInstance->UpdateAvailableWeaponsAndConsumables(pBaseGameInstance->m_iCurrentLevel);
      return true;
    }
  }
  else
  {
    m_pCurrentSaveGame = Cast<UDorothysJobSaveGame>(UGameplayStatics::CreateSaveGameObject(UDorothysJobSaveGame::StaticClass()));
    pBaseGameInstance->m_iCurrentLevel = 0;
    pBaseGameInstance->m_iHighestLevelAchieved = 0;
    pBaseGameInstance->UpdateAvailableWeaponsAndConsumables(0);
    pBaseGameInstance->m_lSelectedWeapons.Empty();
    for (const UWeaponDataAsset* pWeaponAsset : pBaseGameInstance->m_lWeapons)
    {
      pBaseGameInstance->m_lSelectedWeapons.Add(pWeaponAsset);
    }

    UE_LOG(LogLoad, Warning, TEXT("CAN'T LOAD: NEW SAVEGAME FILE CREATED"));
  }

  return false;
}

bool USaveGameManager::SaveWeaponsBuild()
{
  if (!GetWorld())
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager SaveWeaponsBuild: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (m_pCurrentSaveGame && pBaseGameInstance)
  { 
    if (pBaseGameInstance->m_lSelectedWeapons == m_pCurrentSaveGame->m_lSavedSelectedWeapons)
    {
      UE_LOG(LogLoad, Warning, TEXT("DIDN'T SAVE BECAUSE SELECTED WEAPONS HAVEN'T CHANGED"))
      return false;
    }

    m_pCurrentSaveGame->m_lSavedSelectedWeapons.Empty();
    for (const UWeaponDataAsset* pWeaponAsset : pBaseGameInstance->m_lSelectedWeapons)
    {
      m_pCurrentSaveGame->m_lSavedSelectedWeapons.Add(pWeaponAsset);
    }   
    return UGameplayStatics::SaveGameToSlot(m_pCurrentSaveGame, m_sSaveSlotName, 0);
  }
  return false;
}

bool USaveGameManager::LoadWeaponsBuild()
{
  if (!GetWorld())
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager LoadWeaponsBuild: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (UGameplayStatics::DoesSaveGameExist(m_sSaveSlotName, 0) && pBaseGameInstance)
  {
    m_pCurrentSaveGame = Cast<UDorothysJobSaveGame>(UGameplayStatics::LoadGameFromSlot(m_sSaveSlotName, 0));

    if (IsValid(m_pCurrentSaveGame) && !m_pCurrentSaveGame->m_lSavedSelectedWeapons.IsEmpty())
    {
      pBaseGameInstance->m_lSelectedWeapons.Empty();
      for (const UWeaponDataAsset* pWeaponAsset : m_pCurrentSaveGame->m_lSavedSelectedWeapons)
      {
        pBaseGameInstance->m_lSelectedWeapons.Add(pWeaponAsset);
      }
      return true;
    }
  }
  return false;
}

bool USaveGameManager::SaveConsumablesBuild()
{
  if (!GetWorld())
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager SaveConsumablesBuild: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (m_pCurrentSaveGame && pBaseGameInstance)
  {
    if (pBaseGameInstance->m_lSelectedConsumables == m_pCurrentSaveGame->m_lSavedSelectedConsumables)
    {
      UE_LOG(LogLoad, Warning, TEXT("DIDN'T SAVE BECAUSE SELECTED CONSUMABLES HAVEN'T CHANGED"))
        return false;
    }

    m_pCurrentSaveGame->m_lSavedSelectedConsumables.Empty();
    for (const UConsumableDataAsset* pConsumableAsset : pBaseGameInstance->m_lSelectedConsumables)
    {
      m_pCurrentSaveGame->m_lSavedSelectedConsumables.Add(pConsumableAsset);
    }
    return UGameplayStatics::SaveGameToSlot(m_pCurrentSaveGame, m_sSaveSlotName, 0);
  }
  return false;
}

bool USaveGameManager::LoadConsumablesBuild()
{
  if (!GetWorld())
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager LoadConsumablesBuild: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (UGameplayStatics::DoesSaveGameExist(m_sSaveSlotName, 0) && pBaseGameInstance)
  {
    m_pCurrentSaveGame = Cast<UDorothysJobSaveGame>(UGameplayStatics::LoadGameFromSlot(m_sSaveSlotName, 0));
    if (IsValid(m_pCurrentSaveGame) && !m_pCurrentSaveGame->m_lSavedSelectedConsumables.IsEmpty())
    {
      pBaseGameInstance->m_lSelectedConsumables.Empty();
      for (const UConsumableDataAsset* pConsumableAsset : m_pCurrentSaveGame->m_lSavedSelectedConsumables)
      {
        pBaseGameInstance->m_lSelectedConsumables.Add(pConsumableAsset);
      }
      return true;
    }
  }
  return false;
}

bool USaveGameManager::SaveEventsTriggers()
{
  if (!GetWorld())
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager SaveConsumablesBuild: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (m_pCurrentSaveGame && pBaseGameInstance)
  {
    m_pCurrentSaveGame->m_oSavedEventUseData.Empty();
    m_pCurrentSaveGame->m_oSavedHUDData.Empty();
    pBaseGameInstance->SyncFirstUseDataFromAsset();
    m_pCurrentSaveGame->m_oSavedEventUseData = pBaseGameInstance->m_oEventUseData;
    m_pCurrentSaveGame->m_oSavedHUDData = pBaseGameInstance->m_oHUDData;
    m_pCurrentSaveGame->m_mSavedHubEventTriggered = pBaseGameInstance->m_oHubEventTriggered;

    return UGameplayStatics::SaveGameToSlot(m_pCurrentSaveGame, m_sSaveSlotName, 0);
  }
  return false;
}

bool USaveGameManager::LoadEventsTriggers()
{
  if (!GetWorld())
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager LoadConsumablesBuild: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (UGameplayStatics::DoesSaveGameExist(m_sSaveSlotName, 0) && pBaseGameInstance)
  {
    m_pCurrentSaveGame = Cast<UDorothysJobSaveGame>(UGameplayStatics::LoadGameFromSlot(m_sSaveSlotName, 0));
    if (IsValid(m_pCurrentSaveGame) && (!m_pCurrentSaveGame->m_oSavedEventUseData.IsEmpty() || !m_pCurrentSaveGame->m_oSavedHUDData.IsEmpty()))
    {
      pBaseGameInstance->m_oEventUseData.Empty();
      pBaseGameInstance->m_oHUDData.Empty();
      pBaseGameInstance->m_oHubEventTriggered.Empty();
      pBaseGameInstance->m_oEventUseData = m_pCurrentSaveGame->m_oSavedEventUseData;
      pBaseGameInstance->m_oHUDData = m_pCurrentSaveGame->m_oSavedHUDData;
      pBaseGameInstance->UpdateDataAssetFromFirstUseData();
      pBaseGameInstance->m_oHubEventTriggered = m_pCurrentSaveGame->m_mSavedHubEventTriggered;
      return true;
    }
  }
  return false;
}

bool USaveGameManager::SaveBestFinalScore(EScore _eScore, int32 _iDay, bool _bHit, bool m_bExtraTime)
{
  if (IsValid(m_pCurrentSaveGame) && m_pCurrentSaveGame->m_lFinalScores.IsValidIndex(_iDay-1))
  {
    FFinalSavedScore& FS = m_pCurrentSaveGame->m_lFinalScores[_iDay - 1];

    if (_eScore < FS.m_eScore)
    {
      FS.m_eScore = _eScore;
    }

    if (!_bHit) FS.m_bHasBeenHit = false;
    if (m_bExtraTime) FS.m_bIsTimerExtra = true;
    return UGameplayStatics::SaveGameToSlot(m_pCurrentSaveGame, m_sSaveSlotName, 0);
  }
  return false;
}

FFinalSavedScore USaveGameManager::GetBestFinalScore(int32 _iDay)
{
  if (IsValid(m_pCurrentSaveGame) && m_pCurrentSaveGame->m_lFinalScores.IsValidIndex(_iDay - 1))
  {
    return m_pCurrentSaveGame->m_lFinalScores[_iDay - 1];
  }
  return FFinalSavedScore();
}

TArray<FFinalSavedScore> USaveGameManager::GetBestFinalScoreList()
{
  if (IsValid(m_pCurrentSaveGame))
  {
    return m_pCurrentSaveGame->m_lFinalScores;
  }
  return {};
}

bool USaveGameManager::SaveSettings(TArray<UObject*> _lManagers)
{
  if (IsValid(m_pCurrentSaveSettings))
  {
    m_pCurrentSaveSettings->m_lSettingsData.Empty();
    for (UObject* manager : _lManagers)
    {
      if (IsValid(manager))
      {
        FSettingsData settingsData;
        FMemoryWriter memWriter(settingsData.m_lSettingsManagerData);
        FObjectAndNameAsStringProxyArchive arProxy(memWriter, true);
        arProxy.ArIsSaveGame = true;
        arProxy.ArNoDelta = true;
        manager->Serialize(arProxy);
        m_pCurrentSaveSettings->m_lSettingsData.Add(manager->GetFName(), settingsData);
      }
    }
    m_pCurrentSaveSettings->m_sGameLanguage = FInternationalization::Get().GetCurrentCulture()->GetName();
    return UGameplayStatics::SaveGameToSlot(m_pCurrentSaveSettings, m_sSettingsSlotName, 0);;
  }
  return false;
}

bool USaveGameManager::LoadSettings(TArray<UObject*> _lManagers)
{
  if (UGameplayStatics::DoesSaveGameExist(m_sSettingsSlotName, 0))
  {
    m_pCurrentSaveSettings = Cast<USettingsSaveGame>(UGameplayStatics::LoadGameFromSlot(m_sSettingsSlotName, 0));
    if (m_pCurrentSaveSettings)
    {
      for (UObject* manager : _lManagers)
      {
        if (IsValid(manager) && m_pCurrentSaveSettings->m_lSettingsData.Contains(manager->GetFName()))
        {
          FSettingsData settingsData;
          settingsData = m_pCurrentSaveSettings->m_lSettingsData.FindRef(manager->GetFName());
          FMemoryReader memReader(settingsData.m_lSettingsManagerData);
          FObjectAndNameAsStringProxyArchive arProxy(memReader, true);
          arProxy.ArIsSaveGame = true;
          arProxy.ArNoDelta = true;
          manager->Serialize(arProxy);
        }
      }
      FInternationalization::Get().SetCurrentLanguageAndLocale(m_pCurrentSaveSettings->m_sGameLanguage);
      return true;
    }
  }
  m_pCurrentSaveSettings = Cast<USettingsSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingsSaveGame::StaticClass()));
  return false;
}

bool USaveGameManager::SaveAchievements()
{
  if (!GetWorld())
  {
    UE_LOG(LogLoad, Warning, TEXT("SaveGameManager SaveAchievements: UWorld is null"));
    return false;
  }

  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());

  if (IsValid(m_pCurrentSaveAchievements) && pBaseGameInstance)
  {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>())
    {
      m_pCurrentSaveAchievements->m_lAchievementManagerData.Empty();
      TArray<uint8> lAchievementManagerData;
      FMemoryWriter memWriter(lAchievementManagerData);
      FObjectAndNameAsStringProxyArchive arProxy(memWriter, true);
      arProxy.ArIsSaveGame = true;
      arProxy.ArNoDelta = true;
      pAchSubsystem->Serialize(arProxy);
      m_pCurrentSaveAchievements->m_lAchievementManagerData = lAchievementManagerData;
    }
    return UGameplayStatics::SaveGameToSlot(m_pCurrentSaveAchievements, m_sAchievementsName, 0);;
  }
  return false;
}

bool USaveGameManager::LoadAchievements()
{
  if (UGameplayStatics::DoesSaveGameExist(m_sAchievementsName, 0))
  {
    if (!GetWorld())
    {
      UE_LOG(LogLoad, Warning, TEXT("SaveGameManager SaveAchievements: UWorld is null"));
      return false;
    }

    UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());

    m_pCurrentSaveAchievements = Cast<UAchievementsSaveGame>(UGameplayStatics::LoadGameFromSlot(m_sAchievementsName, 0));
    if (IsValid(m_pCurrentSaveAchievements) && pBaseGameInstance)
    {
      if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>())
      {
        TArray<uint8> lAchievementManagerData = m_pCurrentSaveAchievements->m_lAchievementManagerData;
        FMemoryReader memReader(lAchievementManagerData);
        FObjectAndNameAsStringProxyArchive arProxy(memReader, true);
        arProxy.ArIsSaveGame = true;
        arProxy.ArNoDelta = true;
        pAchSubsystem->Serialize(arProxy);
      }
      return true;
    }
  }
  m_pCurrentSaveAchievements = Cast<UAchievementsSaveGame>(UGameplayStatics::CreateSaveGameObject(UAchievementsSaveGame::StaticClass()));
  return false;
}

bool USaveGameManager::LoadAllGame()
{
  LoadWeaponsBuild();
  LoadConsumablesBuild();
  LoadEventsTriggers();
  return LoadCurrentLevel();
}

bool USaveGameManager::DeleteSave() const
{
  if (GetWorld())
  {
    if (UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance()))
    {
      pBaseGameInstance->ResetTriggersUseData();
      pBaseGameInstance->UpdateAvailableWeaponsAndConsumables(0);
      pBaseGameInstance->ResetSelectedWeaponsAndConsumables();
      if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>())
      {
        pAchSubsystem->m_fActualGameTime = 0.f;
        pAchSubsystem->m_bNoStinkyReload = true;
      }

    }
  }
  if (UGameplayStatics::DoesSaveGameExist(m_sSaveSlotName, 0))
  {
    return UGameplayStatics::DeleteGameInSlot(m_sSaveSlotName, 0);
  }
  else
  {
    UE_LOG(LogLoad, Error, TEXT("TRYING TO DELETE AN INEXISTING SAVEGAME FILE"));
  }
  return false;
}

bool USaveGameManager::CheckSaveGameExists() const
{
  return UGameplayStatics::DoesSaveGameExist(m_sSaveSlotName, 0);
}