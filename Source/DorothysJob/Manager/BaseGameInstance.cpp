/************************************************************************
 * @description: Represents the Base Game Instance.
 * @author: David Gonzalez
 * @date: 30/05/2025
 * @edited_by: Jaime Paramo
 * @edited_by: Aurora Fernandez
 *             Josephine Esposito
 *             Jorge Duart
 ************************************************************************/

#include "BaseGameInstance.h"
#include "DorothysJob/Utils/CombatSystem/DamageCalculator.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Data/DataAssets/Tutorial/EventTutorialDataAsset.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataTables/LevelData.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

 // Manager
#include "DorothysJob/Manager/NonPersistent/TipManager.h"
#include "DorothysJob/Manager/Dialogs/DialogsManager.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

void UBaseGameInstance::Init()
{
  Super::Init();

  EnemiesKilledInEndless = 0;

  if (IsValid(m_pTypesTable))
  {
    DamageCalculator::SetTypeData(m_pTypesTable->m_mTypeTable);
  }

  m_pTipManager = NewObject<UTipManager>(this);
  m_pTipManager->Initialize(m_pTipData);

  m_pDialogsManager = NewObject<UDialogsManager>(this);
  m_pDialogsManager->Init();

  m_pEventManager = NewObject<UEventManager>(this);
  m_pEventManager->Init(m_pDialogsManager, m_pEWDataAsset, m_pGeneralEventsDataAsset);

  for (int32 i = 0; i <= 10; i++)
  {
    m_oHubEventTriggered.Add(i, 0);
  }

  //Temporal:
  m_oEventUseData = SyncFirstUseDataFromAsset();
}

void UBaseGameInstance::Shutdown()
{
  if (UAchievementSubsystem* pAchSubsystem = GetSubsystem<UAchievementSubsystem>())
  {
    pAchSubsystem->SaveAchievements();
  }

  Super::Shutdown();
}

void UBaseGameInstance::LoadLevelWithLoadingScreen()
{
  if (IsValid(m_pLoadingScreenClass))
  {
    if (UUserWidget* LoadingWidget = CreateWidget<UUserWidget>(GetWorld(), m_pLoadingScreenClass))
    {
      LoadingWidget->AddToViewport();
    }
  }

  if (m_pProceduralLevel.IsValid())
  {
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
      {
        m_pProceduralLevel.LoadSynchronous();
        UGameplayStatics::OpenLevel(this, FName(*m_pProceduralLevel.GetAssetName()));
      }, 0.1f, false);
  }
}

UTipManager* UBaseGameInstance::GetTipManager() const
{
  return m_pTipManager;
}

UDialogsManager* UBaseGameInstance::GetDialoguesManager() const
{
  return m_pDialogsManager;
}

UEventManager* UBaseGameInstance::GetEventManager() const
{
  return m_pEventManager;
}

TMap<FName, bool> UBaseGameInstance::SyncFirstUseDataFromAsset()
{
  if (!IsValid(m_pGeneralEventsDataAsset))
  {
    return m_oEventUseData;
  }

  m_oEventUseData.Empty();

  for (const auto& Pair : m_pGeneralEventsDataAsset->m_oAllEventData)
  {
    const FName& Key = Pair.Key;
    const FEventDataArray& EventData = Pair.Value;

    m_oEventUseData.Add(Key, EventData.m_bHasAlreadyAppear);
  }

  return m_oEventUseData;
}

void UBaseGameInstance::UpdateDataAssetFromFirstUseData()
{
  if (!IsValid(m_pGeneralEventsDataAsset)) return;

  for (const auto& Pair : m_oEventUseData)
  {
    FName Key = Pair.Key;

    if (FEventDataArray* EventData = m_pGeneralEventsDataAsset->m_oAllEventData.Find(Key))
    {
      EventData->m_bHasAlreadyAppear = Pair.Value;
    }
  }
}

void UBaseGameInstance::ResetTriggersUseData()
{
  SyncFirstUseDataFromAsset();
  for (auto& Pair : m_oEventUseData)
  {
    Pair.Value = false;
  }

  for (auto& Pair : m_oHUDData)
  {
    if (Pair.Key != EHUDSection::LIFE)
    {
      Pair.Value = false;
    }
  }
  m_oHubEventTriggered.Empty();
  for (int32 i = 0; i <= 10; i++)
  {
    m_oHubEventTriggered.Add(i, 0);
  }
  UpdateDataAssetFromFirstUseData();
}


bool UBaseGameInstance::UpdateAvailableWeaponsAndConsumables(int _iDay)
{
  TArray<FLevelData*> m_lLevelData;
  if (IsValid(m_pLevelData))
  {
    m_pLevelData->GetAllRows(TEXT("Rooms"), m_lLevelData);
    if (m_lLevelData.IsValidIndex(_iDay))
    {
      m_lWeapons.Empty();
      m_lConsumables.Empty();
      m_lWeapons = m_lLevelData[_iDay]->m_lUnlockedWeapons;
      m_lConsumables = m_lLevelData[_iDay]->m_lUnlockedConsumables;
      return true;
    }
  }
  return false;
}

bool UBaseGameInstance::ResetSelectedWeaponsAndConsumables()
{
  TArray<FLevelData*> m_lLevelData;
  if (IsValid(m_pLevelData))
  {
    m_pLevelData->GetAllRows(TEXT("Rooms"), m_lLevelData);
    if (m_lLevelData.IsValidIndex(0))
    {
      m_lSelectedWeapons.Empty();
      m_lSelectedConsumables.Empty();
      m_lSelectedWeapons = m_lLevelData[0]->m_lUnlockedWeapons;
      m_lSelectedConsumables = m_lLevelData[0]->m_lUnlockedConsumables;
      return true;
    }
  }
  return false;
}

void UBaseGameInstance::UpdateSelectedWeapons()
{
  const UWeaponDataAsset* pLiquidWeapon = nullptr;
  const UWeaponDataAsset* pDustWeapon = nullptr;

  for (const UWeaponDataAsset* pWeaponAsset : m_lSelectedWeapons)
  {
    if (pWeaponAsset)
    {
      if (m_lWeapons.Contains(pWeaponAsset))
      {
        pWeaponAsset->m_eWeaponCleanType == EDirtType::Liquid ? pLiquidWeapon = pWeaponAsset : pDustWeapon = pWeaponAsset;
        continue;
      }
      else
      {
        TObjectPtr<const UWeaponDataAsset>* pFoundWeapon = m_lWeapons.FindByPredicate(
          [pWeaponAsset](const UWeaponDataAsset* pWeapon)
          {
            return pWeapon && pWeaponAsset && pWeapon->m_eWeaponCleanType == pWeaponAsset->m_eWeaponCleanType;
          }
        );

        if (pFoundWeapon)
        {
          pWeaponAsset->m_eWeaponCleanType == EDirtType::Liquid ? pLiquidWeapon = pFoundWeapon->Get() : pDustWeapon = pFoundWeapon->Get();
        }
      }
    }
  }

  if (m_lSelectedWeapons.Num() < 2)
  {
    TObjectPtr<const UWeaponDataAsset>* pFoundDustWeapon = m_lWeapons.FindByPredicate(
      [](const UWeaponDataAsset* pWeapon)
      {
        return pWeapon && pWeapon->m_eWeaponCleanType == EDirtType::Dust;
      }
    );
    if (pFoundDustWeapon) pDustWeapon = pFoundDustWeapon->Get();
  }

  m_lSelectedWeapons.Empty();
  if (pLiquidWeapon) m_lSelectedWeapons.Add(pLiquidWeapon);
  if (pDustWeapon) m_lSelectedWeapons.Add(pDustWeapon);
}

void UBaseGameInstance::FilterSelectedConsumbles(const UConsumableDataAsset* &_pFirstCons, const UConsumableDataAsset* _pSecondCons)
{
  if (!m_lConsumables.Contains(_pFirstCons))
  {
    TObjectPtr<const UConsumableDataAsset>* pFoundFirstCons = m_lConsumables.FindByPredicate(
      [_pSecondCons](const UConsumableDataAsset* pCons)
      {
        return pCons && _pSecondCons && pCons != _pSecondCons;
      }
    );
    if (pFoundFirstCons)
    {
      m_lSelectedConsumables.Add(pFoundFirstCons->Get());
      _pFirstCons = pFoundFirstCons->Get();
    }
  }
  else
  {
    m_lSelectedConsumables.Add(_pFirstCons);
  }
}

void UBaseGameInstance::UpdateSelectedConsumables()
{
  if (m_lConsumables.IsEmpty())
  {
    m_lSelectedConsumables.Empty();
    return;
  }
  else if (m_lSelectedConsumables.IsEmpty())
  {
    for (const UConsumableDataAsset* pConsumableAsset : m_lConsumables)
    {
      if (pConsumableAsset) m_lSelectedConsumables.Add(pConsumableAsset);
      if (m_lSelectedConsumables.Num() == 2) break;
    }
    return;
  }

  if (m_lSelectedConsumables.Num() == 2)
  {
    const UConsumableDataAsset* pFirstConsumableAsset = m_lSelectedConsumables[0];
    const UConsumableDataAsset* pSecondConsumableAsset = m_lSelectedConsumables[1];
    m_lSelectedConsumables.Empty();
    FilterSelectedConsumbles(pFirstConsumableAsset, pSecondConsumableAsset);
    FilterSelectedConsumbles(pSecondConsumableAsset, pFirstConsumableAsset); 
  }
}
