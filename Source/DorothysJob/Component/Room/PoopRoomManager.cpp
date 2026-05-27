
#include "PoopRoomManager.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Data/DataAssets/Enemy/PookenManagerDataAsset.h"
#include <Kismet/GameplayStatics.h>

UPoopRoomManager::UPoopRoomManager()
{
  m_lTokensAvailables.Init(1, 2);

  float lMaxTimes[] = {9.f, 15.f};
  m_lTokensMaxTimes.Append(lMaxTimes, 2);

  m_lTHRestoreToken.SetNum(2);

  m_lLastDustBunniesHoldingTokens.Init(nullptr, 2);

  m_fMinDistancePoop = 1000.f;
  m_fCanPoopPorcentage = 0.6f;
  m_fCanNotPoopPercentage = 0.7f;
}

void UPoopRoomManager::InitPookens(UWorld* _mainWorld)
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(_mainWorld));
  if (pGameInstance)
  {
    const UPookenManagerDataAsset* pTokenData = pGameInstance->m_lDustPookens;
    if (pTokenData && !pTokenData->m_lTokenParameters.IsEmpty())
    {
      m_fMinDistancePoop = pTokenData->m_fDustPoopDistance;
      m_fCanPoopPorcentage = pTokenData->m_fDirtPercentageCanPoop;
      m_fCanNotPoopPercentage = pTokenData->m_fDirtPercentageCanNotPoop;
      m_lTokensAvailables.Empty();
      m_lTokensMaxTimes.Empty();
      m_lLastDustBunniesHoldingTokens.Empty();
      m_lCurrentDustBunniesHoldingTokens.Empty();
      m_lReservedPositions.Empty();
      m_lTHRestoreToken.Empty();

      const int iNumTokens = pTokenData->m_lTokenParameters.Num();
      m_lTHRestoreToken.SetNum(iNumTokens);

      for (const FToken& token : pTokenData->m_lTokenParameters)
      {
        m_lTokensAvailables.Add(token.m_iMaxTokens);
        m_lTokensMaxTimes.Add(token.m_fRestoreTime);
      }
    }
  }
}

int32 UPoopRoomManager::GetAvailableDustPoopTokens() const
{
  int32 iTotalAvailableTokens_ = 0;
  for (int i = 0; i < m_lTokensAvailables.Num(); i++)
  {
    iTotalAvailableTokens_ += m_lTokensAvailables[i];
  }
  return iTotalAvailableTokens_;
}

bool UPoopRoomManager::RequestDustPoopToken(ABaseEnemy* _pEnemy)
{
  if (GetAvailableDustPoopTokens() == 0 || !IsValid(_pEnemy))
  {
    return false;
  }

  if (m_lLastDustBunniesHoldingTokens.Contains(_pEnemy))
  {
    m_lLastDustBunniesHoldingTokens.Remove(_pEnemy);
    return false;
  }

  for (int i = 0; i < m_lTokensAvailables.Num(); i++)
  {
    if (m_lTokensAvailables[i] > 0 && !m_lCurrentDustBunniesHoldingTokens.Contains(_pEnemy))
    {
      m_lTokensAvailables[i] -= 1;
      m_lCurrentDustBunniesHoldingTokens.Add(_pEnemy, i);
      /*FString Msg = FString::Printf(TEXT("Token %d tomado: actualmente %d"), i, m_lTokensAvailables[i]);
      UE_LOG(LogActorComponent, Warning, TEXT("Token %d tomado: actualmente %d"), i, m_lTokensAvailables[i]);
      GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, Msg);*/
      return true;
    }
  }
  return false;
}

bool UPoopRoomManager::ReturnDustPoopToken(ABaseEnemy* _pEnemy, UWorld* _mainWorld)
{
  if (!_mainWorld || !IsValid(_pEnemy))
  {
    UE_LOG(LogActorComponent, Error, TEXT("ReturnDustPoopToken: mainWorld or enemy is null"));
    return false;
  }

  int index = -1;
  if (m_lCurrentDustBunniesHoldingTokens.Contains(_pEnemy))
  {
    index = m_lCurrentDustBunniesHoldingTokens.FindRef(_pEnemy);
  }
  if (!(m_lTokensAvailables.IsValidIndex(index) && m_lTHRestoreToken.IsValidIndex(index) && m_lTokensMaxTimes.IsValidIndex(index)))
  {
    return false;
  }

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(_mainWorld));
  if (!pGameInstance || !IsValid(pGameInstance->m_lDustPookens))
  {
    return false;
  }

  int maxTokens = pGameInstance->m_lDustPookens->GetMaxToken(index);
  if (m_lTokensAvailables[index] < maxTokens)
  {
    //UE_LOG(LogActorComponent, Warning, TEXT("Returning token %d"), index);
    TWeakObjectPtr<UPoopRoomManager> WeakThis(this);
    _mainWorld->GetTimerManager().SetTimer(m_lTHRestoreToken[index], [WeakThis, index]()
      {
        if (WeakThis.IsValid())
        {
          if (WeakThis->m_lTokensAvailables.IsValidIndex(index))
          {
            WeakThis->m_lTokensAvailables[index] += 1;
          }
          //FString Msg = FString::Printf(TEXT("Token %d devuelto: actualmente %d"), index, WeakThis->m_lTokensAvailables[index]);
          //UE_LOG(LogActorComponent, Warning, TEXT("Token %d devuelto: actualmente %d"), index, WeakThis->m_lTokensAvailables[index]);
          //GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, Msg);
        }
      }, m_lTokensMaxTimes[index], false);

    m_lCurrentDustBunniesHoldingTokens.Remove(_pEnemy);
    m_lLastDustBunniesHoldingTokens.AddUnique(_pEnemy);
    return true;
  }

  return false;
}

bool UPoopRoomManager::SetTokens(int _index, int _newNumTokens)
{
  if (m_lTokensAvailables.IsValidIndex(_index) && _newNumTokens >= 0)
  {
    m_lTokensAvailables[_index] = _newNumTokens;
    return true;
  }
  UE_LOG(LogActorComponent, Error, TEXT("Wrong index or trying to set minus 0 tokens"));
  return false;
}

void UPoopRoomManager::ResetAllTokens(UWorld* _mainWorld)
{
  for (int i = 0; i < m_lTokensAvailables.Num(); i++)
  {
    m_lTokensAvailables[i] = 0;
  }

  m_lCurrentDustBunniesHoldingTokens.Empty();
  m_lLastDustBunniesHoldingTokens.Empty();

  if (_mainWorld)
  {
    for (FTimerHandle ft : m_lTHRestoreToken)
    {
      _mainWorld->GetTimerManager().ClearTimer(ft);
    }
  }
}

bool UPoopRoomManager::SetAllTokensToMinimum(UWorld* _mainWorld)
{
  bool bReturn_ = true;
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(_mainWorld));
  if (pGameInstance)
  {
    const UTokenManagerDataAsset* pTokenData = pGameInstance->m_lDustPookens;
    if (pTokenData && !pTokenData->m_lTokenParameters.IsEmpty())
    {
      for (int i = 0; i < m_lTokensAvailables.Num(); i++)
      {
        bReturn_ &= SetTokens(i, pTokenData->GetMaxToken(i));
      }
    }
  }
  return bReturn_;
}

void UPoopRoomManager::RoomDirtLevel(bool _bActive, UWorld* _mainWorld)
{
  if (_bActive)
  {
    //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("POOKENS ACTIVE"));
    //UE_LOG(LogActorComponent, Warning, TEXT("POOKENS ACTIVE"));
    SetAllTokensToMinimum(_mainWorld);
  }
  else
  {
    //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("POOKENS DEACTIVE"));
    //UE_LOG(LogActorComponent, Warning, TEXT("POOKENS DEACTIVE"));
    ResetAllTokens(_mainWorld);
  }
}

float UPoopRoomManager::GetMinDPDistance() const
{
  return m_fMinDistancePoop;
}

float UPoopRoomManager::GetCPPercentage() const
{
  return m_fCanPoopPorcentage;
}

float UPoopRoomManager::GetCNPPercentage() const
{
  return m_fCanNotPoopPercentage;
}

bool UPoopRoomManager::ReservePoopPosition(const AActor* _pActor)
{
  if (!IsValid(_pActor)) return false;

  const float fMinDist = m_fMinDistancePoop * m_fMinDistancePoop;
  const FVector vPoopLocation = _pActor->GetActorLocation();

  for (const auto& Pair : m_lReservedPositions)
  {
    if (FVector::DistSquared(vPoopLocation, Pair.Value) <= fMinDist)
    {
      return false;
    }
  }
  m_lReservedPositions.Add(TWeakObjectPtr(_pActor), vPoopLocation);
  return true;
}

void UPoopRoomManager::ClearDustPoopReservation(const AActor* _pActor)
{
  if (m_lReservedPositions.Contains(_pActor))
  {
    m_lReservedPositions.Remove(_pActor);
  }
}
