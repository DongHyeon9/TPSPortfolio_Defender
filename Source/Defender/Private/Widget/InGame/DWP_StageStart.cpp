#include "Widget/InGame/DWP_StageStart.h"

void UDWP_StageStart::NativeConstruct()
{
	Super::NativeConstruct();

	widgetAnimationEvent.BindDynamic(this, &UDWP_StageStart::OnFadeEnd);
	BindToAnimationFinished(WidgetAnimation, widgetAnimationEvent);
	PlayAnimation(WidgetAnimation);
	
}

void UDWP_StageStart::OnFadeEnd()
{
	widgetAnimationEvent.Clear();
	RemoveInViewport();
}
