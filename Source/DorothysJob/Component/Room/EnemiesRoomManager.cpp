
#include "EnemiesRoomManager.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Data/DataAssets/Enemy/TokenManagerDataAsset.h"
#include <Kismet/GameplayStatics.h>

UEnemiesRoomManager::UEnemiesRoomManager()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.bStartWithTickEnabled = true;
  SetComponentTickEnabled(true);

  m_lTokensAvailables.Init(0, 4);

  float lMaxTimes[] = { 2.f, 5.f, 3.f, 3.f };
  m_lTokensMaxTimes.Append(lMaxTimes, 4);

  m_lTHRestoreToken.SetNum(4);
}

void UEnemiesRoomManager::InitTokens(UWorld* _mainWorld)
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(_mainWorld));
  if (pGameInstance)
  {
    const UTokenManagerDataAsset* pTokenData = pGameInstance->m_lAttackTokens;
    if (pTokenData && !pTokenData->m_lTokenParameters.IsEmpty())
    {
      m_lTokensAvailables.Empty();
      m_lTokensMaxTimes.Empty();
      m_lLastEnemiesHoldingTokens.Empty();
      m_lCurrentEnemiesHoldingTokens.Empty();
      m_lTHRestoreToken.Empty();

      const int iNumTokens = pTokenData->m_lTokenParameters.Num();
      m_lTHRestoreToken.SetNum(iNumTokens);

      for (const FToken& token : pTokenData->m_lTokenParameters)
      {
        m_lTokensAvailables.Add(0);
        m_lTokensMaxTimes.Add(token.m_fRestoreTime);
      }
    }
  }
}

int32 UEnemiesRoomManager::GetAvailableOozerBunnyTokens() const
{
  int32 iTotalAvailableTokens_ = 0;
  for (int i = 0; i < m_lTokensAvailables.Num() - 1; i++)
  {
    iTotalAvailableTokens_ += m_lTokensAvailables[i];
  }

  return iTotalAvailableTokens_;
}

int32 UEnemiesRoomManager::GetAvailableGrimelingTokens() const
{
  int iGrimelingIndex = m_lTokensAvailables.Num() - 1;
  if (iGrimelingIndex >= 0)
  {
    return m_lTokensAvailables[iGrimelingIndex];
  }
  else
  {
    return 0;
  }
}

bool UEnemiesRoomManager::RequestOozerBunnyToken(ABaseEnemy* _pEnemy)
{
  if (GetAvailableOozerBunnyTokens() == 0) 
  {
    return false;
  }

  if (m_lLastEnemiesHoldingTokens.Contains(_pEnemy))
  {
    m_lLastEnemiesHoldingTokens.Remove(_pEnemy);
    return false;
  }

  for (int i = 0; i < m_lTokensAvailables.Num() - 1; i++)
  {
    if (m_lTokensAvailables[i] > 0 && !m_lCurrentEnemiesHoldingTokens.Contains(_pEnemy))
    {
      m_lTokensAvailables[i] -= 1;
      m_lCurrentEnemiesHoldingTokens.Add(_pEnemy, i);
      //FString Msg = FString::Printf(TEXT("Token %d tomado: actualmente %d"), i, m_lTokensAvailables[i]);
      //UE_LOG(LogActorComponent, Warning, TEXT("Token %d tomado: actualmente %d"), i, m_lTokensAvailables[i]);
      //GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, Msg);
      return true;
    }
  }

  return false;
}

bool UEnemiesRoomManager::ReturnOozerBunnyToken(ABaseEnemy* _pEnemy, UWorld* _mainWorld)
{
  int index = -1;
  if (m_lCurrentEnemiesHoldingTokens.Contains(_pEnemy))
  {
    index = m_lCurrentEnemiesHoldingTokens.FindRef(_pEnemy);
  }
  if (!(m_lTokensAvailables.IsValidIndex(index) && m_lTHRestoreToken.IsValidIndex(index) && m_lTokensMaxTimes.IsValidIndex(index)))
  {
    return false;
  }

  if (!_mainWorld)
  {
    UE_LOG(LogActorComponent, Error, TEXT("ReturnOozerBunnyToken: mainWorld is null"));
    return false;
  }

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(_mainWorld));
  if (!pGameInstance || !pGameInstance->m_lAttackTokens)
  {
    return false;
  }

  int maxTokens = pGameInstance->m_lAttackTokens->GetMaxToken(index);
  if (m_lTokensAvailables[index] < maxTokens)
  {
    //UE_LOG(LogActorComponent, Warning, TEXT("Returning token %d"), index, m_lTokensAvailables[index]);
    TWeakObjectPtr<UEnemiesRoomManager> WeakThis(this);
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

    m_lCurrentEnemiesHoldingTokens.Remove(_pEnemy);
    m_lLastEnemiesHoldingTokens.AddUnique(_pEnemy);
    return true;
  }

  return false;
}

bool UEnemiesRoomManager::RequestGrimelingToken(ABaseEnemy* _pEnemy)
{
  int iGrimelingIndex = m_lTokensAvailables.Num() - 1;

  if (GetAvailableGrimelingTokens() == 0)
  {
    /*UE_LOG(LogActorComponent, Error, TEXT("Not available tokens for Grimeling."));
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Not available tokens for Grimeling."));*/
    return false;
  }

  if (m_lLastEnemiesHoldingTokens.Contains(_pEnemy))
  {
    //UE_LOG(LogActorComponent, Error, TEXT("An enemy can't request a token two consecutive times."));
    m_lLastEnemiesHoldingTokens.Remove(_pEnemy);
    return false;
  }

  if (!m_lTokensAvailables.IsValidIndex(iGrimelingIndex))
  {
    UE_LOG(LogActorComponent, Error, TEXT("RequestGrimelingToken: index not valid"));
    return false;
  }

  if (m_lTokensAvailables[iGrimelingIndex] > 0)
  {
    m_lTokensAvailables[iGrimelingIndex] -= 1;
    m_lCurrentEnemiesHoldingTokens.Add(_pEnemy, iGrimelingIndex);

    //FString Msg = FString::Printf(TEXT("Token Grimeling tomado: actualmente %d"), m_lTokensAvailables[iGrimelingIndex]);
    //UE_LOG(LogActorComponent, Warning, TEXT("Token Grimeling tomado: actualmente %d"), m_lTokensAvailables[iGrimelingIndex]);
    //GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, Msg);

    return true;
  }
  return false;
}

bool UEnemiesRoomManager::ReturnGrimelingToken(ABaseEnemy* _pEnemy, UWorld* _mainWorld)
{
  int iGrimelingIndex = m_lTokensAvailables.Num() - 1;
  if (!(m_lTokensAvailables.IsValidIndex(iGrimelingIndex) && m_lTHRestoreToken.IsValidIndex(iGrimelingIndex) && m_lTokensMaxTimes.IsValidIndex(iGrimelingIndex)))
  {
    UE_LOG(LogActorComponent, Error, TEXT("ReturnGrimelingToken: index not valid"));
    return false;
  }

  if (!_mainWorld)
  {
    UE_LOG(LogActorComponent, Error, TEXT("ReturnGrimelingToken: mainWorld is null"));
    return false;
  }

  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(_mainWorld));
  if (!pGameInstance || !pGameInstance->m_lAttackTokens)
  {
    return false;
  }

  int maxTokens = pGameInstance->m_lAttackTokens->GetMaxToken(iGrimelingIndex);

  if (m_lCurrentEnemiesHoldingTokens.Contains(_pEnemy) && m_lTokensAvailables[iGrimelingIndex] < maxTokens)
  {
    TWeakObjectPtr<UEnemiesRoomManager> WeakThis(this);
    _mainWorld->GetTimerManager().SetTimer(m_lTHRestoreToken[iGrimelingIndex], [WeakThis, iGrimelingIndex]()
      {
        if (WeakThis.IsValid())
        {
          if (WeakThis->m_lTokensAvailables.IsValidIndex(iGrimelingIndex))
          {
            WeakThis->m_lTokensAvailables[iGrimelingIndex] += 1;

            //FString Msg = FString::Printf(TEXT("Token Grimeling devuelto: actualmente %d"), WeakThis->m_lTokensAvailables[iGrimelingIndex]);
            //UE_LOG(LogActorComponent, Warning, TEXT("Token Grimeling devuelto: actualmente %d"), WeakThis->m_lTokensAvailables[iGrimelingIndex]);
            //GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, Msg);
          }
        }
      }, m_lTokensMaxTimes[iGrimelingIndex], false);

    m_lCurrentEnemiesHoldingTokens.Remove(_pEnemy);
    m_lLastEnemiesHoldingTokens.AddUnique(_pEnemy);
    return true;
  }
  return false;
}

bool UEnemiesRoomManager::SetTokens(int _iIndex, int _iNewNumTokens)
{
  if (m_lTokensAvailables.IsValidIndex(_iIndex) && _iNewNumTokens >= 0)
  {
    m_lTokensAvailables[_iIndex] = _iNewNumTokens;
    return true;
  }
  UE_LOG(LogActorComponent, Error, TEXT("Wrong index or trying to set minus 0 tokens"));
  return false;
}

void UEnemiesRoomManager::ResetAllTokens(UWorld* _mainWorld)
{
  for (int i = 0; i < m_lTokensAvailables.Num(); i++)
  {
    m_lTokensAvailables[i] = 0;
  }

  m_lCurrentEnemiesHoldingTokens.Empty();
  m_lLastEnemiesHoldingTokens.Empty();

  if (_mainWorld)
  {
    for (int i = 0; i < m_lTHRestoreToken.Num(); i++)
    {
      _mainWorld->GetTimerManager().ClearTimer(m_lTHRestoreToken[i]);
    }
  }
}

bool UEnemiesRoomManager::SetAllTokensToMinimum(UWorld* _mainWorld)
{
  bool bReturn_ = true;
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(_mainWorld));
  if (pGameInstance)
  {
    const UTokenManagerDataAsset* pTokenData = pGameInstance->m_lAttackTokens;
    if (pTokenData && !pTokenData->m_lTokenParameters.IsEmpty())
    {
      for (int i = 0; i < m_lTokensAvailables.Num(); i++)
      {
        bReturn_ &= SetTokens(i, pTokenData->GetMaxToken(i));
      }
    }
  }
  //UE_LOG(LogActorComponent, Warning, TEXT("NEW ROOM"));
  return bReturn_;
}



