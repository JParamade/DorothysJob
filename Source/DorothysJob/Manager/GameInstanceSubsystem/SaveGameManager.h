/************************************************************************
 * @description: Represents the SaveGame Manager.
 * GameInstanceSubsytem to manage the SaveGame system
 * @author: Jorge Duart
 * @date: 05/03/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameManager.generated.h"


class UDorothysJobSaveGame;
class USettingsSaveGame;
class UAchievementsSaveGame;

UENUM()
enum class EScore : uint8 {
  SPLUS  UMETA(DisplayName = "SPLUS"),
  S  UMETA(DisplayName = "S"),
  A  UMETA(DisplayName = "A"),
  B  UMETA(DisplayName = "B"),
  D  UMETA(DisplayName = "D"),
  F  UMETA(DisplayName = "F"),
};

USTRUCT(BlueprintType)
struct FFinalSavedScore
{
  GENERATED_BODY()

public:

  UPROPERTY()
  EScore m_eScore = EScore::F;

  /**
   * @brief If Dorothy has been hit in the level
   */
  UPROPERTY()
  bool m_bHasBeenHit = true;

  /**
   * @brief If the remaining time is more than the initial time
   */
  UPROPERTY()
  bool m_bIsTimerExtra = false;
};

UCLASS()
class DOROTHYSJOB_API USaveGameManager : public UGameInstanceSubsystem
{
  GENERATED_BODY()

public:

  virtual void Initialize(FSubsystemCollectionBase& Collection) override;

  /**
   * @brief Save the current level.
   * @return True if exists a file with the slot name, False otherwise.
   */
  UFUNCTION(BlueprintCallable)
  bool SaveCurrentLevel(int32 _iCurrentLevel, int32 _iMaxLevel = -1);

  /**
   * @brief Save the current level.
   * @return True if exists a file with the slot name, False otherwise.
   */
  UFUNCTION(BlueprintCallable)
  bool LoadCurrentLevel();

  /**
  * @brief Save the current Selected Weapons.
  * @return True if exists a file with the slot name, False otherwise.
  */
  UFUNCTION(BlueprintCallable)
  bool SaveWeaponsBuild();

  /**
  * @brief Load the saved Selected Weapons.
  * @return True if exists a file with the slot name, False otherwise.
  */
  UFUNCTION(BlueprintCallable)
  bool LoadWeaponsBuild();

  /**
  * @brief Save the current Selected Consumables.
  * @return True if exists a file with the slot name, False otherwise.
  */
  UFUNCTION(BlueprintCallable)
  bool SaveConsumablesBuild();

  /**
  * @brief Load the saved Selected Consumables.
  * @return True if exists a file with the slot name, False otherwise.
  */
  UFUNCTION(BlueprintCallable)
  bool LoadConsumablesBuild();

  /**
  * @brief Save the Event triggers state.
  * @return True if exists a file with the slot name, False otherwise.
  */
  UFUNCTION(BlueprintCallable)
  bool SaveEventsTriggers();

  /**
  * @brief Load the Event triggers state.
  * @return True if exists a file with the slot name, False otherwise.
  */
  UFUNCTION(BlueprintCallable)
  bool LoadEventsTriggers();

  /**
   * @brief Save the final score getting the best results (grade and stars separately)
   * @param _finalScore 
   * @param _iDay 
   * @return True if can be saved
   */
  UFUNCTION(BlueprintCallable)
  bool SaveBestFinalScore(EScore _eScore, int32 _iDay ,bool _bHit = false, bool m_bExtraTime = true);

  /**
   * @brief Gets the best final score struct on one of the days
   * @param _iDay 
   * @return FFinalScore struct
   */
  UFUNCTION(BlueprintCallable)
  FFinalSavedScore GetBestFinalScore(int32 _iDay);

  /**
   * @brief Gets the struct list of current best scores 
   * @return List of FFinalScore struct
   */
  UFUNCTION(BlueprintCallable)
  TArray<FFinalSavedScore> GetBestFinalScoreList();

  /**
   * @brief 
   * @param _lManagers 
   * @return 
   */
  UFUNCTION(BlueprintCallable)
  bool SaveSettings(TArray<UObject*> _lManagers);

  /**
   * @brief 
   */
  UFUNCTION(BlueprintCallable)
  bool LoadSettings(TArray<UObject*> _lManagers);

  UFUNCTION(BlueprintCallable)
  bool SaveAchievements();

  UFUNCTION(BlueprintCallable)
  bool LoadAchievements();

  /**
  * @brief Load the current level and the weapons and consumables build.
  * @return True if exists a file with the slot name, False otherwise.
  */
  UFUNCTION(BlueprintCallable)
  bool LoadAllGame();

  /**
   * @brief Delete the SaveGame file
   * @return True if the SaveGame file has been deleted, False if not or if the file doesn't exist.
   */
  UFUNCTION(BlueprintCallable)
  bool DeleteSave() const;

  /**
   * @brief Delete the SaveGame file
   * @return True if the SaveGame file has been deleted, False if not or if the file doesn't exist.
   */
  UFUNCTION(BlueprintCallable)
  bool CheckSaveGameExists() const;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Save Slot Name"))
  FString m_sSaveSlotName;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Settings Slot Name"))
  FString m_sSettingsSlotName;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Achievements Slot Name"))
  FString m_sAchievementsName;

private:
  UPROPERTY()
  TObjectPtr<UDorothysJobSaveGame> m_pCurrentSaveGame;

  UPROPERTY()
  TObjectPtr<USettingsSaveGame> m_pCurrentSaveSettings;

  UPROPERTY()
  TObjectPtr<UAchievementsSaveGame> m_pCurrentSaveAchievements;

};
