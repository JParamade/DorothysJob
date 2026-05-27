// Fill out your copyright notice in the Description page of Project Settings.


#include "CleaningGimmick.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"

ACleaningGimmick::ACleaningGimmick() 
{
  m_v2SCleaningSize = FVector2D(90.0f);
  m_fCleaningAngle = 90.f;
  m_v2SCleaningPivot = FVector2D(0.5f);
}

void ACleaningGimmick::GetHit()
{

  m_pDrawing->Clean(m_pCleaningTexture, m_v2SCleaningSize, GetOwner()->GetActorRotation().Yaw + m_fCleaningAngle, m_v2SCleaningPivot);

}
