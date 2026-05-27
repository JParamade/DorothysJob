/************************************************************************
 * @description: Represents the Setting Types struct container.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "SettingTypes.generated.h"

USTRUCT(BlueprintType)
struct FCustomAccesibilitySettings
{
  GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FCustomAudioSettings
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (DisplayName = "Master Volume"))
  float m_fMasterVolume = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (DisplayName = "Music Volume"))
  float m_fMusicVolume = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (DisplayName = "SFX Volume"))
  float m_fSFXVolume = 1.0f;
};

USTRUCT(BlueprintType)
struct FCustomInputSettings
{
  GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FCustomGeneralSettings
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Show FPS"))
  bool m_bShowFPS = false;
};

UENUM(BlueprintType)
enum class EGraphicsQuality : uint8 {
  LOW   UMETA(DisplayName = "LOW"),
  MEDIUM UMETA(DisplayName = "MEDIUM"),
  HIGH   UMETA(DisplayName = "HIGH"),
  ULTRA   UMETA(DisplayName = "ULTRA")
};

USTRUCT(BlueprintType)
struct FCustomVisualSettings
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (DisplayName = "Graphics Quality"))
  EGraphicsQuality m_eGraphicsQuality = EGraphicsQuality::HIGH;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (DisplayName = "Resolution"))
  FIntPoint m_vResolution = { 1920, 1080 };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (DisplayName = "VSync"))
  bool m_bVSync = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, meta = (DisplayName = "Framerate"))
  int32 m_iFPS = 60;
};