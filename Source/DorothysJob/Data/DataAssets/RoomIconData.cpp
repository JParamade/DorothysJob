#include "RoomIconData.h"
#include "DorothysJob/Data/DataTables/LevelData.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"

FRoomTypeInfo::FRoomTypeInfo() :
  Type(ERoomType::LIVINGROOM)
{
}

UTexture2D* URoomIconData::GetRewardTextureForType(UBaseRoom* Room) const
{
	if (!IsValid(Room) || Room->GetRoomShape() == ERoomShape::Q1 || Room->GetDirtStage() == EDirtStage::CLEAN)
	{
		return IsValid(RewardNothing) ? RewardNothing : nullptr;
	}

	switch (Room->GetRoomType())
	{
	case ERoomType::LIVINGROOM:
		return IsValid(RewardLivingroom) ? RewardLivingroom : nullptr;
	case ERoomType::KITCHEN:
		return IsValid(RewardKitchen) ? RewardKitchen : nullptr;
	case ERoomType::BATHROOM:
		return IsValid(RewardBathroom) ? RewardBathroom : nullptr;
	case ERoomType::GREENHOUSE:
		return IsValid(RewardGreenhouse) ? RewardGreenhouse : nullptr;
	case ERoomType::BEDROOM:
		return IsValid(RewardBathroom) ? RewardBathroom : nullptr;
	default:
		return IsValid(RewardNothing) ? RewardNothing : nullptr;
	}
}
