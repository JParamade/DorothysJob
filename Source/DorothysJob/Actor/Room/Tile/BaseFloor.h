// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseTile.h"
#include "BaseFloor.generated.h"


/**
 *
 */
UCLASS()
class DOROTHYSJOB_API ABaseFloor : public ABaseTile
{
  GENERATED_BODY()

public:

#pragma region Base Functions

  virtual void BeginPlay() override;


#pragma endregion



#pragma region Tiles functions

  /**
   * @brief Search all the tiles, around
   * @return
   */

  virtual bool InitTile();


  /**
   * @brief Get the FloorTile in a position (it has a detection range of 1)
   * @param _location The locataion of the tile
   * @return The tile, if there are no Tiles it return a nullptr
   */
  ABaseFloor* GetTileAtLocation(FVector _location);

  FVector2f GetGridOffset(FIntPoint _Offset) const;
  FVector GetWorldLocation(FIntPoint _Offset) const;


#pragma endregion

#pragma region Getter
  /**
   * @brief Getter of the size
   * @return It return the size
   */
  FVector GetSize() const;


  /**
   * @brief Getter of the floor have around
   * @return The pointer to the first element
   */
  ABaseFloor* GetAroundTiles(int i) const;

  /**
   * @brief Getter of the position in the Grid
   * @param x_ Position in the X axis.
   * @param y_ Position in the Y axis.
   */
  UFUNCTION(BlueprintPure)
  void GetGridLocation(int& x_, int& y_) const;


  FString GetId() const;


#pragma endregion

#pragma region Setter

  /**
   * @brief Setter of the position of the grid
   * @param _x Position in X axis
   * @param _y POsition en Y axis
   */
  UFUNCTION(BlueprintCallable)
  void SetGridLocation(int _x, int _y);

#pragma endregion

#pragma region Events

  UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
  void InitLaunch();

#pragma endregion



  /**
   * @brief The sizu of the array
   */
  static const unsigned int m_uiArraySize = 8;


protected:

private:


  /**
   * @brief Size of the tile
   */
  UPROPERTY(EditAnywhere, Category = "Bola13|Tile")
  FVector m_vTileSize = FVector(1600, 1600, 0);

  FString m_sId = " ";


  /**
   * @brief number of elements in the array, it is static because all will have the same number of neighbour.
   */

   /**
    * @brief pointer to the others floors
    */
  UPROPERTY(EditAnywhere)
  ABaseFloor* m_pTileArray[m_uiArraySize]; // 0 = arriba, 1 = arriba derecha, 2 = derecha, 3 = Derecha abajo, 4 = abajo, 5 = Izquierda Abajo, 6 = Izquirda, 7 Izquierda Arriba  

  /**
   * @brief it is already inicialize
   */
  bool m_bHasBeenInicialize;


  /**
   * @brief Position in the X axis in the Grid
   */
  UPROPERTY(EditAnywhere, Category = "Bola13|Tile")
  int m_iX = 1000;

  /**
   * @brief Position in the Y axis in the Grid
   */
  UPROPERTY(EditAnywhere, Category = "Bola13|Tile")
  int m_iY = 1000;

};