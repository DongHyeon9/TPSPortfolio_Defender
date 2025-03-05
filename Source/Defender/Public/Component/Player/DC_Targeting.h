#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Targeting.generated.h"

class ADA_Monster;
class UCameraComponent;

//플레이어의 Lock On 시스템을 활성화 해주는 컴포넌트
UCLASS(ClassGroup = (DC_Player), meta = (BlueprintSpawnableComponent))
class DEFENDER_API UDC_Targeting : public UDC_PawnComponent
{
	GENERATED_BODY()
public:

protected:

private:
	//The range of the circle to detect the target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Lock On System", meta = (AllowPrivateAccess = true))
	float targetTraceRadius;

	//Distance to detect target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Lock On System", meta = (AllowPrivateAccess = true))
	float targetDistance;

	//InterpSpeed ​​of owner character's control rotation when target is present
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Lock On System", meta = (AllowPrivateAccess = true))
	float lockOnInterpSpeed;

	//This is the target the owner character will look at
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Lock On System", meta = (AllowPrivateAccess = true))
	ADA_Monster* targetMonster;

	//If the owner is a player, it is based on the camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Lock On System", meta = (AllowPrivateAccess = true))
	UCameraComponent* playerCamera;

public:
	UDC_Targeting();
	/*
	* Set the target
	* Remove the target if it already exists
	*/
	void LockOn();
	/*
	* Changes the target to the closest actor to the right of the target
	*/
	void TargetMoveRight();
	/*
	* Changes the target to the closest actor to the left of the target
	*/
	void TargetMoveLeft();

protected:
	virtual void InitializeComponent()override;
	virtual void TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)override;

private:
	//내부 호출 함수

	bool TraceTarget(TArray<FHitResult>& _OutHitResults);
	float CalculateScreenYDistance(const AActor* _TargetActor);
	void SetTargetCharacter(ADA_Monster* _NewTarget);
	UFUNCTION()
	void TargetDead(ECHARACTER_STATE _CharacterState);
};
