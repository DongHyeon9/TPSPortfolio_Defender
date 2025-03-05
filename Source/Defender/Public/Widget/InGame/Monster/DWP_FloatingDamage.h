#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_FloatingDamage.generated.h"

class UTextBlock;
class UWidgetAnimation;

//�÷��̾ ������ �������� ������ ǥ�����ִ� ����(���� �ִϸ��̼� ����)
UCLASS()
class DEFENDER_API UDWP_FloatingDamage : public UDW_PlayerWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent		widgetAnimationEvent;		//�̺�Ʈ�� ���ε��� �ִϸ��̼��̺�Ʈ ����ü

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

	FVector2D		startLocation;		//ǥ�õǴ� ������ġ
	FVector2D		endLocation;		//ǥ�õǴ� ������ ��ġ

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