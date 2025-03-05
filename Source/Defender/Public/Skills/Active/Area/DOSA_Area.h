#pragma once

#include "Skills/Active/DO_ActiveBase.h"
#include "DOSA_Area.generated.h"

class AActor;

//특정지역에 발동하는 스킬의 최상위 클래스
UCLASS()
class DEFENDER_API UDOSA_Area : public UDO_ActiveBase
{
	GENERATED_BODY()

public:

protected:

private:
	//Type used for tracing
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Area Common Setting", meta = (AllowPrivateAccess = true))
	ESKILL_TRACE_TYPE traceType;

	//Starting position for tracing
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Area Common Setting", meta = (AllowPrivateAccess = true))
	FVector startLocation;

	//End position for tracing
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Area Common Setting", meta = (AllowPrivateAccess = true))
	FVector endLocation;

	//Collision type to trace
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Area Common Setting", meta = (AllowPrivateAccess = true))
	TArray<TEnumAsByte<EObjectTypeQuery>> objectType;

	//Actors to ignore
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Area Common Setting", meta = (AllowPrivateAccess = true))
	TArray<AActor*> ignoreActors;

	//Tags that will not be applied to skills
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Area Common Setting", meta = (AllowPrivateAccess = true))
	TArray<FName> ignoreTags;

	//Radius to be used for circle or capsule tracing
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Sphere And Capsule Setting", meta = (AllowPrivateAccess = true))
	float radius;

	//Half height to be used for capsule tracing
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Capsule Setting", meta = (AllowPrivateAccess = true))
	float halfHeight;

	//Half size to be used for box tracing
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Box Setting", meta = (AllowPrivateAccess = true))
	FVector halfSize;

	//Rotation to be used for box tracing
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Box Setting", meta = (AllowPrivateAccess = true))
	FRotator orientation;

public:
	UDOSA_Area();

	virtual void UseSkill_Implementation()override;
	/*
	* A function called if there is a result after tracing.
	* Implementation is done in a subclass.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ActiveSkill", meta = (HideSelfPin = true))
	void AreaCheckResults(const TArray<FHitResult>& _HitResults);
	virtual void AreaCheckResults_Implementation(const TArray<FHitResult>& _HitResults);

protected:

private:

};
