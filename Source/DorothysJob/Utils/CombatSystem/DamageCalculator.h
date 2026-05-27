/************************************************************************
 * @description: Collection of methods for the damage calculation
 * @author: Jorge Duart
 * @date: 26/06/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"

class UDirtTypesDataAsset;
struct FTypeEffectiveness;
enum class EDirtType : uint8;
/**
 * 
 */
class DOROTHYSJOB_API DamageCalculator
{
public:
	DamageCalculator();
	~DamageCalculator();

	static float GetTypeDamageMultiplier(EDirtType _attackingType, EDirtType _targetType);

	static int CalculateDamage(int _iBaseDamage, float _fMitigation, EDirtType _attackingType, EDirtType _targetType, float _fBonus);

	static void SetTypeData(TMap<EDirtType, FTypeEffectiveness> _pNewTypeTable);
private:
	static TMap<EDirtType, FTypeEffectiveness> m_mTypeEffectivenessTable;
};
