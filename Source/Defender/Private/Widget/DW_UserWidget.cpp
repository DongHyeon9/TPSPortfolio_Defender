#include "Widget/DW_UserWidget.h"
#include "Actor/DA_Character.h"

UDW_UserWidget::UDW_UserWidget(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{

}

void UDW_UserWidget::SetOwnerCharacter(ADA_Character* _OwnerCharacter)
{
	ownerCharacter = _OwnerCharacter;
}

void UDW_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DFLOG_N(Display);
}

void UDW_UserWidget::NativeDestruct()
{
	Super::NativeDestruct();
	DFLOG_N(Display);
}

void UDW_UserWidget::ArrangeChildrenRecursive(FArrangedChildren& _Childs, FArrangedWidget& _ParentWidget)
{
	FArrangedChildren arrangedChilds(EVisibility::All);

	_ParentWidget.Widget->ArrangeChildren(_ParentWidget.Geometry, arrangedChilds);
	//정렬된 자식을 배열에 추가
	_Childs.Append(arrangedChilds);

	//재귀적으로 탐색
	for (auto& arrangedChild : arrangedChilds.GetInternalArray())
		ArrangeChildrenRecursive(_Childs, arrangedChild);
	
}

void UDW_UserWidget::InitializeWidget()
{
	DFLOG_N(Display);
}

bool UDW_UserWidget::ShowInViewport(int32 _Order)
{
	DFLOG_N(Display);
	if (IsInViewport())return false;
	AddToViewport(_Order);
	return true;
}

bool UDW_UserWidget::RemoveInViewport()
{
	DFLOG_N(Display);
	if (!IsInViewport())return false;
	RemoveFromParent();
	return true;
}

FVector2D UDW_UserWidget::GetWidgetSize()
{
	FVector2D desiredWidgetSize;
	FArrangedChildren childs(EVisibility::All);
	auto thisWidget = TakeWidget();
	auto parent = GetCachedWidget()->GetParentWidget();

	//유요한 첫번째 부모를 검색
	while (parent.IsValid() &&
		parent->GetCachedGeometry().Size.SizeSquared() == 0)
		parent = parent->GetParentWidget();
	

	//모든 자식의 크기를 계산
	FArrangedWidget parentArrangedWidget(parent.ToSharedRef(), parent->GetCachedGeometry());

	ArrangeChildrenRecursive(childs, parentArrangedWidget);

	//원하는 위젯 검색
	for (auto& child : childs.GetInternalArray())
	{
		if (child.Widget == thisWidget)
		{
			desiredWidgetSize = child.Geometry.Size;
			break;
		}
	}
	return desiredWidgetSize;
}

