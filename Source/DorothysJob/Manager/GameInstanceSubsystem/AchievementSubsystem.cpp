#include "AchievementSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystemSteam.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/OnlineStatsInterface.h"
#include "Misc/ConfigCacheIni.h"
#include "Interfaces/OnlineAchievementsInterface.h"
#include "steam/steam_api.h"

// Includes for the specific achievements.
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Manager/WorldSubsystem/TimeManager.h"

void UAchievementSubsystem::Initialize(FSubsystemCollectionBase& _rCollection) {
  Super::Initialize(_rCollection);

  _rCollection.InitializeDependency<USaveGameManager>();
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    m_pSaveGameManager = pGameInstance->GetSubsystem<USaveGameManager>();
  }

  RegisterFromINI();
  LoadAchievements();

  IOnlineSubsystem* pOSS = IOnlineSubsystem::Get(TEXT("STEAM"));
  if (pOSS) {
    m_pCachedAchievementsInterface = pOSS->GetAchievementsInterface();
    if (m_pCachedAchievementsInterface.IsValid()) {
      UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] IOnlineAchievementsInterface loaded SUCCESSFULLY."));
    }
    else UE_LOG(LogTemp, Error, TEXT("[ACHIEVEMENTS] IOnlineAchievementsInterface is NULL."));

    IOnlineIdentityPtr pIdentity = pOSS->GetIdentityInterface();
    if (pIdentity.IsValid()) {
      m_pCachedPlayerId = pIdentity->GetUniquePlayerId(0);
      if (m_pCachedPlayerId.IsValid()) {
        UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Player ID %s loaded SUCCESSFULLY."), *m_pCachedPlayerId->ToString());

        if (m_pCachedAchievementsInterface.IsValid()) {
          m_pCachedAchievementsInterface->QueryAchievements(*m_pCachedPlayerId,
            FOnQueryAchievementsCompleteDelegate::CreateUObject(this, &UAchievementSubsystem::OnQueryAchievementsComplete));
          UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] QueryAchievements requested."));
        }
      }
      else UE_LOG(LogTemp, Error, TEXT("[ACHIEVEMENTS] Player ID is NULL."));
    }
    else UE_LOG(LogTemp, Error, TEXT("[ACHIEVEMENTS] IOnlineIdentityInterface is NULL."));
  }
  else UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] IOnlineSubsystem::Get(TEXT(\"STEAM\")) returned null on Initialize."));
}


void UAchievementSubsystem::RegisterAchievement(const FString& _rAchievementId, int32 _iMaxValue, const FString& _rStatName) {
  if (_rAchievementId.IsEmpty()) return;

  FName sAchievementName = FName(*_rAchievementId);

  if (m_mAchievements.Contains(sAchievementName)) {
    UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] Achievement ID '%s' already registered."), *_rAchievementId);
    return;
  }

  FAchievementData oData;
  oData.AchievementId = _rAchievementId;
  oData.MaxValue = FMath::Max(1, _iMaxValue);
  oData.StatName = _rStatName;
  oData.CurrentValue = 0;
  oData.bUnlocked = false;

  m_mAchievements.Add(sAchievementName, oData);
  UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Registered '%s' (MaxValue=%d, StatName=%s)"), *_rAchievementId, oData.MaxValue, *_rStatName);
}

void UAchievementSubsystem::UnlockAchievement(const FString& _rAchievementId) {
  FAchievementData* pData = FindAchievement(_rAchievementId);
  if (!pData) {
    UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] UnlockAchievement: Cannot find achievement '%s'."), *_rAchievementId);
    return;
  }
  if (pData->bUnlocked) return;

  if (pData->MaxValue > 1) {
    UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] UnlockAchievement called on progression achievement '%s'. Force unlocking."), *_rAchievementId);
  }

  pData->CurrentValue = pData->MaxValue;
  pData->bUnlocked = true;

  if (AreAllAchievementsUnlocked() && _rAchievementId != "DorothysWorld") UnlockAchievement("DorothysWorld");

  if (!m_bSteamAchievementsReady) {
    UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] Steam not ready yet. Queuing achievement %s."), *_rAchievementId);
    m_lPendingUnlocks.AddUnique(_rAchievementId);
  }
  else WriteAchievementToSteam(*pData);

  OnAchievementUnlocked.Broadcast(_rAchievementId);
}

void UAchievementSubsystem::AddProgress(const FString& _rAchievementId, int32 _iAmount) {
  if (_iAmount <= 0) return;

  FAchievementData* pData = FindAchievement(_rAchievementId);
  if (!pData) {
    UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] AddProgress: Cannot find achievement '%s'."), *_rAchievementId);
    return;
  }
  if (pData->bUnlocked) return;

  if (pData->StatName.IsEmpty()) {
    UE_LOG(LogTemp, Error, TEXT("[ACHIEVEMENTS] AddProgress failed: Achievement '%s' has no StatName in the .ini file."), *_rAchievementId);
    return;
  }
  if (pData->MaxValue <= 1) {
    UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] AddProgress called on non-progression achievement '%s'. Use UnlockAchievement instead."), *_rAchievementId);
    UnlockAchievement(_rAchievementId);
    return;
  }

  int32 iOldValue = pData->CurrentValue;
  pData->CurrentValue = FMath::Clamp(pData->CurrentValue + _iAmount, 0, pData->MaxValue);

  if (pData->CurrentValue == iOldValue) return;

  UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Progress for '%s' modified: %d/%d."),
    *_rAchievementId, pData->CurrentValue, pData->MaxValue);

  bool bJustUnlocked = false;
  if (pData->CurrentValue >= pData->MaxValue && !pData->bUnlocked) {
    pData->bUnlocked = true;
    bJustUnlocked = true;

    if (AreAllAchievementsUnlocked()) UnlockAchievement("DorothysWorld");
  }

  if (!m_bSteamAchievementsReady) {
    UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] Steam not ready. Queuing sync for %s."), *_rAchievementId);
    m_lPendingUnlocks.AddUnique(_rAchievementId);
  }
  else WriteAchievementToSteam(*pData);

  OnAchievementProgressChanged.Broadcast(_rAchievementId, pData->CurrentValue);
  if (bJustUnlocked) OnAchievementUnlocked.Broadcast(_rAchievementId);
}

int32 UAchievementSubsystem::GetProgress(const FString& _rAchievementId) const {
  const FAchievementData* pData = m_mAchievements.Find(*_rAchievementId);
  return pData ? pData->CurrentValue : 0;
}

bool UAchievementSubsystem::IsUnlocked(const FString& _rAchievementId) const {
  const FAchievementData* pData = m_mAchievements.Find(*_rAchievementId);
  return pData ? pData->bUnlocked : false;
}

void UAchievementSubsystem::SaveAchievements() {
  if (m_pSaveGameManager) m_pSaveGameManager->SaveAchievements();
}

void UAchievementSubsystem::LoadAchievements() {
  if (m_pSaveGameManager) m_pSaveGameManager->LoadAchievements();
}

void UAchievementSubsystem::RegisterFromINI() {
  const FString sSection = TEXT("OnlineSubsystemSteam");

  for (int32 iIndex = 0; iIndex < 256; ++iIndex) {
    FString sIdKey = FString::Printf(TEXT("Achievement_%d_Id"), iIndex);
    FString sIdValue;
    if (GConfig->GetString(*sSection, *sIdKey, sIdValue, GEngineIni) && !sIdValue.IsEmpty()) {
      sIdValue.TrimStartAndEndInline();

      int32 iMaxValue = 1;
      FString sMaxKey = FString::Printf(TEXT("Achievement_%d_Max"), iIndex);
      GConfig->GetInt(*sSection, *sMaxKey, iMaxValue, GEngineIni);

      FString sStatValue;
      FString sStatKey = FString::Printf(TEXT("Achievement_%d_Stat"), iIndex);
      GConfig->GetString(*sSection, *sStatKey, sStatValue, GEngineIni);

      sStatValue.TrimStartAndEndInline();
      RegisterAchievement(sIdValue, iMaxValue, sStatValue);
    }
  }
}

#if WITH_EDITOR
void UAchievementSubsystem::ResetStats(const FUniqueNetId& _rUserId) {
  if (m_pCachedAchievementsInterface.IsValid()) m_pCachedAchievementsInterface->ResetAchievements(_rUserId);

  for (auto& rElement : m_mAchievements) {
    rElement.Value.CurrentValue = 0;
    rElement.Value.bUnlocked = false;

    WriteAchievementToSteam(rElement.Value);

    OnAchievementProgressChanged.Broadcast((rElement.Key).ToString(), 0);
  }

  UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] Stats and local achievements reset!"));
}

void UAchievementSubsystem::ResetStats() {
  if (!m_pCachedPlayerId.IsValid()) return;
  if (m_pCachedAchievementsInterface.IsValid()) m_pCachedAchievementsInterface->ResetAchievements(*m_pCachedPlayerId);

  for (auto& rElement : m_mAchievements) {
    rElement.Value.CurrentValue = 0;
    rElement.Value.bUnlocked = false;

    WriteAchievementToSteam(rElement.Value);

    OnAchievementProgressChanged.Broadcast((rElement.Key).ToString(), 0);
  }

  UE_LOG(LogTemp, Warning, TEXT("[ACHIEVEMENTS] Stats and local achievements reset!"));
}
#endif

FAchievementData* UAchievementSubsystem::FindAchievement(const FString& _rAchievementId) {
  return m_mAchievements.Find(*_rAchievementId);
}

void UAchievementSubsystem::WriteAchievementToSteam(const FAchievementData& _rData) {
  if(!m_pCachedAchievementsInterface.IsValid() || !m_pCachedPlayerId.IsValid()) {
    UE_LOG(LogTemp, Error, TEXT("[ACHIEVEMENTS] WriteAchievementProgress FAILED: Invalid AchievementsInterface or PlayerId."));
    return;
  }

  FOnlineAchievementsWriteRef pWriteRef = MakeShared<FOnlineAchievementsWrite>();

  if (_rData.bUnlocked) pWriteRef->SetFloatStat(_rData.AchievementId, 100.f);
  else {
    if (!_rData.StatName.IsEmpty()) {
      if (SteamUserStats()) {
        SteamUserStats()->SetStat(TCHAR_TO_ANSI(*_rData.StatName), _rData.CurrentValue);
        SteamUserStats()->StoreStats();
      }
    }
  }

  m_pCachedAchievementsInterface->WriteAchievements(
    *m_pCachedPlayerId,
    pWriteRef,
    FOnAchievementsWrittenDelegate::CreateLambda(
      [AchievementId = _rData.AchievementId](const FUniqueNetId& InPlayerId, bool bSuccess) {
        if (bSuccess) {
          UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Successfully wrote UI progress for '%s'."), *AchievementId);
        }
        else {
          UE_LOG(LogTemp, Error, TEXT("[ACHIEVEMENTS] FAILED to write UI progress for '%s'."), *AchievementId);
        }
      }
    )
  );
}

void UAchievementSubsystem::OnQueryAchievementsComplete(const FUniqueNetId& _rUserId, bool _bWasSuccessful) {
  if (!_bWasSuccessful || !m_pCachedAchievementsInterface.IsValid()) {
    UE_LOG(LogTemp, Error, TEXT("[ACHIEVEMENTS] Failed to load Steam achievement data."));
    return;
  }

  UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Syncing achievements from Steam for player %s."), *_rUserId.ToString());

  TArray<FOnlineAchievement> lSteamAchievements;
  m_pCachedAchievementsInterface->GetCachedAchievements(_rUserId, lSteamAchievements);

  for (FOnlineAchievement& rSteamAch : lSteamAchievements) {
    FString sId = rSteamAch.Id;
    FAchievementData* pLocal = FindAchievement(sId);
    if (pLocal) {
      int32 iSteamValue = 0;
      
      if (rSteamAch.Progress >= 100.f) iSteamValue = pLocal->MaxValue;
      else SteamUserStats()->GetStat(TCHAR_TO_ANSI(*pLocal->StatName), &iSteamValue);

      if (iSteamValue > pLocal->CurrentValue) {
        UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Syncing '%s': Steam progress (%.0f%% -> %d) is newer than local (%d)."),
          *sId, rSteamAch.Progress, iSteamValue, pLocal->CurrentValue);
        pLocal->CurrentValue = iSteamValue;
      }
      else if (pLocal->CurrentValue > iSteamValue) {
        UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Syncing '%s': Local progress (%d) is newer than Steam (%.0f%% -> %d)."),
          *sId, pLocal->CurrentValue, rSteamAch.Progress, iSteamValue);
      }

      pLocal->bUnlocked = (pLocal->CurrentValue >= pLocal->MaxValue);

      OnAchievementProgressChanged.Broadcast(sId, pLocal->CurrentValue);
      if (pLocal->bUnlocked) OnAchievementUnlocked.Broadcast(sId);
    }
  }

  m_bSteamAchievementsReady = true;
  UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Steam achievements are ready. Flushing %d pending items..."), m_lPendingUnlocks.Num());

  for (const FString& sId : m_lPendingUnlocks) {
    FAchievementData* pData = FindAchievement(sId);
    if (pData) {
      UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Flushing pending update for '%s'."), *sId);
      WriteAchievementToSteam(*pData);
    }
  }
  m_lPendingUnlocks.Empty();

  UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Checking for local progress ahead of Steam..."));
  for (auto& rElement : m_mAchievements) {
    FAchievementData& rData = rElement.Value;
    float fSteamProgress = 0.f;
    FOnlineAchievement* pSteamAch = lSteamAchievements.FindByPredicate([&](const FOnlineAchievement& Ach) { return Ach.Id == rData.AchievementId; });
    if (pSteamAch) fSteamProgress = pSteamAch->Progress;

    float fLocalProgressPercent = (float)rData.CurrentValue / (float)rData.MaxValue * 100.0f;

    if (fLocalProgressPercent > fSteamProgress + 0.1f) {
      UE_LOG(LogTemp, Log, TEXT("[ACHIEVEMENTS] Local progress for %s (%.0f%%) is ahead of Steam (%.0f%%). Re-syncing."),
        *rData.AchievementId, fLocalProgressPercent, fSteamProgress);
      WriteAchievementToSteam(rData);
    }
  }
}

bool UAchievementSubsystem::AreAllAchievementsUnlocked() {
  for (auto& rElement : m_mAchievements) {
    if (!rElement.Value.bUnlocked && rElement.Value.AchievementId != "DorothysWorld") return false;
  }

  return true;
}

void UAchievementSubsystem::ResetTemporalLevelVariables() {
  // Reset level variables for achievements.
  m_fLevelStartTime = 0.0f;
  m_iCleanRoomsObtain = 0;
  m_fDogsDeploy.Empty();
  m_bEquipApron = false;
  m_bApronUse = false;
}

#pragma region | TheUltimateArsenal Achievement 
void UAchievementSubsystem::SetPassLevelStinkyAchievement()
{
  ABasePlayer* pPlayer = Cast<ABasePlayer>(GetWorld()->GetPlayerControllerIterator()->Get()->GetPawn());
  if (pPlayer)
  {
    if (pPlayer->GetIsPlayerStinky())
    {
      UnlockAchievement("DirtyWork");
    }
  }
}
#pragma endregion

#pragma region | DirtyWork Achievement
void UAchievementSubsystem::SetWeaponCombinationAchievement()
{
  ABasePlayer* pPlayer = Cast<ABasePlayer>(GetWorld()->GetPlayerControllerIterator()->Get()->GetPawn());
  if (!pPlayer) return;

  const TArray<TObjectPtr<ABaseWeapon>>& CurrentWeapons = pPlayer->GetCurrentWeapons();
  if (!CurrentWeapons.IsValidIndex(0) || !CurrentWeapons.IsValidIndex(1)) return;

  const UBaseWeaponStatsDataAsset* LiquidWeaponData = CurrentWeapons[0] ? CurrentWeapons[0]->GetDataAsset().Get() : nullptr;
  const UBaseWeaponStatsDataAsset* DustWeaponData = CurrentWeapons[1] ? CurrentWeapons[1]->GetDataAsset().Get() : nullptr;

  if (!LiquidWeaponData || !DustWeaponData) return;

  EWeaponCombination Combination;

  if (LiquidWeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance &&
    DustWeaponData->m_eWeaponAttackType == EWeaponAttackType::Melee)
  {
    Combination = EWeaponCombination::WF_DUSTY;
  }
  else if (LiquidWeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance &&
    DustWeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance)
  {
    Combination = EWeaponCombination::WF_VAC;
  }
  else if (LiquidWeaponData->m_eWeaponAttackType == EWeaponAttackType::Melee &&
    DustWeaponData->m_eWeaponAttackType == EWeaponAttackType::Melee)
  {
    Combination = EWeaponCombination::MOP_DUSTY;
  }
  else if (LiquidWeaponData->m_eWeaponAttackType == EWeaponAttackType::Melee &&
    DustWeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance)
  {
    Combination = EWeaponCombination::MOP_VAC;
  }
  else
  {
    return;
  }

  if (!m_lAllWeaponCombination.Contains(Combination))
  {
    m_lAllWeaponCombination.Add(Combination);
  }

  if (m_lAllWeaponCombination.Num() == 4)
  {
    UnlockAchievement("TheUltimateArsenal");
  }
}
#pragma endregion

#pragma region | GGSqueakyClean Achievement 
void UAchievementSubsystem::SetGGSqueakyCleanAchievement()
{
  if (m_bNoStinkyReload)
  {
    UnlockAchievement("GGSqueakyClean");
  }
}
void UAchievementSubsystem::SetKillsAchievement(EWeaponsTypes _eWeaponLastAttackReceive)
{
  static const TMap<EWeaponsTypes, FString> AchievementMap = {
      { EWeaponsTypes::WF,          "ThatsGottaSoak" },
      { EWeaponsTypes::DUSTY,       "AbsolutelyTidyFying" },
      { EWeaponsTypes::MOPPRESSOR,  "SubjugateTheMesses" },
      { EWeaponsTypes::DIRTMINATOR, "PutThatFilterDown" }
  };

  if (const FString* AchievementName = AchievementMap.Find(_eWeaponLastAttackReceive))
  {
    AddProgress(*AchievementName, 1);
  }
  AddProgress("AnAbsoluteMessAcre", 1);
}
#pragma endregion

#pragma region | Kills With B13 Achievement
void UAchievementSubsystem::SetKillsWithB13()
{
  ABasePlayer* pPlayer = Cast<ABasePlayer>(GetWorld()->GetPlayerControllerIterator()->Get()->GetPawn());
  if (!pPlayer) return;

  if (pPlayer->GetIsPlayerWithB13())
  {
    AddProgress("CleaningPowerOverwhelming", 1);
  }
}
#pragma endregion

#pragma region | Mopped and Loaded Achievement
void UAchievementSubsystem::SetMoppedAndLoadedAchievement()
{
  float fElapsedTime = 0.f;

  if (m_iCleanRoomsObtain < 3)
  {
    m_iCleanRoomsObtain += 1;
  }

  if (m_iCleanRoomsObtain == 3)
  {
    float iCurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
    fElapsedTime = iCurrentTime - m_fLevelStartTime;

    if (fElapsedTime >= 60.0f)
    {
      UnlockAchievement("MoppedAndLoaded");
    }
  }
}
#pragma endregion

#pragma region | Paid Overtime Achievement
void UAchievementSubsystem::SetPaidOvertimeAchievement()
{
  UTimeManager* pTimeManager = GetWorld()->GetSubsystem<UTimeManager>();
  if (!pTimeManager)
  {
    return;
  }

  const float fExtraTime = pTimeManager->GetCurrentSeconds();

  if (m_iCleanRoomsObtain < 2 && fExtraTime <= 5.0f)
  {
    m_iCleanRoomsObtain += 1;
  }

  if (m_iCleanRoomsObtain == 2)
  {
    UnlockAchievement("PaidOvertime");
  }
}
#pragma endregion

#pragma region | A Maids Best Friends Achievement
void UAchievementSubsystem::SetDogsDeployAchievement()
{
  const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());

  m_fDogsDeploy.Add(CurrentTime);

  if (m_fDogsDeploy.Num() > 5)
  {
    m_fDogsDeploy.RemoveAt(0);
  }

  if (m_fDogsDeploy.Num() == 5)
  {
    float TimeSpan = m_fDogsDeploy.Last() - m_fDogsDeploy[0];

    if (TimeSpan <= 5.0f)
    {
      UnlockAchievement("AMaidsBestFriends");
      m_fDogsDeploy.Empty();
    }
  }
}
#pragma endregion

#pragma region | Nothing under my sleeves Achievement
void UAchievementSubsystem::Set2hTimeAchievement()
{
  float fCurrentGameTime = UGameplayStatics::GetTimeSeconds(GetWorld());

  m_fActualGameTime = m_fActualGameTime + (fCurrentGameTime - m_fStartGameTime);

  if (m_fActualGameTime <= 7200.f)
  {
    UnlockAchievement("NothingUnderMySleeves");
  }
}

void UAchievementSubsystem::Save2hTimeAchievement()
{
  float fCurrentGameTime = UGameplayStatics::GetTimeSeconds(GetWorld());

  m_fActualGameTime = m_fActualGameTime + (fCurrentGameTime - m_fStartGameTime);
}
#pragma endregion

#pragma region | Maids are like onions Achievement
void UAchievementSubsystem::SetNotUseApronAchievement()
{
  if (m_bEquipApron && !m_bApronUse)
  {
    UnlockAchievement("MaidsAreLikeOnions");
  }
}
#pragma endregion

#pragma region | Long live the Clean Achievement
void UAchievementSubsystem::SetWFSpecialAttackAchievement()
{
  const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());

  float fElapsedTime = CurrentTime - m_fWFSpecialStart;

  if (fElapsedTime >= 6.0f)
  {
    UnlockAchievement("LongLiveTheClean");
  }
}
#pragma endregion

#pragma region | CATEGORY 5 DUST-NADO Achievement
void UAchievementSubsystem::SetDustySAAchievement()
{
  if (m_bDustySpecialActive)
  {
    m_iDustPoopsClean += 1;

    if (m_iDustPoopsClean >= 5)
    {
      UnlockAchievement("Categoty5Dustnado");
    }
  }
}
#pragma endregion

#pragma region | Global Mopnopoly Achievement
void UAchievementSubsystem::SetMopSAAchievement()
{
  const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());

  m_fMopSpecialAttackActive.Add(CurrentTime);

  if (m_fMopSpecialAttackActive.Num() > 2)
  {
    m_fMopSpecialAttackActive.RemoveAt(0);
  }

  if (m_fMopSpecialAttackActive.Num() == 2)
  {
    float TimeSpan = m_fMopSpecialAttackActive.Last() - m_fMopSpecialAttackActive[0];

    if (TimeSpan <= 60.0f)
    {
      UnlockAchievement("GlobalMopnopoly");
      m_fMopSpecialAttackActive.Empty();
    }
  }
}
#pragma endregion

#pragma region | You are Dirtminated Achievement
void UAchievementSubsystem::SetDirtMaxChargeAchievement()
{
  if (m_bIsMaxChargeShot)
  {
    m_iMaxChargeDeaths += 1;

    if (m_iMaxChargeDeaths == 2)
    {
      UnlockAchievement("YouAreDirtminated");
    }
  }
}
#pragma endregion

#pragma region | Monster clean Achievement
void UAchievementSubsystem::SetClean4DeathsAchievement()
{
  if (m_bClean4Active)
  {
    m_iClean4Deaths += 1;

    if (m_iClean4Deaths >= 6)
    {
      UnlockAchievement("MonsterClean");
    }
  }
}
#pragma endregion

#pragma region | Go beyond flush ultra Achievement
void UAchievementSubsystem::SetKillsWith1B13Achievement()
{
  if (m_bUseB13)
  {
    m_iEnemyKillsWith1B13 += 1;

    if (m_iEnemyKillsWith1B13 == 10)
    {
      UnlockAchievement("GoBeyondFlushUltra");
    }
  }
}
#pragma endregion

#pragma region | Squeaky Mean Achievement
void UAchievementSubsystem::CheckSqueakyMeanAchievement()
{
  if (!IsValid(m_pSaveGameManager) || m_bSqueakyMean)
  {
    return;
  }

  TArray<FFinalSavedScore> lScores = m_pSaveGameManager->GetBestFinalScoreList();
  if (lScores.Num() <= 1)
  {
    return;
  }

  lScores.SetNum(lScores.Num() - 1);
  bool bNotAllS = lScores.ContainsByPredicate(
    [](const FFinalSavedScore& finalScore)
    {
      return finalScore.m_eScore > EScore::S;
    }
  );

  if (!bNotAllS)
  {
    UnlockAchievement("SqueakyMean");
    m_bSqueakyMean = true;
  }
}
#pragma endregion

#pragma region | Whats Yours Is Mine Achievement
void UAchievementSubsystem::SetClean4ChainAchievement()
{
  if (m_bBombActive)
  {
    m_fBombExplode += 1;

    if (m_fBombExplode == 4)
    {
      UnlockAchievement("WhatsYoursIsMine");
    }
  }
  else
  {
    m_fBombExplode = 0;
  }
}

#pragma endregion

#pragma region | Wash Wash, Womp Womp Achievement

void UAchievementSubsystem::SetDeathCounterAchievement()
{
  if (m_iDeathCounter >= 5)
  {
    UnlockAchievement("WashWashWompWomp");
  }
}
#pragma endregion

#pragma region | Divide and Worsen
void UAchievementSubsystem::ReportSlimeKill(EWeaponsTypes KillerWeapon)
{
  int32& Count = m_SimultaneousSlimeKillsThisFrame.FindOrAdd(KillerWeapon);
  Count++;

  GetWorld()->GetTimerManager().ClearTimer(m_tSimultaneousKillReset);
  GetWorld()->GetTimerManager().SetTimer(
    m_tSimultaneousKillReset,
    this,
    &UAchievementSubsystem::ProcessSimultaneousKills,
    0.3f, false);
}

void UAchievementSubsystem::ProcessSimultaneousKills()
{
  for (auto const& [WeaponType, KillCount] : m_SimultaneousSlimeKillsThisFrame)
  {
    if (KillCount >= 6)
    {

      UnlockAchievement("DivideandWorsen");
      break;
    }
  }
  m_SimultaneousSlimeKillsThisFrame.Empty();
}
#pragma endregion
void UAchievementSubsystem::SetEndlessAchievements(int _iEndlessDay)
{
  if (_iEndlessDay != 5)
  {
    return;
  }

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (!pGameInstance) return;

  const TArray<TObjectPtr<const UWeaponDataAsset>>& CurrentWeapons = pGameInstance->m_lSelectedWeapons;
  if (!CurrentWeapons.IsValidIndex(0) || !CurrentWeapons.IsValidIndex(1)) return;

  const UWeaponDataAsset* LiquidWeaponData = CurrentWeapons[0];
  const UWeaponDataAsset* DustWeaponData = CurrentWeapons[1];

  if (!LiquidWeaponData || !DustWeaponData) return;

  if (LiquidWeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance &&
    DustWeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance)
  {
    UnlockAchievement("TheMaidWithNoName");
  }
  else if (LiquidWeaponData->m_eWeaponAttackType == EWeaponAttackType::Melee &&
    DustWeaponData->m_eWeaponAttackType == EWeaponAttackType::Melee)
  {
    UnlockAchievement("MaximumCleaningEffort");
  }
}
