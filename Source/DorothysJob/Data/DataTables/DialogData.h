/************************************************************************
 * @description: Represents the structure for the dialogues
 * @author: Josephine Esposito
 * @date: 04/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DialogData.generated.h"

UENUM(BlueprintType)
enum class EDialogPosition : uint8
{
  Start UMETA(DisplayName = "Start Dialogue"),
  End UMETA(DisplayName = "End Dialogue")
};

struct FDialogBlock
{
  FName m_sLeftCharacterName;

  FName m_sLeftCharacterSprite;

  FName m_sRightCharacterName;

  FName m_sRightCharacterSprite;

  int32 m_iCharacterTalking = 0;

  FText m_sVoiceAudio;

  FText m_sDialogueText;
};

/**
 * @brief The structure for the dialogues
 */
USTRUCT(BlueprintType)
struct FDialogData : public FTableRowBase
{
  GENERATED_BODY()

  /**
   * @brief The name of the sequence of the dialog
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Sequence"))
  FText m_sSequence;

  /**
   * @brief The index of the current scene
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scene"))
  int32 m_iScene = 0;

  /**
   * @brief The name to display of the character on the left
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Left Character Name"))
  FName m_sLeftCharacterName;

  /**
   * @brief The ID of the sprite to use for the character on the left
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Left Character Sprite"))
  FName m_sLeftCharacterSprite;

  /**
   * @brief The name to display of the character on the right
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Right Character Name"))
  FName m_sRightCharacterName;

  /**
   * @brief The ID of the sprite to use for the character on the right
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Right Character Sprite"))
  FName m_sRightCharacterSprite;

  /**
   * @brief An index stating which one of the characters is talking
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Character Talking", ClampMin = "1", ClampMax = "2"))
  int32 m_iCharacterTalking = 0;

  /**
   * @brief The audio track ID of the voice to reproduce
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Voice Audio"))
  FText m_sVoiceAudio;

  /**
   * @brief The text to display in the dialog box
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Dialog Text", MultiLine = true))
  FText m_sDialogText;
};
