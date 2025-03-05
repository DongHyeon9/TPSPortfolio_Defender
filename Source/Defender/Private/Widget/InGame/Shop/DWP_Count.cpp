#include "Widget/InGame/Shop/DWP_Count.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UDWP_Count::SetPlayerGold(int32 _TotalGold)
{
	playerGold = _TotalGold;
}

void UDWP_Count::SetItemPrice(int32 _ItemPrice)
{
	itemPrice = _ItemPrice;
}

void UDWP_Count::SetBuy(bool _bIsBuy)
{
	bIsBuy = _bIsBuy;
}

void UDWP_Count::NativeConstruct()
{
	Super::NativeConstruct();
	
	etb_Count->SetText(FText::FromString(FString()));
	txt_TotalGold->SetText(SetGoldText(0));

	etb_Count->OnTextCommitted.AddDynamic(this, &UDWP_Count::OnCommitText);
	btn_Minus->OnClicked.AddDynamic(this, &UDWP_Count::OnClickedMinus);
	btn_Plus->OnClicked.AddDynamic(this, &UDWP_Count::OnClickedPlus);
	btn_Cancel->OnClicked.AddDynamic(this, &UDWP_Count::OnClickedCancel);
	btn_OK->OnClicked.AddDynamic(this, &UDWP_Count::OnClickedOK);

	etb_Count->SetKeyboardFocus();
}

void UDWP_Count::NativeDestruct()
{
	Super::NativeDestruct();

	etb_Count->OnTextCommitted.RemoveDynamic(this, &UDWP_Count::OnCommitText);
	btn_Minus->OnClicked.RemoveDynamic(this, &UDWP_Count::OnClickedMinus);
	btn_Plus->OnClicked.RemoveDynamic(this, &UDWP_Count::OnClickedPlus);
	btn_Cancel->OnClicked.RemoveDynamic(this, &UDWP_Count::OnClickedCancel);
	btn_OK->OnClicked.RemoveDynamic(this, &UDWP_Count::OnClickedOK);
}

void UDWP_Count::OnCommitText(const FText& _Text, ETextCommit::Type _CommitMethod)
{
	currentCount = FCString::Atoi(*_Text.ToString());
	CalculateTextBox();
}

void UDWP_Count::OnClickedMinus()
{
	--currentCount;
	CalculateTextBox();
}

void UDWP_Count::OnClickedPlus()
{
	++currentCount;
	CalculateTextBox();
}

void UDWP_Count::OnClickedOK()
{
	//�Ǹ��� ��� ������ ������ �����Ѵ�
	if (!bIsBuy)
		currentCount *= -1;

	onConfirmed.Broadcast(currentCount, itemPrice * currentCount);
	DFCHECK(RemoveInViewport());
}

void UDWP_Count::OnClickedCancel()
{
	DFCHECK(RemoveInViewport());
}

void UDWP_Count::CalculateTextBox()
{
	//������ ������ �÷��̾��� ���� ��庸�� ���ٸ� ������ ���δ�
	if (currentCount * itemPrice > playerGold)
		currentCount = playerGold / itemPrice;

	//���� ī��Ʈ�� ������� 0���� �����
	if (currentCount < 0)
		currentCount = 0;

	//�Ǹ����� ���������� ���� �� �ݾ��� �ٸ��� �����Ѵ�
	bIsBuy ?
		txt_TotalGold->SetText(SetGoldText(currentCount * itemPrice)) :
		txt_TotalGold->SetText(SetGoldText(currentCount * itemPrice / 2));

	etb_Count->SetText(FText::FromString(FString::FromInt(currentCount)));
}
