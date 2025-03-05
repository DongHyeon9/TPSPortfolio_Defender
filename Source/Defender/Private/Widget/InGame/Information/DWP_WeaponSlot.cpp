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

	//무기의 상태를 위젯에 셋팅해 준다
	weaponName = _Initializer.weaponData.weaponName;
	img_Weapon->SetBrushFromTexture(_Initializer.weaponData.weaponImage);
	txt_WeaponName->SetText(FText::FromString(weaponName));
	txt_Magazine->SetText(SetMagazineText(_Initializer));
	SetWeaponOwnedState(_Initializer.ownedState);

	//탄창이 바뀔때와 무기가 바뀔때 호출되는 델리게이트에 연결해준다
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onMagazineChange.AddDynamic(this, &UDWP_WeaponSlot::SetMagazine);
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onWeaponChange.AddDynamic(this, &UDWP_WeaponSlot::SetWeaponState);
	btn_Item->OnClicked.AddDynamic(this, &UDWP_WeaponSlot::OnCliecked);
}

void UDWP_WeaponSlot::SetWeaponOwnedState(EWEAPON_OWNED_STATE _WeaponOwnedState)
{
	//현재 무기의 보유 상태를 체크하고
	currentWeaponOwnedState = _WeaponOwnedState;

	//보유 상태에 따라 표시하는 색상을 변경
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

	//무기의 이름을 셋팅해준다
	//만약 처음 셋팅하는거라면 델리게이트도 연결해준다
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

	//연결했던 델리게이트를 삭제해준다
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onMagazineChange.RemoveDynamic(this, &UDWP_WeaponSlot::SetMagazine);
	GetOwnerPlayerCharacter()->GetWeaponComponent()->onWeaponChange.RemoveDynamic(this, &UDWP_WeaponSlot::SetWeaponState);
	btn_Item->OnClicked.RemoveDynamic(this, &UDWP_WeaponSlot::OnCliecked);
}

void UDWP_WeaponSlot::OnCliecked()
{
	DFLOG_N(Display);
	//무기가 소유중이 아니라면 클릭해도 반응하지 않는다
	if (currentWeaponOwnedState == EWEAPON_OWNED_STATE::NONE_OWNED)return;
	onButtonClicked.Broadcast(weaponName, EITEM_TYPE::WEWAPON);
}

void UDWP_WeaponSlot::SetMagazine(const FWeaponOwnedData& _WeaponData)
{
	//호출된 델리게이트의 이름이 현재 무기와 같다면 표시되는 탄창 갯수를 변경한다
	if (_WeaponData.weaponData.weaponName != weaponName)return;
	
	txt_Magazine->SetText(SetMagazineText(_WeaponData));
}

void UDWP_WeaponSlot::SetWeaponState(const FString& _WeaponName, EWEAPON_OWNED_STATE _WeaponOwnedState)
{
	//호출된 델리게이트의 이름이 현재 무기와 같다면 보유상태를 변경한다
	if (_WeaponName != weaponName)return;

	SetWeaponOwnedState(_WeaponOwnedState);
}
