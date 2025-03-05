#include "Widget/Select/DWP_StartCharacter.h"
#include "Widget/Select/DWP_CharacterSelectSlot.h"
#include "Actor/Player/Controller/DAPC_Select.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"

UDWP_StartCharacter::UDWP_StartCharacter(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
	selectedIndex = -1;
}

void UDWP_StartCharacter::InitializeWidget()
{
	Super::InitializeWidget();

	//�������� ĳ���� ���� ������ ������ �迭�� ����ϴ�
	TArray<UWidget*> allWidgets{};
	WidgetTree->GetAllWidgets(allWidgets);

	for (auto widget : allWidgets) {
		auto characterSlot = Cast<UDWP_CharacterSelectSlot>(widget);
		if (characterSlot != nullptr)
			characterSlots.Add(characterSlot);
	}
}

void UDWP_StartCharacter::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<FDFPlayerData*> playerData{};

	//������ ���̺��� ĳ������ ������ ������ ĳ���� ���� ���Կ� �����մϴ�
	characterDataTable->GetAllRows<FDFPlayerData>(TEXT("GetAllRows"), playerData);

	for (int32 i = 0; i < characterSlots.Num(); ++i) {
		characterSlots[i]->SetCharacterIndex(i);
		characterSlots[i]->SetCharacterImage(playerData[i]->characterIcon.LoadSynchronous());
		if (selectedIndex == i)
			characterSlots[i]->SetCheckState(true);
		characterSlots[i]->onChecked.AddDynamic(this, &UDWP_StartCharacter::OnChecked);
	}

	ownerPlayerController = Cast<ADAPC_Select>(GetOwnerPlayerController());
	DFCHECK(ownerPlayerController != nullptr);

	btn_Check->OnClicked.AddDynamic(this, &UDWP_StartCharacter::OnClickedCheck);
	btn_Back->OnClicked.AddDynamic(ownerPlayerController, &ADAPC_Select::ShowMainWidget);
}

void UDWP_StartCharacter::NativeDestruct()
{
	Super::NativeDestruct();

	for (auto characterSlot : characterSlots)
		characterSlot->onChecked.RemoveDynamic(this, &UDWP_StartCharacter::OnChecked);

	btn_Check->OnClicked.RemoveDynamic(this, &UDWP_StartCharacter::OnClickedCheck);
	btn_Back->OnClicked.RemoveDynamic(ownerPlayerController, &ADAPC_Select::ShowMainWidget);
}

void UDWP_StartCharacter::OnClickedCheck()
{
	FString playerName = etb_ID->GetText().ToString();
	
	//ĳ���͸� üũ�ϰ� �г����� �����ٸ� �����͸� �����ϰ� ��ų���� ȭ������ �Ѿ�ϴ�
	if (playerName.IsEmpty())
		ownerPlayerController->ShowPopupBoxWidget(FText::FromString(TEXT("Please set the player name")));

	else if (0 > selectedIndex || selectedIndex >= characterSlots.Num())
		ownerPlayerController->ShowPopupBoxWidget(FText::FromString(TEXT("Please set the character you want to use")));

	else {
		ownerPlayerController->SetPlayerName(playerName);
		ownerPlayerController->SetCharacterIndex(selectedIndex);
		ownerPlayerController->ShowSkillSelectWidget();
	}
}

void UDWP_StartCharacter::OnChecked(UDWP_CharacterSelectSlot* _CheckedSlot)
{
	selectedIndex = _CheckedSlot->GetCharacterIndex();
	for (auto characterSlot : characterSlots)
		if (selectedIndex != characterSlot->GetCharacterIndex())
			characterSlot->SetCheckState(false);
}
