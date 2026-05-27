/************************************************************************
 * @description: Represents the Enemy room token manager in the Game.
 * @author: Jorge Duart
 * @date: 04/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/BaseActorComponent.h"
#include "EnemiesRoomManager.generated.h"

class ABaseEnemy;

UCLASS(ClassGroup = Manager, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UEnemiesRoomManager : public UBaseActorComponent
{
	GENERATED_BODY()

public:

	UEnemiesRoomManager();

	void InitTokens(UWorld* _mainWorld);
	
	int32 GetAvailableOozerBunnyTokens() const;

	int32 GetAvailableGrimelingTokens() const;

	bool RequestOozerBunnyToken(ABaseEnemy* _pEnemy);

	bool ReturnOozerBunnyToken(ABaseEnemy* _pEnemy, UWorld* _mainWorld); 

	bool RequestGrimelingToken(ABaseEnemy* _pEnemy);

	bool ReturnGrimelingToken(ABaseEnemy* _pEnemy, UWorld* _mainWorld);

	bool SetTokens(int _index, int _newNumTokens);

	void ResetAllTokens(UWorld* _mainWorld);

	bool SetAllTokensToMinimum(UWorld* _mainWorld);

protected:
	TArray<FTimerHandle> m_lTHRestoreToken;

private:

	//Array with tokens availables. Each token have a different regenerating time. Last position for Grimeling token.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<int32> m_lTokensAvailables;

	//Array with regenerating times for each token in m_lTokensAvailables.
	TArray<float> m_lTokensMaxTimes;

	UPROPERTY()
	TMap<TWeakObjectPtr<ABaseEnemy>, int> m_lCurrentEnemiesHoldingTokens;

	UPROPERTY()
	TArray<TWeakObjectPtr<ABaseEnemy>> m_lLastEnemiesHoldingTokens;
};
