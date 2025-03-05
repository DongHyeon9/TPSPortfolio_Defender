#include "Widget/InGame/Shop/DWP_Shop.h"
#include "Widget/InGame/Shop/DWP_ItemSlot.h"
#include "Widget/InGame/Shop/DWP_BuySellButton.h"
#include "Widget/InGame/Shop/DWP_Count.h"
#include "Widget/InGame/Information/DWP_PotionSlot.h"
#include "Widget/InGame/Information/DWP_WeaponSlot.h"
#include "Actor/Player/Controller/DAPC_InGame.h"
#include "Actor/Player/DA_Player.h"
#include "Component/Player/DC_Inventory.h"
#include "Component/Player/DC_Weapon.h"
#include "Actor/Player/DA_PlayerState.h"
#include "Server/DA_GameInstance.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

void UDWP_Shop::InitializeWidget()
{
	Super::InitializeWidget();

	//��� ������ �����ͼ� �� ������ �迭�� �־��ش�
	TArray<UWidget*> allWidgets{};
	WidgetTree->GetAllWidgets(allWidgets);

	for (auto widget : allWidgets) {
		auto weaponSlot = Cast<UDWP_WeaponSlot>(widget);
		if (weaponSlot != nullptr) {
			weaponSlots.Add(weaponSlot);
			continue;
		}

		auto itemSlot = Cast<UDWP_ItemSlot>(widget);
		if (itemSlot != nullptr) {
			itemSlots.Add(itemSlot);
			continue;
		}

		auto potionSlot = Cast<UDWP_PotionSlot>(widget);
		if (potionSlot != nullptr) {
			potionSlots.Add(potionSlot);
			continue;
		}
	}

	itemDataTable->GetAllRows<FDFItem>(TEXT("GetAllRows"), itemDataArray);
}

void UDWP_Shop::SetTimer(float _Timer)
{
	if (_Timer <= 0.f)
		txt_Timer->SetText(FText());

	else {
		timeRemaining = _Timer;
		txt_Timer->SetText(FText::AsTimespan(FTimespan::FromSeconds(--timeRemaining)));

		auto gameInstance = GetGameInstance<UDA_GameInstance>();
		DFCHECK(gameInstance != nullptr);
		gameInstance->RegistTimerHandle(&shopTimerHandle);

		GetWorld()->GetTimerManager().SetTimer(shopTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				txt_Timer->SetText(FText::AsTimespan(FTimespan::FromSeconds(--timeRemaining)));
			}), 1.f, true);
	}
}

void UDWP_Shop::NativeConstruct()
{
	Super::NativeConstruct();

	//�ʿ��� ������ �����Ѵ�
	//�Լ� ȣ������ ���̱� ����
	//�÷��̾��� ���� ������Ʈ�� �κ��丮 ������Ʈ�� ������ �����Ѵ�
	buySellButtonWidget = CreateWidget<UDWP_BuySellButton>(GetOwnerPlayerController(), buySellButtonClass);
	countWidget = CreateWidget<UDWP_Count>(GetOwnerPlayerController(), countClass);

	buySellButtonWidget->ShowInViewport(VO_BUY_SELL);
	buySellButtonWidget->SetVisibility(ESlateVisibility::Hidden);

	weaponComp = GetOwnerPlayerCharacter()->GetWeaponComponent();
	inventoryComp = GetOwnerPlayerCharacter()->GetInventoryComponent();

	//�÷��̾��� ���� ������Ʈ���� ���� �����͵��� �����´�
	//�⺻����� �� �� ������ �迭���� �����Ѵ�
	TArray<FWeaponOwnedData> ownedData = weaponComp->GetAllWeaponData();
	ownedData.RemoveAt(0);

	//�� ���Ե��� �����Ϳ� �µ��� �ʱ�ȭ���ش�
	for (int32 i = 0; i < weaponSlots.Num(); ++i) {
		weaponSlots[i]->InitializeWeaponSlot(ownedData[i]);
		weaponSlots[i]->onButtonClicked.AddDynamic(this, &UDWP_Shop::OnClicked);
	}

	SetPotionSlot();

	for (int32 i = 0; i < ownedData.Num(); ++i) {
		FItemSlotInitilizer init{};
		init.weaponData = &ownedData[i];
		itemSlots[i]->InitializeItemSlot(init);
		itemSlots[i]->onButtonClicked.AddDynamic(this, &UDWP_Shop::OnClicked);
	}

	for (int32 i = ownedData.Num(); i < itemSlots.Num(); ++i) {
		int32 itemIndex = i - ownedData.Num();
		FItemSlotInitilizer init{};
		init.potionData = itemDataArray[itemIndex];
		init.inPadding = FMargin(0.f, 0.f, 120.f, 0.f);
		itemSlots[i]->InitializeItemSlot(init);
		itemSlots[i]->onButtonClicked.AddDynamic(this, &UDWP_Shop::OnClicked);
	}

	//������ �ʱ� ���� �������ش�
	inventoryComp->onGoldChange.AddDynamic(this, &UDWP_Shop::SetGold);
	inventoryComp->onItemCountChange.AddDynamic(this, &UDWP_Shop::ResetPotionSlots);
	GetOwnerPlayerState()->onNameChange.AddDynamic(this, &UDWP_Shop::SetPlayerName);
	buySellButtonWidget->onBuySellMode.AddDynamic(this, &UDWP_Shop::OnClickedTradeButton);
	countWidget->onConfirmed.AddDynamic(this, &UDWP_Shop::Trade);
	btn_Close->OnClicked.AddDynamic(this, &UDWP_Shop::OnClickedClose);

	txt_Info_PlayerName->SetText(FText::FromString(GetOwnerPlayerState()->GetPlayerName()));
	txt_GoldHas->SetText(SetGoldText(inventoryComp->GetCurrentGold()));
}

void UDWP_Shop::NativeDestruct()
{
	Super::NativeDestruct();

	//������ ������ �� �����ߴ� ��������Ʈ���� �����ش�
	inventoryComp->onGoldChange.RemoveDynamic(this, &UDWP_Shop::SetGold);
	inventoryComp->onItemCountChange.RemoveDynamic(this, &UDWP_Shop::ResetPotionSlots);
	GetOwnerPlayerState()->onNameChange.RemoveDynamic(this, &UDWP_Shop::SetPlayerName);
	buySellButtonWidget->onBuySellMode.RemoveDynamic(this, &UDWP_Shop::OnClickedTradeButton);
	countWidget->onConfirmed.RemoveDynamic(this, &UDWP_Shop::Trade);
	btn_Close->OnClicked.RemoveDynamic(this, &UDWP_Shop::OnClickedClose);

	for (auto weaponSlot : weaponSlots)
		weaponSlot->onButtonClicked.RemoveDynamic(this, &UDWP_Shop::OnClicked);

	for (auto itemSlot : itemSlots)
		itemSlot->onButtonClicked.RemoveDynamic(this, &UDWP_Shop::OnClicked);

	buySellButtonWidget->RemoveInViewport();
	countWidget->RemoveInViewport();
}

void UDWP_Shop::OnClicked(const FString& _ItemName, EITEM_TYPE _ItemType)
{
	DFLOG_N(Display);

	buySellButtonWidget->SetVisibility(ESlateVisibility::Visible);

	//������ Ŭ���ϸ� ������ �����ϰ� Ŭ���� ��ġ�� ������ �ű��
	selectedItemName = _ItemName;
	selectedItemType = _ItemType;
	
	float xPos{}, yPos{};
	
	float viewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), xPos, yPos);

	FVector2D widgetSize = buySellButtonWidget->GetWidgetSize();

	xPos -= widgetSize.X * 0.5;
	yPos -= widgetSize.Y * 0.5;

	buySellButtonWidget->SetPositionInViewport(FVector2D(xPos, yPos) * viewportScale);
	if (widgetSize.Size() > 1000.f)
		OnClicked(_ItemName, _ItemType);
}

void UDWP_Shop::OnClickedTradeButton(bool _bIsBuy)
{
	DFLOG_N(Display);
	buySellButtonWidget->SetVisibility(ESlateVisibility::Hidden);

	int32 itemPrice{};
	int32 playerGold = inventoryComp->GetCurrentGold();

	//���õ� ������ ���� �������� ������ �����Ѵ�
	switch (selectedItemType)
	{
	case EITEM_TYPE::WEWAPON: {
		TMap<FString, FWeaponOwnedData> weaponMap = weaponComp->GetWeaponMap();
		if (_bIsBuy) {
			if (weaponMap[selectedItemName].ownedState == EWEAPON_OWNED_STATE::NONE_OWNED){
				if (playerGold >= weaponMap[selectedItemName].weaponData.weaponPrice) {
					inventoryComp->AddGold(-weaponMap[selectedItemName].weaponData.weaponPrice);
					weaponComp->AddWeapon(selectedItemName);
					return;
				}
				else return;
			}
			itemPrice = weaponMap[selectedItemName].weaponData.magazinePrice;
		}
		else {
			if (weaponMap[selectedItemName].ownedState == EWEAPON_OWNED_STATE::NONE_OWNED) return;
			else if (weaponMap[selectedItemName].ownedState != EWEAPON_OWNED_STATE::NONE_OWNED &&
				weaponMap[selectedItemName].total == 0 &&
				weaponMap[selectedItemName].current == 0) {
				inventoryComp->AddGold(weaponMap[selectedItemName].weaponData.weaponPrice * 0.5);
				weaponComp->DropWeapon(selectedItemName);
				return;
			}
			itemPrice = weaponMap[selectedItemName].weaponData.magazinePrice * 0.5;
			playerGold = itemPrice * (weaponMap[selectedItemName].total + weaponMap[selectedItemName].current);
		}
		break;
	}
	case EITEM_TYPE::USE: {
		auto itemData = itemDataTable->FindRow<FDFItem>(*selectedItemName, TEXT(""));
		itemPrice = itemData->itemPrice;
		break;
	}
	}
	
	//������ ����ְ� �����͵��� �������ش�
	DFCHECK(countWidget->ShowInViewport(VO_BUY_SELL_COUNT));
	countWidget->SetPlayerGold(playerGold);
	countWidget->SetItemPrice(itemPrice);
	countWidget->SetBuy(_bIsBuy);

	//������ ȭ�� �߾ӿ� ����ش�
	FVector2D res{};
	GEngine->GameViewport->GetViewportSize(res);
	countWidget->SetPositionInViewport(res * 0.5);
}

void UDWP_Shop::Trade(int32 _Count, int32 _TotalPrice)
{
	//��ȯ�� ������ �������� ������ ��带 �����Ѵ�
	switch (selectedItemType)
	{
	case EITEM_TYPE::WEWAPON: {
		weaponComp->AddMagazine(selectedItemName, _Count);
		break;
	}
	case EITEM_TYPE::USE: {
		inventoryComp->AddItem(selectedItemName, _Count);
		break;
	}
	}

	inventoryComp->AddGold(-_TotalPrice);
}

void UDWP_Shop::OnClickedClose()
{
	if (shopTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(shopTimerHandle);
		shopTimerHandle.Invalidate();
	}

	auto player = Cast<ADAPC_InGame>(GetOwnerPlayerController());
	DFCHECK(player != nullptr);
	player->ShowMainWidget();
	onClose.Broadcast();
	DFCHECK(RemoveInViewport());
}

void UDWP_Shop::SetGold(int32 _Gold)
{
	txt_GoldHas->SetText(SetGoldText(_Gold));
}

void UDWP_Shop::SetPotionSlot()
{
	//���� �����͸� �������ش�
	const TMap<FString, int32>& itemMap = inventoryComp->GetItemMap();

	for (int32 i{}, j{}; i < potionSlots.Num() && j < potionSlots.Num(); ++i) {
		FString potionName = itemDataArray[i]->itemName;
		if (!itemMap.Contains(potionName)) continue;
		FPotionSlotInitializer init{};
		init.itemData = itemDataArray[i];
		init.currentCount = itemMap[potionName];
		potionSlots[j]->InitializePotionSlot(init);
		++j;
	}
}

void UDWP_Shop::ResetPotionSlots(const FString& _PotionName, int32 _Count)
{
	SetPotionSlot();
}

void UDWP_Shop::SetPlayerName(const FString& _Name)
{
	txt_Info_PlayerName->SetText(FText::FromString(_Name));
}
