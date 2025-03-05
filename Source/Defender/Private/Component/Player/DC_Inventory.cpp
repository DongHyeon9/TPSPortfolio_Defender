#include "Component/Player/DC_Inventory.h"
#include "Item/DA_Item.h"
#include "Interface/DI_UseableItem.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

UDC_Inventory::UDC_Inventory()
{

}

bool UDC_Inventory::AddItem(const FString& _ItemName, int32 _Count)
{
	DFLOG_N(Display);
	//추가할려는 아이템의 키에 맞는 아이템의 갯수를 더한다
	auto currentItemData = itemDataTable->FindRow<FDFItem>(*_ItemName, TEXT(""));
	DFCHECK(currentItemData != nullptr, false);
	
	if (itemMap.Contains(_ItemName))
		itemMap[_ItemName] += _Count;

	//아이템이 없는아이템이라면 새로 추가한다
	else
		itemMap.Add(TTuple<FString, int32>(_ItemName, _Count));

	if (itemMap[_ItemName] < 0) {
		itemMap[_ItemName] = 0;
		return false;
	}

	onItemCountChange.Broadcast(_ItemName, itemMap[_ItemName]);
	return true;
}

void UDC_Inventory::SetItem(const FString& _ItemName, int32 _Count)
{
	DFLOG_N(Display);
	if (itemMap.Contains(_ItemName))
		itemMap[_ItemName] = _Count;
	else
		itemMap.Add(TTuple<FString, int32>(_ItemName, _Count));
	onItemCountChange.Broadcast(_ItemName, itemMap[_ItemName]);
}

bool UDC_Inventory::AddGold(int32 _AddGold)
{
	DFLOG_N(Display);
	//현재 보유 골드를 더한다
	//음수라면 골드를 원래대로 돌리고 실패를 리턴한다
	gold += _AddGold;
	if (gold < 0) {
		gold -= _AddGold;
		return false;
	}
	onGoldChange.Broadcast(gold);
	return true;
}

void UDC_Inventory::RegistItem(const FString& _ItemName)
{
	DFLOG_N(Display);
	DFCHECK(itemMap.Contains(_ItemName));

	//아이템을 등록하고 CDO를 생성해서 가지고있는다
	auto currentItem = itemDataTable->FindRow<FDFItem>(*_ItemName, TEXT(""));

	DFCHECK(currentItem != nullptr);
	
	if (currentItem->useableImplementation->ImplementsInterface(UDI_UseableItem::StaticClass())) {
		currentUseItem = GetMutableDefault<ADA_Item>(currentItem->useableImplementation);
		onRegistItem.Broadcast(currentItem->itemName, currentItem->iconImage, itemMap[currentItem->itemName]);
	}
}

void UDC_Inventory::SetGold(int32 _NewGold)
{
	gold = _NewGold;
	onGoldChange.Broadcast(gold);
}

void UDC_Inventory::UseItem()
{
	DFLOG_N(Display);
	DFCHECK(currentUseItem != nullptr);

	//CDO에서 인터페이스로 가지고있는 UseItem함수를 실행한다
	if (itemMap[currentUseItem->GetItemName()] <= 0) return;

	IDI_UseableItem::Execute_UseItem(currentUseItem, GetOwnerCharacter());
	AddItem(currentUseItem->GetItemName(), -1);
}
