/************************************************************************
 * @description: Represents the Level Manager in the Game.
 * @author: Josephine Esposito
 * @date: 20/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelManager.generated.h"

class StructuredLog; // for logs
class QuadTree;
class ABaseTile;
class UBaseRoom;
class UProceduralGeneration;
class ABaseEnemy;
class UEventListDataAsset;
class UBaseGameInstance;
struct FEventDataArray;
enum class ERoomShape : uint8;

#pragma region | Delegates
/**
* @brief Declaration of the Category for Logs
*/
DECLARE_LOG_CATEGORY_EXTERN(LevelManager, Log, All);

/**
* @brief A bool to retreive the state of the game between true, when the game is over, and false, when the game is won
*/
DECLARE_MULTICAST_DELEGATE_OneParam(FCurrentGameState, EBaseGameState /* enum */);
DECLARE_MULTICAST_DELEGATE(FCurrentConsumableInput);
DECLARE_MULTICAST_DELEGATE(FOnGamePaused);
DECLARE_DELEGATE_OneParam(FConsumableNavigation, int32 /* direction */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllRoomsCreated);
DECLARE_MULTICAST_DELEGATE(FOnRoomsInTheList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomChanged, UBaseRoom*, NewRoom);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemiesBuffChange);
DECLARE_DELEGATE_OneParam(FOnInputKeyPressed, bool);
#pragma endregion

/**
* @brief Represents the different states of the Game
*/
UENUM(BlueprintType)
enum class EBaseGameState : uint8
{
  PLAYING		        UMETA(DisplayNmae = "Playing"),

  GAMEOVER	        UMETA(DisplayName = "GameOver"),

  WIN				        UMETA(DisplayName = "Win"),

  PAUSED            UMETA(DisplayName = "Paused"),

  SELECTWEAPON      UMETA(DisplayName = "Weapon Select"),

  SELECTCONSUMABLE  UMETA(DisplayName = "Consumable Select"),

  PRELEVEL          UMETA(DisplayName = "Pre-Level"),

  DIALOGUE          UMETA(DisplayName = "Dialogue"),

  WIDGET            UMETA(DisplayName = "Widget"),

  REPLAY            UMETA(DisplayName = "Replay")
};

USTRUCT(BlueprintType)
struct FFinalScore
{
  GENERATED_BODY()

public:

  /**
   * @brief The added points from the timer
   */
  UPROPERTY()
  int32 m_iTimePoints = 0;

  /**
   * @brief The added points from the consumables
   */
  UPROPERTY()
  int32 m_iConsumablesPoints = 0;

  /**
   * @brief The added points from the cleaning station
   */
  UPROPERTY()
  int32 m_iCleaningStationPoints = 0;

  /**
   * @brief The total points
   */
  UPROPERTY()
  int32 m_iTotalScore = 0;

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

/**
 * The Level Manager Class
 */
UCLASS()
class DOROTHYSJOB_API ULevelManager : public UWorldSubsystem
{
  GENERATED_BODY()
public:

  /**
 * @brief The current game state
 */
  UPROPERTY(EditAnywhere, Category = "Level Manager", meta = (DisplayName = "Game state"))
  EBaseGameState CurrentGameState;

#pragma region | Delegates Variables
  /**
  * @brief Delegate representing the current game state
  */
  FCurrentGameState OnGameStateChanged;

  FOnGamePaused OnGamePaused;
  FOnGamePaused OnGameUnpaused;

  FCurrentConsumableInput OnReceivedConsumableInput;

  FConsumableNavigation OnReceivedConsumableNavigation;

  UPROPERTY(BlueprintAssignable)
  FOnAllRoomsCreated OnAllRoomsCreated;

  UPROPERTY(BlueprintAssignable, Category = "Bola13|Rooms")
  FOnRoomChanged OnEnteredRoom;
  
  UPROPERTY(BlueprintAssignable, Category = "Bola13|Rooms")
  FOnRoomChanged OnExitedRoom;

  UPROPERTY(BlueprintAssignable, Category = "Bola13|Rooms")
  FOnEnemiesBuffChange OnEnemiesBuffed;

  UPROPERTY(BlueprintAssignable, Category = "Bola13|Rooms")
  FOnEnemiesBuffChange OnEnemiesDebuffed;

  FOnInputKeyPressed OnCancelButtonPressed;

  FOnInputKeyPressed OnChangeTabButtonPressed;
#pragma endregion

#pragma region | Base Virtual Methods
  /**
  * @brief To initialize the Manager, this will set automatically the game status to playing
  * @param Collection, a subsystem collection base
  */
  virtual void Initialize(FSubsystemCollectionBase& Collection) override;

  /**
   * @brief Called once all UWorldSubsystems have been initialized
   */
  virtual void PostInitialize() override;

  /**
  * @brief Called when world is ready to start gameplay before the game mode transitions to the correct state and call BeginPlay on all actors
  * @param InWorld
  */
  virtual void OnWorldBeginPlay(UWorld& InWorld) override;

  /**
  * @brief Called to de-initialize
  */
  virtual void Deinitialize() override;
#pragma endregion

  void SetupNotTriggeredEvents();

  void SetupNotTriggeredEvent(FName EventName);

  const TArray<TObjectPtr<UBaseRoom>>& GetRooms() const;

  void AddRoom(UBaseRoom* RoomToAdd);

  /**
   * @brief What to init when level has finish initialize
   */
  void OnLevel0DialogInit();

  /**
   * @brief What to init when level has finish initialize
   */
  void OnLevelInit();

  /**
   * @brief What to do when level has finished
   */
  void OnLevelFinish();

  /**
   * @brief What to do when event trigger
   */
  void StartEvent();

  TArray<FName> m_aArrows;

private:

#pragma region | Private Variables
  /**
   * @brief Level Rooms
   */
  UPROPERTY()
  TArray<TObjectPtr<UBaseRoom>> m_lRooms;

  /**
   * @brief Player current room
   */
  TObjectPtr<UBaseRoom> m_pCurrentRoom;

  /**
   * @brief If all the rooms are created or not
   */
  bool m_bAreAllRoomsCreated = false;
#pragma endregion 

#pragma region | Functions
  /**
   * @brief Set boolean all rooms created to true
   */
  UFUNCTION()
  void AllRoomsCreated();

  /**
   * @brief To handle the timer and trigger the OnGameOver method
   * @param _seconds
   */
  UFUNCTION()
  void HandleTimerExpired(int _seconds);

  /**
   * @brief Pauses the current game. This will NOT change the game status NOR the Input mode
   * @param _status, The status of the mode, true to pause, false to unpause
   */
  UFUNCTION()
  void SetPauseMode(bool _status);
#pragma endregion

public:

#pragma region | Public Variables
  UPROPERTY(BlueprintReadWrite)
  TObjectPtr<UProceduralGeneration> ProceduralGeneration;

  UPROPERTY(BlueprintReadWrite, Category = "Current Level")
  int32 m_iCurrentLevel;
#pragma endregion

#pragma region | Default Get and Set GameStatus
  /**
   * @brief Gets the current game state
   * @return An EGameState enum variable representing the current state of the game
   */
  UFUNCTION(BlueprintCallable)
  EBaseGameState GetCurrentGameState() const;

  /**
   * @brief Sets the current game state. This methods will NOT run in Shipping Builds!
   * @param _status, The desired new EGameState between PLAYING, GAMEOVER, WIN and PAUSED
   */
  UFUNCTION(BlueprintCallable)
  void SetCurrentGameState(EBaseGameState _status);
#pragma endregion

  UFUNCTION()
  void HandleEnemiesBuffed(UBaseRoom* Room);
  
  UFUNCTION()
  void HandleEnemiesDebuffed(UBaseRoom* Room);

#pragma region | Getters
  UFUNCTION(BlueprintCallable)
  UBaseRoom* GetCurrentRoom() const; // is it better here or in the UBaseRoomClass?

  FFinalScore* GetFinalScore() const;

  UPROPERTY()
  UBaseGameInstance* pGameInstance;

  UPROPERTY()
  UEventListDataAsset* pListDataAssets;
#pragma endregion

#pragma region | Setters
  /**
  * @brief This method will set the game status to over depending on the timer
  */
  UFUNCTION(BlueprintCallable)
  void OnGameOver();

  /**
  * @brief This method will set the game status to win depending on the level objectives
  */
  UFUNCTION(BlueprintCallable)
  void OnGameWin();

  /**
   * @brief This method will set the game status to pause. It will freeze the game but it will NOT set the input mode
   */
  UFUNCTION(BlueprintCallable)
  void OnGamePause(bool _toggle);

  /**
   * @brief This method will set the game status to pause. It will freeze the game but it will NOT set the input mode
   */
  UFUNCTION(BlueprintCallable)
  void OnGameDialogue(bool _toggle, FName _sSequenceName = "");

  /**
 * @brief This method will set the game status to pause. It will freeze the game but it will NOT set the input mode
 */
  UFUNCTION(BlueprintCallable)
  void OnGameEvent(bool _toggle);

  void SetCurrentRoom(TObjectPtr<UBaseRoom> NewCurrentRoom);

  /**
   * @brief This will update the player stinky times
   */
  void AddStinkyTimes();

  /**
   * @brief This will update the consumable uses
   */
  void AddConsumableUse();

  void HasDorothyBeenHit();

#pragma endregion

#pragma region | Audio Interpolation Parameters
  /**
   * @brief Current FMOD Audio Parameter value.
   */
  float m_fCurrentAudioValue = 0.f;
  /**
   * @brief Target FMOD Audio Parameter value.
   */
  float m_fTargetAudioValue = 0.f;
  /**
   * @brief Interpolation speed between current and target FMOD Audio Parameter values.
   */
  float m_fInterpAudioSpeed = 2.f;

  /**
   * @brief Flag that handles when to start interpolating.
   */
  bool m_bStartAudioInterp = false;

  FTimerHandle m_oAudioHandle;
#pragma endregion 

  // ========= THIS IS JUST FOR ALPHA DELETE THIS AFTER ============
  UFUNCTION(BlueprintCallable)
  void LoadNotProceduralRooms(int32 key);
  void AddNewRoom(int32 ID, FVector Position, float Rotation, ERoomShape Shape);
  void OnRoomCleaned(UBaseRoom* _CleanedRoom);
  FOnRoomsInTheList RoomsInTheList;
  int32 CleanedRooms;
  int32 GetNotQ1Rooms();
  // ========= THIS IS JUST FOR ALPHA DELETE THIS AFTER ============

  float m_fExtraTime;

  int32 m_iStinkyTimes;

  int32 m_iConsumableUses;

  bool m_bHasDorothyBeenHit;

  bool m_bHasTimerExceeded;
};
