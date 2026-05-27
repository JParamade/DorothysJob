/************************************************************************
 * @description: Represents the Base Game Instance.
 * @author: David Gonzalez
 * @date: 30/05/2025
 * @edited_by: Josephine - 07/09/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

class UWeaponDataAsset;
class UConsumableDataAsset;
class UDirtTypesDataAsset;
class UEnemyDataAsset;
class UTokenManagerDataAsset;
class UPookenManagerDataAsset;
class UFMODBus;
class UFMODEvent;
class UPostProcessRooms;
class UCleanRoomData;
class UProceduralDataAsset;
class UDefaultSettingsDataAsset;
class USpriteDataAsset;
class UEventTutorialDataAsset;
class UEventListDataAsset;
class UScoreDataAsset;
class UEndlessDataAsset;
class UEndlessScoreDataAsset;
class UTipManager;
class UDialogsManager;
class UEventManager;

UENUM(BlueprintType)
enum class EHUDSection : uint8
{
  CONSUMABLES UMETA(DisplayName = "Consumables"),
  MINIMAP     UMETA(DisplayName = "Minimap"),
  CLOCK     UMETA(DisplayName = "Clock"),
  LIFE       UMETA(DisplayName = "Life"),
  SPECIAL     UMETA(DisplayName = "Special")
};

UCLASS()
class DOROTHYSJOB_API UBaseGameInstance : public UGameInstance
{
  GENERATED_BODY()

public:

  virtual void Init() override;

  virtual void Shutdown() override;

  void LoadLevelWithLoadingScreen();

#pragma region | Weapons
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Weapons", meta = (DisplayName = "Weapons"))
  TArray<TObjectPtr<const UWeaponDataAsset>> m_lWeapons;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Weapons", meta = (DisplayName = "Selected Weapons"))
  TArray<TObjectPtr<const UWeaponDataAsset>> m_lSelectedWeapons;
#pragma endregion

#pragma region | Consumables
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Consumable", meta = (DisplayName = "Consumables"))
  TArray<TObjectPtr<const UConsumableDataAsset>> m_lConsumables;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Consumable", meta = (DisplayName = "Selected Consumables"))
  TArray<TObjectPtr<const UConsumableDataAsset>> m_lSelectedConsumables;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Consumable", meta = (DisplayName = "Consumables Info"))
  TObjectPtr<UDataTable> m_pConsumablesData;
#pragma endregion

#pragma region | Types Data
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bola 13|Types", meta = (DisplayName = "Types Table"))
  TObjectPtr<UDirtTypesDataAsset> m_pTypesTable;
#pragma endregion

#pragma region | Enemies
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Enemies", meta = (DisplayName = "Enemies"))
  TObjectPtr<const UEnemyDataAsset> m_lEnemies;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Enemies", meta = (DisplayName = "Attack Token Manager"))
  TObjectPtr<const UTokenManagerDataAsset> m_lAttackTokens;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Enemies", meta = (DisplayName = "Dust Pooken Manager"))
  TObjectPtr<const UPookenManagerDataAsset> m_lDustPookens;
#pragma endregion

#pragma region | Score
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Score", meta = (DisplayName = "Categories"))
  TObjectPtr<const UScoreDataAsset> m_pScores;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Score", meta = (DisplayName = "Points Data"))
  TObjectPtr<UDataTable> m_pScorePointsData;
#pragma endregion

#pragma region | Events
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Dialogs", meta = (DisplayName = "Dialogs Data Tables"))
  TMap<FName, TObjectPtr<UDataTable>> m_mDialogsData;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Dialogs", meta = (DisplayName = "Dialogs Assets"))
  TMap<FName, TObjectPtr<const USpriteDataAsset>> m_mSprites;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Events", meta = (DisplayName = "Persistent Widget Data Asset", AllowedClasses = "/Script/DorothysJob.EventTutorialDataAsset"))
  TObjectPtr<UEventTutorialDataAsset> m_pEWDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Events", meta = (DisplayName = "Event List Data Asset"))
  TObjectPtr<UEventListDataAsset> m_pGeneralEventsDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Events", meta = (DisplayName = "HUD Data To Show"))
  TMap<EHUDSection, bool> m_oHUDData{
    { EHUDSection::CONSUMABLES, false },
    { EHUDSection::MINIMAP, false },
    { EHUDSection::CLOCK, false },
    { EHUDSection::LIFE, true },
    { EHUDSection::SPECIAL, false }
  };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Events", meta = (DisplayName = "HUB Event Names"))
  TMap<int32, int32> m_oHubEventTriggered;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Events", meta = (DisplayName = "Whats has already been use data"))
  TMap<FName, bool> m_oEventUseData;
#pragma endregion

#pragma region | PostProcessVolume
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|PPV", meta = (DisplayName = "PPV"))
  TArray<TObjectPtr<const UPostProcessRooms>> m_lPPVAndRooms;
#pragma endregion

#pragma region | Audio
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Audio", meta = (DisplayName = "Music Bus"))
  TObjectPtr<UFMODBus> m_pMusicBus;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Audio", meta = (DisplayName = "Sound Effects Bus"))
  TObjectPtr<UFMODBus> m_pSFXBus;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Audio", meta = (DisplayName = "Killer Event"))
  TObjectPtr<UFMODEvent> m_pKillerEvent;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Audio", meta = (DisplayName = "Baby Killer Event"))
  TObjectPtr<UFMODEvent> m_pBabyKillerEvent;
#pragma endregion

#pragma region | Level
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Clean Room Data"))
  TObjectPtr<UCleanRoomData> m_pCleanRoomData;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Room Data"))
  TObjectPtr<UDataTable> m_pRoomData;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Level Data"))
  TObjectPtr<UDataTable> m_pLevelData;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Procedural Data"))
  TObjectPtr<UProceduralDataAsset> m_pProceduralData;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Endless Data"))
  TObjectPtr<UEndlessDataAsset> m_pEndlessData;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Procedural Level"))
  TSoftObjectPtr<UWorld>  m_pProceduralLevel;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Loading Screen Class"))
  TSubclassOf<UUserWidget> m_pLoadingScreenClass;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Endless Score Data"))
  TObjectPtr<UEndlessScoreDataAsset> m_pEndlessScoreData;

  UPROPERTY()
  int32 PrimaryConsumableAmount = 0;

  UPROPERTY()
  int32 SecondaryConsumableAmount = 0;

  UPROPERTY()
  int32 HealthToRemove = 0;

  int32 SecondsLastedInEndless = 0;

  int32 EnemiesKilledInEndless;
#pragma endregion

#pragma region |Tutorial
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Tutorial", meta = (DisplayName = "Tutorial Room Data"))
  TObjectPtr<UDataTable> m_pTutorialRoomData;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Tutorial", meta = (DisplayName = "Tutorial Locations"))
  TArray<FIntPoint> m_lTutorialLocations;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Tutorial", meta = (DisplayName = "Tutorial Connections"))
  TArray<FIntPoint> m_lTutorialConnections;
#pragma endregion

#pragma region | Tips
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Tips", meta = (DisplayName = "Tip Data"))
  TObjectPtr<UDataTable> m_pTipData;
#pragma endregion

#pragma region | Settings
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Settings", meta = (DisplayName = "Default Settings Data"))
  TObjectPtr<const UDefaultSettingsDataAsset> m_pDefaultSettingsData;
#pragma endregion

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Weapons", meta = (DisplayName = "Update Available Weapons and Consumables by Day"))
  bool UpdateAvailableWeaponsAndConsumables(int _iDay);

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Weapons", meta = (DisplayName = "Reset Selected Weapons and Consumables"))
  bool ResetSelectedWeaponsAndConsumables();

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Weapons", meta = (DisplayName = "Update Selected Weapons"))
  void UpdateSelectedWeapons();

  UFUNCTION(BlueprintCallable, Category = "Bola 13|Consumables", meta = (DisplayName = "Update Selected Consumables"))
  void UpdateSelectedConsumables();


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Settings", meta = (DisplayName = "Current Level"))
  int m_iCurrentLevel = 0;

  UPROPERTY(BlueprintReadWrite)
  int m_iHighestLevelAchieved = 0;
  
  UPROPERTY()
  int m_iCurrentEndlessLevel = -1;

#pragma region | Getters
  UFUNCTION(BlueprintCallable, Category = "Bola 13|Tips", meta = (DisplayName = "Tip Manager"))
  UTipManager* GetTipManager() const;

  UDialogsManager* GetDialoguesManager() const;

  UEventManager* GetEventManager() const;
#pragma endregion

  TMap<FName, bool> SyncFirstUseDataFromAsset();

  void UpdateDataAssetFromFirstUseData();

  void ResetTriggersUseData();

  UPROPERTY(BlueprintReadOnly)
  TObjectPtr<UEventManager> m_pEventManager;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Level", meta = (DisplayName = "Player Start Offset"))
  FIntPoint OffsetPlayerStart;

  UPROPERTY(BlueprintReadWrite, Category = "Bola 13|UI", meta = (DisplayName = "Menu Launched"))
  bool m_bMenuLaunched = false;

private:
  UPROPERTY()
  TObjectPtr<UTipManager> m_pTipManager;

  UPROPERTY()
  TObjectPtr<UDialogsManager> m_pDialogsManager;

  void FilterSelectedConsumbles(const UConsumableDataAsset* &_pFirstCons, const UConsumableDataAsset* _pSecondCons);
};