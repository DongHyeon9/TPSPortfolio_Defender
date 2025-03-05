#include "Widget/InGame/Information/DWP_PotionSlot.h"
#include "Component/Player/DC_Inventory.h"
#include "Actor/Player/DA_Player.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UDWP_PotionSlot::UDWP_PotionSlot(const FObjectInitializer& _ObjectInitializer) :
	Super(_ObjectInitializer)
{
	potionName = FString();
}

void UDWP_PotionSlot::InitializePotionSlot(const FPotionSlotInitializer& _Initializer)
{
	DFLOG_N(Display);
	DFCHECK(_Initializer.itemData->itemName != FString());

	if (potionName == FString()) {
		//�������� ����Ҷ� ȣ��Ǵ� ��������Ʈ�� �������ش�
		GetOwnerPlayerCharacter()->GetInventoryComponent()->onItemCountChange.AddDynamic(this, &UDWP_PotionSlot::SetItemCount);
		btn_Potion->OnClicked.AddDynamic(this, &UDWP_PotionSlot::OnCliecked);
		btn_Potion->OnUnhovered.AddDynamic(this, &UDWP_PotionSlot::OnUnHovered);
		btn_Potion->OnHovered.AddDynamic(this, &UDWP_PotionSlot::OnHovered);
	}

	//�������� �ʱⰪ�� �������ش�
	potionName = _Initializer.itemData->itemName;
	img_Potion->SetBrushFromTexture(_Initializer.itemData->iconImage);
	txt_Count->SetText(FText::AsNumber(_Initializer.currentCount));
	txt_Description->SetText(_Initializer.itemData->description);
}

void UDWP_PotionSlot::SetPotionName(const FString& _PotionName)
{
	DFLOG_N(Display);

	//ó�� �����ϴ°Ŷ�� ��������Ʈ�� �������ش�
	if (potionName == FString()) {
		GetOwnerPlayerCharacter()->GetInventoryComponent()->onItemCountChange.AddDynamic(this, &UDWP_PotionSlot::SetItemCount);
		btn_Potion->OnClicked.AddDynamic(this, &UDWP_PotionSlot::OnCliecked);
		btn_Potion->OnUnhovered.AddDynamic(this, &UDWP_PotionSlot::OnUnHovered);
		btn_Potion->OnHovered.AddDynamic(this, &UDWP_PotionSlot::OnHovered);
	}

	potionName = _PotionName;
}

void UDWP_PotionSlot::SetPotionImage(UTexture2D* _Image)
{
	DFLOG_N(Display);
	img_Potion->SetBrushFromTexture(_Image);
}

void UDWP_PotionSlot::SetPotionDescription(const FText& _Description)
{
	DFLOG_N(Display);
	txt_Description->SetText(_Description);
}

void UDWP_PotionSlot::NativeDestruct()
{
	Super::NativeDestruct();

	if (potionName == FString()) return;

	//����Ǿ��ִ� ��������Ʈ�� �����Ѵ�
	GetOwnerPlayerCharacter()->GetInventoryComponent()->onItemCountChange.RemoveDynamic(this, &UDWP_PotionSlot::SetItemCount);
	btn_Potion->OnClicked.RemoveDynamic(this, &UDWP_PotionSlot::OnCliecked);
	btn_Potion->OnUnhovered.RemoveDynamic(this, &UDWP_PotionSlot::OnUnHovered);
	btn_Potion->OnHovered.RemoveDynamic(this, &UDWP_PotionSlot::OnHovered);
}

void UDWP_PotionSlot::OnHovered()
{
	txt_Description->SetVisibility(ESlateVisibility::Visible);
}

void UDWP_PotionSlot::OnUnHovered()
{
	txt_Description->SetVisibility(ESlateVisibility::Hidden);
}

void UDWP_PotionSlot::OnCliecked()
{
	DFLOG_N(Display);
	onButtonClicked.Broadcast(potionName, EITEM_TYPE::USE);
}

void UDWP_PotionSlot::SetItemCount(const FString& _ItemName, int32 _TotalItemCount)
{
	//ȣ��� ��������Ʈ�� �̸��� ���ٸ� ���������� �����Ѵ�
	if (potionName != _ItemName)return;

	txt_Count->SetText(FText::AsNumber(_TotalItemCount));
}
