#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Inventory.generated.h"

class ADA_Item;
class ADA_Charater;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemCountChangeDelegate, const FString&, _ItemName, int32, _TotalItemCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRegistItemDelegate, const FString&, _ItemName, UTexture2D*, _ItemImage, int32, _ItemCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChangeDelegate, int32, _TotalGold);

//플레이어의 아이템을 관리해주는 컴포넌트
UCLASS(ClassGroup = (DC_Player), meta = (BlueprintSpawnableComponent))
class DEFENDER_API UDC_Inventory : public UDC_PawnComponent
{
	GENERATED_BODY()

public:
	//Delegate called when the number of items changes
	UPROPERTY(BlueprintAssignable)
	FOnItemCountChangeDelegate onItemCountChange;

	//Delegate called when the currently used item changes
	UPROPERTY(BlueprintAssignable)
	FOnRegistItemDelegate onRegistItem;

	//Delegate called when the currently held gold changes
	UPROPERTY(BlueprintAssignable)
	FOnGoldChangeDelegate onGoldChange;

protected:

private:
	//Data table containing item information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ItemData", meta = (AllowPrivateAccess = true))
	UDataTable* itemDataTable;

	//A map that shows the number of items by name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "ItemData", meta = (AllowPrivateAccess))
	TMap<FString, int32> itemMap;
	
	//Items currently registered as used items
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "UseItem", meta = (AllowPrivateAccess = true))
	ADA_Item* currentUseItem;

	//Gold held
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Gold", meta = (AllowPrivateAccess))
	int32 gold;

public:
	UDC_Inventory();
	/*
	* Register items to use
	* @param[in] _ItemName Name of item to use
	*/
	UFUNCTION(BlueprintCallable, category = "Item")
	void RegistItem(const FString& _ItemName);
	/*
	* Add the number of items
	* @param[in] _ItemName Item name to add quantity to
	* @param[in] _Count Number to add
	*/
	UFUNCTION(BlueprintCallable, category = "Item")
	bool AddItem(const FString& _ItemName, int32 _Count);
	/*
	* Set the number of items
	* @param[in] _ItemName Item name to add quantity to
	* @param[in] _Count Number to set
	*/
	UFUNCTION(BlueprintCallable, category = "Item")
	void SetItem(const FString& _ItemName, int32 _Count);
	/*
	* Earn gold. If you enter a negative number, it will be deducted, but if the result is less than 0 gold, it will return a failure.
	* @param[in] _AddGold Gold to Earn
	* @return In case of failure, the total gold is negative
	*/
	UFUNCTION(BlueprintCallable, category = "Gold")
	bool AddGold(int32 _AddGold);
	/*
	* Set the gold you currently have
	* @param[in] _NewGold Gold to set
	*/
	UFUNCTION(BlueprintCallable, category = "Gold")
	void SetGold(int32 _NewGold);
	/*
	* Use the currently registered item
	*/
	void UseItem();
	/*
	* Get a map of the items player owns
	* @return Returns const TMap<FString, int32>&
	*/
	FORCEINLINE const TMap<FString, int32>& GetItemMap()const { return itemMap; }
	/*
	* Get the gold you currently have
	* @return Returns int32
	*/
	FORCEINLINE int32 GetCurrentGold() { return gold; }
	/*
	* Get the currently registered item
	* @return Returns ADA_Item*
	*/
	FORCEINLINE ADA_Item* GetCurrentRegistedItem()const { return currentUseItem; }

protected:

private:

};
