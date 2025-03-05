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
	//���ĵ� �ڽ��� �迭�� �߰�
	_Childs.Append(arrangedChilds);

	//��������� Ž��
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

	//������ ù��° �θ� �˻�
	while (parent.IsValid() &&
		parent->GetCachedGeometry().Size.SizeSquared() == 0)
		parent = parent->GetParentWidget();
	

	//��� �ڽ��� ũ�⸦ ���
	FArrangedWidget parentArrangedWidget(parent.ToSharedRef(), parent->GetCachedGeometry());

	ArrangeChildrenRecursive(childs, parentArrangedWidget);

	//���ϴ� ���� �˻�
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

