// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageCalculator.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"

TMap<EDirtType, FTypeEffectiveness> DamageCalculator::m_mTypeEffectivenessTable;

DamageCalculator::DamageCalculator()
{
}

DamageCalculator::~DamageCalculator()
{
}

float DamageCalculator::GetTypeDamageMultiplier(EDirtType _attackingType, EDirtType _targetType)
{
  if (const FTypeEffectiveness* row = m_mTypeEffectivenessTable.Find(_attackingType))
  {
    if (const float* fTypeEffectiveness_ = row->EffectivenessMap.Find(_targetType))
    {
      return *fTypeEffectiveness_;
    }
  }
  return 1.0f; 
}

int DamageCalculator::CalculateDamage(int _iBaseDamage, float _fMitigation, EDirtType _eAttackingType, EDirtType _eTargetType, 
  float _fBonus)
{
  float fTypeEffectiveness = GetTypeDamageMultiplier(_eAttackingType, _eTargetType);
  int iTotalDamage_ = FMath::RoundToInt((_iBaseDamage - (_iBaseDamage * _fMitigation)) * fTypeEffectiveness * _fBonus);
  return iTotalDamage_;
}

void DamageCalculator::SetTypeData(TMap<EDirtType, FTypeEffectiveness> _pNewTypeTable)
{
  m_mTypeEffectivenessTable = _pNewTypeTable;
}
