/************************************************************************
 * @description: Represents the Default Settings Data Asset.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

// Setting Types
#include "DorothysJob/Settings/SettingTypes.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DefaultSettingsDataAsset.generated.h"

UCLASS()
class DOROTHYSJOB_API UDefaultSettingsDataAsset : public UDataAsset
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Settings", meta = (DisplayName = "Accesibilty"))
  FCustomAccesibilitySettings m_oDefaultAccesibilty;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Settings", meta = (DisplayName = "Audio"))
  FCustomAudioSettings m_oDefaultAudio;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Settings", meta = (DisplayName = "Input"))
  FCustomInputSettings m_oDefaultInput;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Settings", meta = (DisplayName = "General"))
  FCustomGeneralSettings m_oDefaultGeneral;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Settings", meta = (DisplayName = "Visual"))
  FCustomVisualSettings m_oDefaultVisual;
};
