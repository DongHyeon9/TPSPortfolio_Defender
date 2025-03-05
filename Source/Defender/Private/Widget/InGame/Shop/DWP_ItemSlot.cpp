#include "Widget/InGame/Shop/DWP_ItemSlot.h"
#include "Actor/Player/DA_Player.h"
#include "Component/Player/DC_Weapon.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/OverlaySlot.h"

UDWP_ItemSlot::UDWP_ItemSlot(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
}

void UDWP_ItemSlot::InitializeItemSlot(const FItemSlotInitilizer& _Initializer)
{
	DFLOG_N(Display);
	DFCHECK(_Initializer.potionData != nullptr || _Initializer.weaponData != nullptr);

	//�̹����� ������ �������ش�
	UWidgetLayoutLibrary::SlotAsOverlaySlot(img_Item)->SetPadding(_Initializer.inPadding);

	//���������� ��� �ʱ�ȭ ���ش�
	if (_Initializer.potionData != nullptr) {
		itemName = _Initializer.potionData->itemName;
		img_Item->SetBrushFromTexture(_Initializer.potionData->iconImage);
		txt_Price->SetText(SetGoldText(_Initializer.potionData->itemPrice));
		currentItemType = EITEM_TYPE::USE;
	}

	//���� ������ ��� �ʱ�ȭ ���ش�
	else if (_Initializer.weaponData != nullptr) {
		UDC_Weapon* weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
		weaponComp->onWeaponChange.AddDynamic(this, &UDWP_ItemSlot::OnWeaponChangeStated);
		itemName = _Initializer.weaponData->weaponData.weaponName;
		img_Item->SetBrushFromTexture(_Initializer.weaponData->weaponData.weaponImage);
		currentItemType = EITEM_TYPE::WEWAPON;

		if (_Initializer.weaponData->ownedState == EWEAPON_OWNED_STATE::NONE_OWNED)
			txt_Price->SetText(SetGoldText(_Initializer.weaponData->weaponData.weaponPrice));
		else
			txt_Price->SetText(SetGoldText(_Initializer.weaponData->weaponData.magazinePrice));
	}

	txt_ItemName->SetText(FText::FromString(itemName));

	btn_Item->OnClicked.AddDynamic(this, &UDWP_ItemSlot::OnClicked);
}

void UDWP_ItemSlot::NativeDestruct()
{
	Super::NativeDestruct();

	if (currentItemType == EITEM_TYPE::WEWAPON) {
		UDC_Weapon* weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
		weaponComp->onWeaponChange.RemoveDynamic(this, &UDWP_ItemSlot::OnWeaponChangeStated);
	}
	btn_Item->OnClicked.RemoveDynamic(this, &UDWP_ItemSlot::OnClicked);
}

void UDWP_ItemSlot::OnClicked()
{
	DFLOG_N(Display);
	onButtonClicked.Broadcast(itemName, currentItemType);
}

void UDWP_ItemSlot::OnWeaponChangeStated(const FString& _WeaponName, EWEAPON_OWNED_STATE _WeaponOwnedState)
{
	//������ ���°� ���������� ������ �����Ѵ�
	if (itemName != _WeaponName) return;

	auto weaponData = weaponDataTable->FindRow<FDFWeaponData>(*_WeaponName, TEXT(""));

	switch (_WeaponOwnedState)
	{
	case EWEAPON_OWNED_STATE::OWNED:
	case EWEAPON_OWNED_STATE::EQUIP: {
		txt_Price->SetText(SetGoldText(weaponData->magazinePrice));
		break;
	}
	case EWEAPON_OWNED_STATE::NONE_OWNED: {
		txt_Price->SetText(SetGoldText(weaponData->weaponPrice));
		break;
	}
	}
}
