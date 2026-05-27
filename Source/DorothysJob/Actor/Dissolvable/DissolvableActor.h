/************************************************************************
 * @description: Represents the Base Dissolvable Actor.
 * @author: Pablo Velasco
 * @date: 23/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DissolvableActor.generated.h"

UENUM(BlueprintType)
enum class ERotation : uint8
{

  ROT0      UMETA(DisplayName = "ROT0"),

  ROT45      UMETA(DisplayName = "ROT45"),

  ROT90      UMETA(DisplayName = "ROT90"),

  ROT135      UMETA(DisplayName = "ROT135"),

  ROT180      UMETA(DisplayName = "ROT180"),

  ROT225       UMETA(DisplayName = "ROT225"),

  ROT270       UMETA(DisplayName = "ROT270"),

  ROT315      UMETA(DisplayName = "ROT315"),

  NOE      UMETA(DisplayName = "NOE"),

};

UCLASS()
class DOROTHYSJOB_API ADissolvableActor : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ADissolvableActor();

  UFUNCTION(BlueprintCallable)
  virtual void Hide();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
  ERotation Rotation = ERotation::NOE;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

  void ChangeRootMesh();

  virtual void TryRegisterInRoom(UBaseRoom* Room);

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13")
  TObjectPtr<USceneComponent> Rotator;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMeshComponent> VisibleParentMesh;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMesh> ChangedParentMesh;

  TObjectPtr<UStaticMesh> OriginalParentMesh;

  UPROPERTY(EditAnywhere)
  bool bUsingOriginalMesh;

private:
  UPROPERTY(EditAnywhere, Category = "Bola13")
  bool bIsSecondFloorProp = false;  
  
  UPROPERTY(EditAnywhere, Category = "Bola13")
  bool bIsConcaveCorner = false;

};
