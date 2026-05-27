/************************************************************************
 * @description: Represents the Dust Poop room token manager in the Game.
 * @author: Jorge Duart
 * @date: 04/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/BaseActorComponent.h"
#include "PoopRoomManager.generated.h"

//Tener en cuenta nivel actual de suciedad (75% por ejemplo) en la habitación
//Pueden cagar con Dorothy en otra habitación
//Controlar distancias entre cacas

class ABaseEnemy;

UCLASS()
class DOROTHYSJOB_API UPoopRoomManager : public UBaseActorComponent
{
	GENERATED_BODY()

public:

	UPoopRoomManager();

	void InitPookens(UWorld* _mainWorld);

	int32 GetAvailableDustPoopTokens() const;

	bool RequestDustPoopToken(ABaseEnemy* _pEnemy);

	bool ReturnDustPoopToken(ABaseEnemy* _pEnemy, UWorld* _mainWorld);

	bool SetTokens(int _index, int _newNumTokens);

	void ResetAllTokens(UWorld* _mainWorld);

	bool SetAllTokensToMinimum(UWorld* _mainWorld);

	void RoomDirtLevel(bool _bActive, UWorld* _mainWorld);

	float GetMinDPDistance() const;

	float GetCPPercentage() const;

	float GetCNPPercentage() const;

	bool ReservePoopPosition(const AActor* _pActor);

	void ClearDustPoopReservation(const AActor* _pActor);

protected:

	TArray<FTimerHandle> m_lTHRestoreToken;

private:

	float m_fMinDistancePoop;

	float m_fCanPoopPorcentage;

	float m_fCanNotPoopPercentage;

	//Array with tokens availables. Each token have a different regenerating time.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<int32> m_lTokensAvailables;

	//Array with regenerating times for each token in m_lTokensAvailables.
	TArray<float> m_lTokensMaxTimes;

	TArray<TWeakObjectPtr<ABaseEnemy>> m_lLastDustBunniesHoldingTokens;

	TMap<TWeakObjectPtr<ABaseEnemy>, int> m_lCurrentDustBunniesHoldingTokens;

	TMap<TWeakObjectPtr<const AActor>, FVector> m_lReservedPositions;
	
};
