#include "Widget/InGame/Monster/DWP_FloatingDamage.h"

#include "Components/TextBlock.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UDWP_FloatingDamage::UDWP_FloatingDamage(const FObjectInitializer& _ObjectInitializer):
	Super(_ObjectInitializer)
{
	xDistancePer = 0.05f;
	yDistancePerMin = 0.4f;
	yDistancePerMax = 0.2;
	interpSpeed = 1.f;
}

void UDWP_FloatingDamage::NativeConstruct()
{
	Super::NativeConstruct();

	SetPositionInViewport(startLocation);

	//뷰포트사이즈에 비례해서 일정거리만큼 움직이는 애니메이션을 재생한다
	FVector2D viewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

	float targetX = FMath::RandRange(viewportSize.X * -xDistancePer, viewportSize.X * xDistancePer);
	float targetY = FMath::RandRange(viewportSize.Y * -yDistancePerMin, viewportSize.Y * -yDistancePerMax);
	endLocation.X = startLocation.X + targetX;
	endLocation.Y = startLocation.Y + targetY;

	PlayAnimation(widgetAnimation);
}

void UDWP_FloatingDamage::NativeDestruct()
{
	Super::NativeDestruct();

	widgetAnimationEvent.Unbind();
}

void UDWP_FloatingDamage::NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime)
{
	Super::NativeTick(_MyGeometry, _InDeltaTime);

	startLocation = UKismetMathLibrary::Vector2DInterpTo(startLocation, endLocation, _InDeltaTime, interpSpeed);

	SetPositionInViewport(startLocation);
}

void UDWP_FloatingDamage::InitializeFloatingDamage(FVector2D& _Position, float _Damage)
{
	txt_Damage->SetText(FText::AsNumber((int32)_Damage));
	startLocation = _Position;

	widgetAnimationEvent.BindDynamic(this, &UDWP_FloatingDamage::AnimationFinished);
	BindToAnimationFinished(widgetAnimation, widgetAnimationEvent);
}

void UDWP_FloatingDamage::AnimationFinished()
{
	RemoveInViewport();
}
