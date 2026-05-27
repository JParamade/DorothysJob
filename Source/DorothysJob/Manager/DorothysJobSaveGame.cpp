

#include "DorothysJobSaveGame.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/ConsumableDataAsset.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"

UDorothysJobSaveGame::UDorothysJobSaveGame():
  m_iSavedCurrentLevel(0),
  m_iSavedHighestLevelAchieved(0)
{
  m_lFinalScores.Init(FFinalSavedScore(), 11);
}
