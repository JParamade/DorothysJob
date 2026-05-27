/************************************************************************
 * @description: Represents the Dissolvable Bookcase.
 * @author: Pablo Velasco
 * @date: 23/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DissolvableActor.h"
#include "DissolvableBookcase.generated.h"

UCLASS()
class DOROTHYSJOB_API ADissolvableBookcase : public ADissolvableActor
{
  GENERATED_BODY()
public:
  virtual void Hide();

protected:
  virtual void BeginPlay() override;

  virtual void HideChildren();
};
