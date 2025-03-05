#include "Widget/InGame/Information/DWP_EquipDropButton.h"
#include "Component/Player/DC_Weapon.h"
#include "Component/Player/DC_Inventory.h"
#include "Actor/Player/DA_Player.h"

#include "Components/Button.h"

UDWP_EquipDropButton::UDWP_EquipDropButton(const FObjectInitializer& _ObjectInitializer) :
	Super(_ObjectInitializer)
{
}

void UDWP_EquipDropButton::SetItemName(const FString& _ItemName)
{
	itemName = _ItemName;
}

void UDWP_EquipDropButton::SetItemType(EITEM_TYPE _ItemType)
{
	currentItemType = _ItemType;
}

void UDWP_EquipDropButton::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Equip->OnClicked.AddDynamic(this, &UDWP_EquipDropButton::OnClickedEquip);
	btn_Drop->OnClicked.AddDynamic(this, &UDWP_EquipDropButton::OnClickedDrop);
}

void UDWP_EquipDropButton::NativeDestruct()
{
	Super::NativeDestruct();

	btn_Equip->OnClicked.RemoveDynamic(this, &UDWP_EquipDropButton::OnClickedEquip);
	btn_Drop->OnClicked.RemoveDynamic(this, &UDWP_EquipDropButton::OnClickedDrop);
}

void UDWP_EquipDropButton::OnClickedDrop()
{
	DFLOG_N(Display);
	switch (currentItemType) {
	case EITEM_TYPE::WEWAPON: {
		GetOwnerPlayerCharacter()->GetWeaponComponent()->DropWeapon(itemName);
		break;
	}
	case EITEM_TYPE::USE: {
		GetOwnerPlayerCharacter()->GetInventoryComponent()->AddItem(itemName, -1);
		break;
	}
	}
	SetVisibility(ESlateVisibility::Hidden);
}

void UDWP_EquipDropButton::OnClickedEquip()
{
	DFLOG_N(Display);
	switch (currentItemType) {
	case EITEM_TYPE::WEWAPON: {
		GetOwnerPlayerCharacter()->GetWeaponComponent()->SetWeapon(itemName);
		break;
	}
	case EITEM_TYPE::USE: {
		GetOwnerPlayerCharacter()->GetInventoryComponent()->RegistItem(itemName);
		break;
	}
	}
	SetVisibility(ESlateVisibility::Hidden);
}