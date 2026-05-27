/************************************************************************
 * @description: Represents the Dissolvable Wall.
 * @author: Pablo Velasco
 * @date: 23/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DissolvableBookcase.h"
#include "DissolvableWall.generated.h"

UCLASS()
class DOROTHYSJOB_API ADissolvableWall : public ADissolvableBookcase
{
  GENERATED_BODY()

public:
  ADissolvableWall();

  virtual void Hide() override;

protected:
  virtual void BeginPlay() override;

  virtual void HideChildren() override;

  void ChangePlinth();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMeshComponent> VisiblePlinthMesh;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMesh> ChangedPlinthMesh;

  TObjectPtr<UStaticMesh> OriginalPlinthMesh;
};
