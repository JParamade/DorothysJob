// Fill out your copyright notice in the Description page of Project Settings.


#include "TokenManagerDataAsset.h"

int UTokenManagerDataAsset::GetMaxToken(int _iIndex) const
{
  if (m_lTokenParameters.IsValidIndex(_iIndex))
  {
    return m_lTokenParameters[_iIndex].m_iMaxTokens;
  }
  return -1;
}

int UTokenManagerDataAsset::GetTokenTime(int _iIndex) const
{
  if (m_lTokenParameters.IsValidIndex(_iIndex))
  {
    return m_lTokenParameters[_iIndex].m_fRestoreTime;
  }
  return -1;
}
