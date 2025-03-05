#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_FloatingDamage.generated.h"

class UTextBlock;
class UWidgetAnimation;

//플레이어가 공격한 데미지가 몇인지 표시해주는 위젯(위젯 애니메이션 포함)
UCLASS()
class DEFENDER_API UDWP_FloatingDamage : public UDW_PlayerWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent		widgetAnimationEvent;		//이벤트를 바인딩할 애니메이션이벤트 구조체

protected:
	//Animation of damage text
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Floating Damage", meta = (BindWidgetAnim))
	UWidgetAnimation* widgetAnimation;

private:
	//Text widget to display damage
	UPROPERTY(BlueprintReadOnly, category = "Floating Damage", meta = (BindWidget, AllowPrivateAccess = true))
	UTextBlock* txt_Damage;

	//Ratio of moving left and right(based on screen size)
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,category="Floating Damage Setting",meta=(AllowPrivateAccess=true))
	float xDistancePer;

	//Minimum rate of upward movement(based on screen size)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Floating Damage Setting", meta = (AllowPrivateAccess = true))
	float yDistancePerMin;

	//Maximum rate of upward movement(based on screen size)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Floating Damage Setting", meta = (AllowPrivateAccess = true))
	float yDistancePerMax;

	//speed of text movement
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Floating Damage Setting", meta = (AllowPrivateAccess = true))
	float interpSpeed;

	FVector2D		startLocation;		//표시되는 시작위치
	FVector2D		endLocation;		//표시되는 마지막 위치

public:
	UDWP_FloatingDamage(const FObjectInitializer& _ObjectInitializer);
	/*
	* Initialize the floating damage widget
	* @param[in] _Position starting position
	* @param[in] _Damage Damage to display
	*/
	void InitializeFloatingDamage(FVector2D& _Position, float _Damage);

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;
	virtual void NativeTick(const FGeometry& _MyGeometry, float _InDeltaTime)override;

private:
	UFUNCTION()
	void AnimationFinished();
};