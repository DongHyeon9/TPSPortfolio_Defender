#pragma once

#include "../Defender.h"
#include "GameFramework/Actor.h"
#include "DA_Actor.generated.h"

//프로젝트에서 사용하는 액터의 최상위 클래스
UCLASS()
class DEFENDER_API ADA_Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	ADA_Actor();

protected:
	virtual void BeginPlay() override;

public:	

};
