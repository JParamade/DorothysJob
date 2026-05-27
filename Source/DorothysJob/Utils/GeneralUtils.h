/************************************************************************
 * @description: General Utils with static functions
 * @author: David Gonzalez
 * @date: 23/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"

class ABaseEnemy;
class UBaseWeaponStatsDataAsset;

class DOROTHYSJOB_API GeneralUtils
{
public:
  /**
   * @brief It returns a location to reach for the AI character. If It can't find a location to reach in the max tries, it will return false.
   * @param _pPawn -> AI character to move.
   * @param _fMinRadius -> Minimum distance form the AI character to find the location to reach.
   * @param _fMaxRadius -> Maximum distance form the AI character to find the location to reach.
   * @param _fAngle -> Angle related to the AI character forward vector to find the location to reach.
   * @param _iMaxTries -> Max tries to find the location to reach.
   * @param _oBaseRotation -> Base rotation to find the point.
   * @param vLocationToReach_ -> AI location to reach. Out parameter.
   * @param _fForbiddenAngle -> Angle to not take in account in the range.
   * @return True if it had been able to found a location to reach.
   */
  static bool GetRandomPointInCone(const APawn* _pPawn, float _fMinRadius, float _fMaxRadius, float _fAngle, int32 _iMaxTries, const FRotator& _oBaseRotation, FVector& vLocationToReach_, float _fForbiddenAngle = -1.f);

  /**
   * @brief It returns a step path point to reach for the AI character.
   * @param _pPawn -> AI character to move.
   * @param _fMinRadius -> Minimum distance form the AI character to find the location to reach.
   * @param _fMaxRadius -> Maximum distance form the AI character to find the location to reach.
   * @param _vLocationToReach -> AI location to reach.
   * @param vStepLocation_ -> AI step location to reach.
   * @return True if it had been able to found a step location to reach.
   */
  static bool GetStepPathPoint(APawn* _pPawn, float _fMinRadius, float _fMaxRadius, const FVector& _vLocationToReach, FVector& vStepLocation_);

  /**
   * @brief Function to check if the enemy can reach the selected point.
   * @param _pPawn -> AI character to move.
   * @param _vReachLocation -> AI location to reach.
   * @return True if it is able to reach the location.
   */
  static bool IsReachable(const APawn* _pPawn, const FVector& _vReachLocation);

  /**
 * @brief Function to check if the enemy can reach the selected point.
 * @param _pPawn -> AI character to move.
 * @param _vReachLocation -> AI location to reach.
 * @param vReachablePoint_ -> If reachable, it will be _vReachLocation, if not it will be the last reachable point in the path.
 * @return True if it is able to reach the location.
 */
  static bool IsReachableLineTrace(const APawn* _pPawn, const FVector& _vReachLocation, FVector* vReachablePoint_);

  /**
   * @brief It draws a cone from the AI character.
   * @param _pPawn -> AI character to move.
   * @param _oBaseRotation -> Base rotation for the cone.
   * @param _fMinRadius -> Minimum distance form the AI character to the first cone.
   * @param _fMaxRadius -> Maximum distance form the AI character to the second cone.
   * @param _fAngle -> Angle related to the AI character forward vector to build the cone.
   * @param _oMinColor -> Min cone color.
   * @param _oMaxColor -> Max cone color.
   * @param _fDuration -> Debug draw duration.
   * @param _fForbiddenAngle -> Angle to not draw.
   */
  static void DrawDebugCone(const APawn* _pPawn, const FRotator& _oBaseRotation, float _fMinRadius, float _fMaxRadius, float _fAngle, FColor _oMinColor, FColor _oMaxColor, float _fDuration = -1.f, float _fForbiddenAngle = -1.f);

  /**
   * @brief It draws two circles. The center is the AI character
   * @param _pPawn -> AI character to move.
   * @param _fMinRadius -> Small radius.
   * @param _fMaxRadius -> Big radius.
   * @param _oMinColor -> Small circle color.
   * @param _oMaxColor -> Big circle color.
   * @param _fDuration -> Debug draw duration.
   */
  static void DrawDebugDoubleCircle(const APawn* _pPawn, float _fMinRadius, float _fMaxRadius, FColor _oMinColor, FColor _oMaxColor, float _fDuration = -1.f);

  /**
   * @brief It checks if the weapon is the washee&flashee.
   */
  static bool IsWasheeFlashee(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset);

  /**
   * @brief It checks if the weapon is the mopressor.
   */
  static bool IsMopressor(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset);

  /**
   * @brief It checks if the weapon is the dirtminator.
   */
  static bool IsDirtminator(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset);

  /**
   * @brief It checks if the weapon is the dusty.
   */
  static bool IsDusty(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset);
};
