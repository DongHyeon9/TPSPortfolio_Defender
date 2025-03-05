#pragma once

#include "Actor/DA_Actor.h"
#include "Interface/DI_UseableItem.h"
#include "Interface/DI_ObjectPoolingActor.h"
#include "DA_Item.generated.h"

class UStaticMeshComponent;
class USphereComponent;

//사용가능한 아이템의 최상위 클래스
UCLASS()
class DEFENDER_API ADA_Item :
	public ADA_Actor,
	public IDI_UseableItem,
	public IDI_ObjectPoolingActor
{
	GENERATED_BODY()

public:

protected:

private:
	//Root component of the item actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "DefaultConfig", meta = (AllowPrivateAccess = true))
	USceneComponent* rootComp;

	//Static mesh of item actors
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category="DefaultConfig",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* staticMeshComp;

	//Item collision determination
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "DefaultConfig", meta = (AllowPrivateAccess = true))
	USphereComponent* sphereCollision;

	//Data table containing item information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ItemData", meta = (AllowPrivateAccess = true))
	UDataTable* itemDataTable;

	//Name of current item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ItemData", meta = (AllowPrivateAccess = true))
	FString itemName;

	//number of items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ItemData", meta = (AllowPrivateAccess = true))
	int32 itemCount;

	FDFItem*				currentItemData;			//현재 아이템의 정보
	bool					bIsActive;					//아이템이 활성화 돼있는지 여부

public:
	ADA_Item();
	/*
	* A function to be called when the item is used. This function is an interface function
	* @param[in] _UsedCharacter Characters using items
	*/
	virtual void UseItem_Implementation(ADA_Character* _UsedCharacter)override;
	/*
	* Gets the collision used to determine collision with an item.
	* @return Retruns USphereComponent*
	*/
	FORCEINLINE USphereComponent* GetSphereCollision()const { return sphereCollision; }
	/*
	* Get the Static Mesh that handles the visual aspect of the item.
	* @return Retruns UStaticMeshComponent*
	*/
	FORCEINLINE UStaticMeshComponent* GetStaticMesh()const { return staticMeshComp; }
	/*
	* Get the name of the item
	* @return Retruns const FString&
	*/
	FORCEINLINE const FString& GetItemName()const { return itemName; }
	/*
	* Sets the active state of an item
	* @param[in] _bIsActive Active state to set
	*/
	virtual void ActiveSetting_Implementation(bool _bIsActive)override;

 protected:
	virtual void BeginPlay()override;

	virtual void MeshActive_Implementation(bool _bIsActive)override;
	virtual void CollisionActive_Implementation(bool _bIsActive)override;

	UFUNCTION()
	virtual void BeginOverlapEvent(UPrimitiveComponent* _OverlappedComp, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);

private:

};
