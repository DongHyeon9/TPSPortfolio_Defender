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
	//�߰��ҷ��� �������� Ű�� �´� �������� ������ ���Ѵ�
	auto currentItemData = itemDataTable->FindRow<FDFItem>(*_ItemName, TEXT(""));
	DFCHECK(currentItemData != nullptr, false);
	
	if (itemMap.Contains(_ItemName))
		itemMap[_ItemName] += _Count;

	//�������� ���¾������̶�� ���� �߰��Ѵ�
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
	//���� ���� ��带 ���Ѵ�
	//������� ��带 ������� ������ ���и� �����Ѵ�
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

	//�������� ����ϰ� CDO�� �����ؼ� �������ִ´�
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

	//CDO���� �������̽��� �������ִ� UseItem�Լ��� �����Ѵ�
	if (itemMap[currentUseItem->GetItemName()] <= 0) return;

	IDI_UseableItem::Execute_UseItem(currentUseItem, GetOwnerCharacter());
	AddItem(currentUseItem->GetItemName(), -1);
}
