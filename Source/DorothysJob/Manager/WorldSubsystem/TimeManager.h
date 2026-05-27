/************************************************************************
 * @description: Represents the Time manager in the Game.						 
 * @author: Jorge Duart
 * @date: 13/03/2025
 * @edited_by: Jaime Paramo - 18/03/2025
 * @edited_by: Josephine Esposito - 22/03/2025
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimeManager.generated.h"

 /**
 * @brief Declaration of the Category for Logs
 */
DECLARE_LOG_CATEGORY_EXTERN(TimerManager, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeChanged, int /* Seconds */);

namespace FMOD {
	namespace Studio {
		class EventInstance;
	}
}

UCLASS()
class DOROTHYSJOB_API UTimeManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	FOnTimeChanged OnTimeChanged;

	/**
	 * @brief Set the timer if it doesn't exist
	 * @param _totalMin Total minutes in the countdown
	 * @param _totalSeg Initial seconds
	 * @return True if a timer doesn't already exist, False otherwhise
	 */
	UFUNCTION(BlueprintCallable)
	bool StartTimer(int _totalMin, int _totalSeg);

	/**
	 * @brief Pauses the timer if it is active
	 * @return True if the timer is active and can be paused, False otherwhise
	 */
	UFUNCTION(BlueprintCallable)
	bool PauseTimer();

	/**
	 * @brief Plays the timer if it is paused
	 * @return True if the timer is paused and can be activated, False otherwhise
	 */
	UFUNCTION(BlueprintCallable)
	bool PlayTimer();

	/**
	 * @brief Clears the timer if it exists
	 * @return True if the timer exists and can be cleared, False otherwhise
	 */
	bool ClearTimer();

	/**
	 * @brief If time runs out, the timer is cleared.
	 */
	void CountDown();

	/**
	 * @brief Add time to the currentSeconds variable
	 * @param _addMin Minutes to add
	 * @param _addSeg Seconds to add
	 */
	UFUNCTION(BlueprintCallable)
	void AddTime(int _addMin, int _addSeg);

	/**
	 * @brief @TOFILL. 
	 * @return @TOFILL.
	 */
	int GetCurrentSeconds() const;

	/**
	 * @brief @TOFILL.
	 * @return @TOFILL.
	 */
	int GetRunningOutTime() const;

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
	bool m_bStartAudioInterp = true;
#pragma endregion 

private:
	void GetCachedWorld();

	FMOD::Studio::EventInstance* pTimeEvent = nullptr;

	UPROPERTY()
	FTimerHandle m_countDownHandle;

	UPROPERTY()
	int m_iCurrentSeconds;

	UPROPERTY()
	int m_iTotalSeconds;

	int32 m_iRunningOutOfTime = 25;

	UPROPERTY()
	UWorld* CachedWorld;
};
