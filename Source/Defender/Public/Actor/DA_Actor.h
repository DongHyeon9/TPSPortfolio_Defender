#pragma once

#include "../Defender.h"
#include "GameFramework/Actor.h"
#include "DA_Actor.generated.h"

//������Ʈ���� ����ϴ� ������ �ֻ��� Ŭ����
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
