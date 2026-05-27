/************************************************************************
 * @description: Represents the structure for the sprites used in dialogs
 * @author: Josephine Esposito
 * @date: 04/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpriteDataAsset.generated.h"

class UFMODEvent;

USTRUCT(BlueprintType)
struct FSpriteAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Texture"))
	TObjectPtr<UTexture2D> m_pTexture;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Audio"))
	TObjectPtr<UFMODEvent> m_pAudio;
};

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API USpriteDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Sprite Data"))
	TMap<FName, FSpriteAsset> m_mSprite;
};
