#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StageStart.generated.h"

class UWidgetAnimation;

//스테이지가 시작할때 애니메이션과 함께 재생될 위젯
UCLASS()
class DEFENDER_API UDWP_StageStart : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:

protected:
	//Fade animation that makes widgets appear gradually
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Fade Animation", meta = (BindWidgetAnim))
	UWidgetAnimation* WidgetAnimation;

private:
	FWidgetAnimationDynamicEvent		widgetAnimationEvent;		//위젯 애니메이션

public:

protected:
	virtual void NativeConstruct()override;

private:
	UFUNCTION()
	void OnFadeEnd();
};
