#pragma once

#include "../Defender.h"
#include "Blueprint/UserWidget.h"
#include "DW_UserWidget.generated.h"

class ADA_Character;

//프로젝트에서 사용할 위젯의 최상이 클래스
UCLASS()
class DEFENDER_API UDW_UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

protected:

private:
	TWeakObjectPtr<ADA_Character>			ownerCharacter;			//UI에 정보를 표시할 캐릭터

public:
	UDW_UserWidget(const FObjectInitializer& _ObjectInitializer);

	/*
	* Set the character to be displayed in the UI
	* @param[in] _OwnerCharacter Character for whom information will be displayed
	*/
	UFUNCTION(BlueprintCallable, category = "Defender Widget")
	void SetOwnerCharacter(ADA_Character* _OwnerCharacter);
	/*
	* Get the character that is being displayed in the UI
	* @return Return ADA_Character*
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "Defender Widget")
	ADA_Character* GetOwnerCharacter()const { return ownerCharacter.Get(); }
	/*
	* Initialize widgets before adding them to the viewport.
	*/
	virtual void InitializeWidget();
	/*
	* Add a widget to the viewport. Returns false if the widget is already added to the viewport.
	* @param[in] _Order Viewport order for widgets
	* @return Successful addition of widget
	*/
	virtual bool ShowInViewport(int32 _Order);
	/*
	* Removes a widget from the viewport. Returns failure if the widget is not added to the viewport.
	* @return Success or failure of widget deletion
	*/
	virtual bool RemoveInViewport();
	/*
	* Gets the size of the current widget
	* @return return the FVector2D
	*/
	FVector2D GetWidgetSize();

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:


	void ArrangeChildrenRecursive(FArrangedChildren& _Childs, FArrangedWidget& _ParentWidget);
};
