// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseActorComponent.h"
#include "Drawing.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFloorFoundSignature);

enum class EDrawColor : uint8;
enum class EDrawTier : uint8;
class ABaseFloor;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Drawing), meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UDrawing : public UBaseActorComponent
{
	GENERATED_BODY()

public:

  virtual void BeginPlay() override;
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnFloorFoundSignature OnFloorFound;

  /**
   * @brief Clean the floor
   * @param _texture The texture that is use clean
   * @param _size  The size it use to clean
   * @param _angle  The algle that it use to display the texture
   * @param _pivot  The pivot of the texture
   */
  UFUNCTION(BlueprintCallable)
  void Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot);

  /**
   * @brief Clean the floor
   * @param _texture The texture that is use clean
   * @param _size  The size it use to clean
   * @param _angle  The algle that it use to display the texture
   * @param _pivot  The pivot of the texture
   */
  void Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot, EDrawTier _tier);

  /**
   * @brief Clean the floor
   * @param _texture The texture that is use clean
   * @param _size  The size it use to clean
   * @param _angle  The algle that it use to display the texture
   * @param _pivot  The pivot of the texture
   */
  //UFUNCTION(/*BlueprintCallable*/)
  void Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot, FVector _position);


  /**
 * @brief Clean the floor
 * @param _texture The texture that is use clean
 * @param _size  The size it use to clean
 * @param _angle  The algle that it use to display the texture
 * @param _pivot  The pivot of the texture
 */
 //UFUNCTION(/*BlueprintCallable*/)
  void Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot, FVector _position, EDrawTier tier);

  /**
   * @brief Unclean the floor
   * @param _texture The texture that is use to unclean 
   * @param _size The size that it use to unclean
   * @param _angle The angle it use to unclean
   * @param _pivot The pivot of the texture
   * @param _colorDraw The color of the drawing
   * @param _tierDraw Tier of the drawing
   */
  UFUNCTION(BlueprintCallable)
  void Dirt(UTexture* _texture, UTexture* _textureCleaning,  FVector2D _size, float _angle, FVector2D _pivot, EDrawColor _colorDraw, EDrawTier _tierDraw);

  /**
   * @brief Draw on a specific flo
   * @param _floor The flo
   * @param _texture The texture to paint
   * @param _textureCleaning The texture to clean
   * @param _size The sizu
   * @param _angle The angle
   * @param _pivot The pivor of the texture
   * @param _colorDraw The color
   * @param _tierDraw The tier
   * @param _isCleaning If it is cleaning
   * @param _position position
   */
  void DrawOnFloor(ABaseFloor* _floor, UTexture* _texture, UTexture* _textureCleaning, FVector2D _size, float _angle, FVector2D _pivot, EDrawColor _colorDraw, EDrawTier _tierDraw, bool _isCleaning, FVector _position);

  /**
   * @brief TO find a flo
   * @return If it has finded
   */
  bool FindFloor();

  /**
   * @brief Check the flo
   * @return If it has checkted
   */
  bool CheckFloor();

  void UpdatePercentageInFloors();



#pragma region Getter

  /**
   * @brief Getter of the flo
   * @return the flo
   */
  UFUNCTION(BlueprintPure)
  ABaseFloor* GetFloor() const;


  float GetNotCleaningPercentage();
#pragma endregion

protected:


private:

  /**
   * @brief Current position on the grid / the flo
   */
  UPROPERTY(VisibleAnywhere, Category = "Visualization")
  TObjectPtr<ABaseFloor> m_currentFloor;


};
