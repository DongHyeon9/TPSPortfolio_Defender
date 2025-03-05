#include "Widget/Select/DWP_PopupBox.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

void UDWP_PopupBox::SetDescriptionText(const FText& _NewText)
{
	txt_Description->SetText(_NewText);
}

void UDWP_PopupBox::NativeConstruct()
{
	Super::NativeConstruct();

	btn_Check->OnClicked.AddDynamic(this, &UDWP_PopupBox::OnClickedCheckButton);
}

void UDWP_PopupBox::NativeDestruct()
{
	Super::NativeDestruct();

	btn_Check->OnClicked.RemoveDynamic(this, &UDWP_PopupBox::OnClickedCheckButton);
}

void UDWP_PopupBox::OnClickedCheckButton()
{
	RemoveInViewport();
}
