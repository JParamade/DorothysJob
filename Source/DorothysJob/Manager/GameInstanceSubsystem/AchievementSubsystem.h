#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystemSteam.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "AchievementSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAchievementUnlockedEvent, FString, AchievementId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAchievementProgressChangedEvent, FString, AchievementId, int32, NewValue);

/**
  * @brief Represents the different states of the Game
  */
UENUM(BlueprintType)
enum class EWeaponCombination : uint8
{
  WF_DUSTY		    UMETA(DisplayNmae = "WF & Dusty"),
  WF_VAC	        UMETA(DisplayName = "WF & Dirtminator"),
  MOP_DUSTY				UMETA(DisplayName = "Moppressor & Dusty"),
  MOP_VAC         UMETA(DisplayName = "Moppressor & Dirtminator")
};

USTRUCT(BlueprintType)
struct FAchievementData {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FString AchievementId;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  FString StatName;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 CurrentValue = 0;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  int32 MaxValue = 1;

  UPROPERTY(BlueprintReadWrite, EditAnywhere)
  bool bUnlocked = false;
};

class USaveGameManager;

UCLASS()
class DOROTHYSJOB_API UAchievementSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()

public:
  virtual void Initialize(FSubsystemCollectionBase& _rCollection) override;

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Achievements")
  void RegisterAchievement(const FString& _rAchievementId, int32 _iMaxValue = 1, const FString& _rStatName = TEXT(""));

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Achievements")
  void UnlockAchievement(const FString& _rAchievementId);

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Achievements")
  void AddProgress(const FString& _rAchievementId, int32 _iAmount);

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Achievements")
  int32 GetProgress(const FString& _rAchievementId) const;

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Achievements")
  bool IsUnlocked(const FString& _rAchievementId) const;

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Achievements")
  void SaveAchievements();
 
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Achievements")
  void LoadAchievements();

#if WITH_EDITOR
  void ResetStats(const FUniqueNetId& _rUserId);
  void ResetStats();
#endif

  UPROPERTY(BlueprintAssignable, Category = "Bola 13|Achievements")
  FAchievementUnlockedEvent OnAchievementUnlocked;

  UPROPERTY(BlueprintAssignable, Category = "Bola 13|Achievements")
  FAchievementProgressChangedEvent OnAchievementProgressChanged;


protected:
  TMap<FName, FAchievementData> m_mAchievements;

  bool m_bSteamAchievementsReady = false; 
  TArray<FString> m_lPendingUnlocks;

  IOnlineAchievementsPtr m_pCachedAchievementsInterface;
  TSharedPtr<const FUniqueNetId> m_pCachedPlayerId;

  void RegisterFromINI();
  FAchievementData* FindAchievement(const FString& _rAchievementId);

  void WriteAchievementToSteam(const FAchievementData& _rData);
  void OnQueryAchievementsComplete(const FUniqueNetId& _rUserId, bool _bWasSuccessful);

  bool AreAllAchievementsUnlocked();

public:
#pragma region | Variables To Save

  UPROPERTY(SaveGame)
  TMap<FName, bool> m_mGottaWashEmAll;

  /**
   * @brief Weapon combination that has apear by now.
   */
  UPROPERTY(SaveGame)
  TArray<EWeaponCombination> m_lAllWeaponCombination;

  /**
   * @brief If the player has not gotten stinky or done a reload level.
   */
  UPROPERTY(SaveGame)
  bool m_bNoStinkyReload = true;

  /**
   * @brief Save the time of the game to see if 2 hours has pass.
   */
  UPROPERTY(SaveGame)
  float m_fActualGameTime = 0.0f;

  UPROPERTY(SaveGame)
  bool m_bSqueakyMean = false;
#pragma endregion

#pragma region | Setters
  void ResetTemporalLevelVariables();
#pragma endregion

#pragma region | TheUltimateArsenal Achievement 
  /**
   * @brief Set the weapon combinations that has appear after each level and launch achievement.
   */
  void SetWeaponCombinationAchievement();
#pragma endregion

#pragma region | DirtyWork Achievement
  /**
   * @brief Set the pass level stinky achievement.
   */
  void SetPassLevelStinkyAchievement();
#pragma endregion

#pragma region | GGSqueakyClean Achievement
  /**
   * @brief Set the pass game without stinky and reload achievement.
   */
  void SetGGSqueakyCleanAchievement();
#pragma endregion

#pragma region | Kills Achievement
  /**
   * @brief Set the pass game without stinky and reload achievement.
   */
  void SetKillsAchievement(EWeaponsTypes _eWeaponLastAttackReceive);
#pragma endregion

#pragma region | Kills With B13 Achievement
  /**
   * @brief Set the pass game without stinky and reload achievement.
   */
  void SetKillsWithB13();
#pragma endregion

#pragma region | Mopped and Loaded Achievement
  float m_fLevelStartTime = 0.f;
  int m_iCleanRoomsObtain = 0;

  /**
   * @brief Set the achivement if player do 3 cleanrooms in 1 minute at the start of the level.
   */
  void SetMoppedAndLoadedAchievement();
#pragma endregion

#pragma region | Paid Overtime Achievement
  int m_fCleanRoomsIn5s = 0;

  /**
   * @brief Set the achivement if player do 3 cleanrooms in 1 minute at the start of the level.
   */
  void SetPaidOvertimeAchievement();
#pragma endregion

#pragma region | A Maids Best Friends Achievement
  /**
   * @brief Dogs deploy.
   */
  TArray<float> m_fDogsDeploy;

  /**
   * @brief Set the achivement if player deploy 5 dogs in 5 seconds.
   */
  void SetDogsDeployAchievement();
#pragma endregion

#pragma region | Nothing under my sleeves Achievement
  /**
   * @brief Start game timer.
   */
  float m_fStartGameTime = 0.0f;

  /**
   * @brief Set the achivement if player has win a star in each level.
   */
  void Set2hTimeAchievement();

  /**
   * @brief Set the achivement if player has win a star in each level.
   */
  void Save2hTimeAchievement();
#pragma endregion

#pragma region | Maids are like onions Achievement
  /**
   * @brief Use apron bolean, if when level finish this is false achievement complete.
   */
  bool m_bApronUse = false;

  /**
   * @brief See if player has change apron or not.
   */
  bool m_bEquipApron = false;

  /**
   * @brief See if player has change apron but hasn't used it.
   */
  void SetNotUseApronAchievement();
#pragma endregion

#pragma region | Long live the Clean Achievement
  /**
   * @brief The start time of the WF special attack.
   */
  float m_fWFSpecialStart = 0.0f;

  /**
   * @brief See if the WF special attack was active for more tah 8 seconds.
   */
  void SetWFSpecialAttackAchievement();
#pragma endregion

#pragma region | CATEGORY 5 DUST-NADO Achievement
  /**
   * @brief The start time of the WF special attack.
   */
  int m_iDustPoopsClean = 0;

  bool m_bDustySpecialActive = false;

  void SetDustySAAchievement();
#pragma endregion

#pragma region | Global Mopnopoly Achievement
  /**
   * @brief Mop special attacks that was set active withina minute.
   */
  TArray<float> m_fMopSpecialAttackActive;

  void SetMopSAAchievement();
#pragma endregion

#pragma region | You are Dirtminated Achievement
  bool m_bIsMaxChargeShot = false;

  int m_iMaxChargeDeaths = 0;

  void SetDirtMaxChargeAchievement();
#pragma endregion

#pragma region | Monster clean Achievement
  bool m_bClean4Active = false;

  int m_iClean4Deaths = 0;

  void SetClean4DeathsAchievement();
#pragma endregion

#pragma region | Go beyond flush ultra Achievement
  bool m_bUseB13 = false;

  int m_iEnemyKillsWith1B13 = 0;

  void SetKillsWith1B13Achievement();
#pragma endregion

#pragma region | Squeaky and Mean Achievement
  void CheckSqueakyMeanAchievement();
#pragma endregion

#pragma region | Whats Yours Is Mine Achievement
  bool m_bBombActive = false;

  int m_fBombExplode = 0;

  void SetClean4ChainAchievement();
#pragma endregion

#pragma region | Wash Wash, Womp Womp Achievement

  int m_iDeathCounter = 0;

  void SetDeathCounterAchievement();
#pragma endregion

#pragma region | Endless achievements
  void SetEndlessAchievements(int _iEndlessDay);
#pragma region | Divide and Worsen

  TMap<EWeaponsTypes, int32> m_SimultaneousSlimeKillsThisFrame;
  FTimerHandle m_tSimultaneousKillReset;

  void ProcessSimultaneousKills();

  void ReportSlimeKill(EWeaponsTypes KillerWeapon);
#pragma endregion
private:
  UPROPERTY()
  TObjectPtr<USaveGameManager> m_pSaveGameManager;


};
