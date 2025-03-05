#include "Widget/InGame/Information/DWP_WeaponSlot.h"
#include "Component/Player/DC_Weapon.h"
#include "Actor/Player/DA_Player.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

UDWP_WeaponSlot::UDWP_WeaponSlot(const FObjectInitializer& _ObjectInitializer) :
	Super(_ObjectInitializer)
{
	weaponName = FString();
}

void UDWP_WeaponSlot::InitializeWeaponSlot(const FWeaponOwnedData& _Initializer)
{
	DFLOG_N(Display);
	DFCHECK(_Initializer.weaponData.weaponName != FString());

	//������ ���¸� ������ ������ �ش�
	weaponName = _Initializer.weaponData.weaponName;
	img_Weapon->SetBrushFromTexture(_Initializer.weaponData.weaponImage);
	txt_WeaponName->SetText(FText::FromString(weaponName));
	txt_Magazine->SetText(SetMagazineText(_Initializer));
	SetWeaponOwnedState(_Initializer.ownedState);

	//źâ�� �ٲ𶧿� ���Ⱑ �ٲ� ȣ��Ǵ� ��������Ʈ�� �������ش�
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onMagazineChange.AddDynamic(this, &UDWP_WeaponSlot::SetMagazine);
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onWeaponChange.AddDynamic(this, &UDWP_WeaponSlot::SetWeaponState);
	btn_Item->OnClicked.AddDynamic(this, &UDWP_WeaponSlot::OnCliecked);
}

void UDWP_WeaponSlot::SetWeaponOwnedState(EWEAPON_OWNED_STATE _WeaponOwnedState)
{
	//���� ������ ���� ���¸� üũ�ϰ�
	currentWeaponOwnedState = _WeaponOwnedState;

	//���� ���¿� ���� ǥ���ϴ� ������ ����
	switch (currentWeaponOwnedState) {
	case EWEAPON_OWNED_STATE::OWNED: {
		img_Enable->SetColorAndOpacity(FColor::FromHex(TEXT("00000000")));
		break;
	}
	case EWEAPON_OWNED_STATE::NONE_OWNED: {
		img_Enable->SetColorAndOpacity(FColor::FromHex(TEXT("FF00001A")));
		break;
	}
	case EWEAPON_OWNED_STATE::EQUIP: {
		img_Enable->SetColorAndOpacity(FColor::FromHex(TEXT("00FF001A")));
		break;
	}
	}
}

void UDWP_WeaponSlot::SetWeaponName(const FString& _WeaponName)
{
	DFLOG_N(Display);
	DFCHECK(_WeaponName != FString());

	//������ �̸��� �������ش�
	//���� ó�� �����ϴ°Ŷ�� ��������Ʈ�� �������ش�
	if (weaponName == FString()) {
		UDC_Weapon* weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
		weaponComp->onMagazineChange.AddDynamic(this, &UDWP_WeaponSlot::SetMagazine);
		weaponComp->onWeaponChange.AddDynamic(this, &UDWP_WeaponSlot::SetWeaponState);
		btn_Item->OnClicked.AddDynamic(this, &UDWP_WeaponSlot::OnCliecked);
	}

	weaponName = _WeaponName;
}

void UDWP_WeaponSlot::NativeDestruct()
{
	Super::NativeDestruct();

	if (weaponName == FString())return;

	//�����ߴ� ��������Ʈ�� �������ش�
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onMagazineChange.RemoveDynamic(this, &UDWP_WeaponSlot::SetMagazine);
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onWeaponChange.RemoveDynamic(this, &UDWP_WeaponSlot::SetWeaponState);
	btn_Item->OnClicked.RemoveDynamic(this, &UDWP_WeaponSlot::OnCliecked);
}

void UDWP_WeaponSlot::OnCliecked()
{
	DFLOG_N(Display);
	//���Ⱑ �������� �ƴ϶�� Ŭ���ص� �������� �ʴ´�
	if (currentWeaponOwnedState == EWEAPON_OWNED_STATE::NONE_OWNED)return;
	onButtonClicked.Broadcast(weaponName, EITEM_TYPE::WEWAPON);
}

void UDWP_WeaponSlot::SetMagazine(const FWeaponOwnedData& _WeaponData)
{
	//ȣ��� ��������Ʈ�� �̸��� ���� ����� ���ٸ� ǥ�õǴ� źâ ������ �����Ѵ�
	if (_WeaponData.weaponData.weaponName != weaponName)return;
	
	txt_Magazine->SetText(SetMagazineText(_WeaponData));
}

void UDWP_WeaponSlot::SetWeaponState(const FString& _WeaponName, EWEAPON_OWNED_STATE _WeaponOwnedState)
{
	//ȣ��� ��������Ʈ�� �̸��� ���� ����� ���ٸ� �������¸� �����Ѵ�
	if (_WeaponName != weaponName)return;

	SetWeaponOwnedState(_WeaponOwnedState);
}
