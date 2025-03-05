#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_StageStart.generated.h"

class UWidgetAnimation;

//���������� �����Ҷ� �ִϸ��̼ǰ� �Բ� ����� ����
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
	FWidgetAnimationDynamicEvent		widgetAnimationEvent;		//���� �ִϸ��̼�

public:

protected:
	virtual void NativeConstruct()override;

private:
	UFUNCTION()
	void OnFadeEnd();
};
