/************************************************************************
 * @description: Represents the Dissolvable Door.
 * @author: Pablo Velasco
 * @date: 23/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DissolvableWall.h"
#include "DissolvableDoor.generated.h"

class UBoxComponent;
class ULevelManager;

UCLASS()
class DOROTHYSJOB_API ADissolvableDoor : public ADissolvableWall
{
  GENERATED_BODY()
public:

  ADissolvableDoor();

protected:

  virtual void BeginPlay() override;

public:

  virtual void Hide() override;

#pragma region | Open/Close Functions
  UFUNCTION(BlueprintCallable)
  void CloseDoor();

  UFUNCTION(BlueprintCallable)
  void OpenDoor();
#pragma endregion

protected:

# pragma region | Meshes Components
  //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Icon", meta = (DisplayName = "Icon Array"))
  //TMap<ERoomType, TObjectPtr<UMaterialInterface>> m_pIconMaterialList;

  //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Icon", meta = (DisplayName = "Icon"))
  //TObjectPtr<UMaterialInterface> m_pIconMaterial;

  //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  //TObjectPtr<UStaticMeshComponent> OpenDoorIcon;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMeshComponent> OpenDoorEmission;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMeshComponent> TapeMesh;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMeshComponent> Plane;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMesh> VisibleParentMeshClosed;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMesh> ChangedParentMeshClosed;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Mesh")
  TObjectPtr<UStaticMesh> TapeMeshComponent;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Trigger")
  TObjectPtr<UBoxComponent> TriggerZone;
#pragma endregion

  virtual void HideChildren() override;

  virtual void TryRegisterInRoom(UBaseRoom* Room) override;

private:

#pragma region | Private Variables
  UPROPERTY(VisibleAnywhere, Category = "Bola13|Room")
  TObjectPtr<UBaseRoom> MyRoom;

  UStaticMesh* VisibleParentMeshOpen;

  UStaticMesh* ChangedParentMeshOpen;

  TObjectPtr<ULevelManager> m_pLevelManager;

  bool bIsDoorClosed = false;

  bool bPlayerWasInside = false;

  bool m_bHideIcon = false;
#pragma endregion

#pragma region | Open/Close Private Functions
  void CloseRoot();

  void OpenRoot();

  void HideEmission();

  void ShowEmission();
#pragma endregion

#pragma region | Overlap Functions
  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  UFUNCTION()
  void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

public:

#pragma region Getters&Setters
  void SetRoom(TObjectPtr<UBaseRoom> NewRoom);

  const TObjectPtr<UBaseRoom>& GetRoom() const;

  bool IsDoorClosed() const;

  void SetIconMaterial(ERoomType _RoomType, bool _ShowIcon);
#pragma endregion
};
