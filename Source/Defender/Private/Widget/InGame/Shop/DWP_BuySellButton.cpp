#include "Widget/InGame/Shop/DWP_BuySellButton.h"

#include "Components/Button.h"

void UDWP_BuySellButton::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Buy->OnClicked.AddDynamic(this, &UDWP_BuySellButton::OnClickedBuy);
	btn_Sell->OnClicked.AddDynamic(this, &UDWP_BuySellButton::OnClickedSell);
}

void UDWP_BuySellButton::NativeDestruct()
{
	Super::NativeDestruct();

	btn_Buy->OnClicked.RemoveDynamic(this, &UDWP_BuySellButton::OnClickedBuy);
	btn_Sell->OnClicked.RemoveDynamic(this, &UDWP_BuySellButton::OnClickedSell);
}

void UDWP_BuySellButton::OnClickedBuy()
{
	DFLOG_N(Display);
	onBuySellMode.Broadcast(true);
}

void UDWP_BuySellButton::OnClickedSell()
{
	DFLOG_N(Display);
	onBuySellMode.Broadcast(false);
}
