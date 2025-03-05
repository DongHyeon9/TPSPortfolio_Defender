#pragma once

#include "AI/Task/DB_TaskNode.h"
#include "DBT_TurnToTarget.generated.h"

//AI�� ���ݽ� Ÿ���� �ٶ󺸵����ϴ� �׽�ũ
UCLASS()
class DEFENDER_API UDBT_TurnToTarget : public UDB_TaskNode
{
	GENERATED_BODY()
public:

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "TurnSpeed", meta = (AllowPrivateAccess = true))
	float turnSpeed;
public:
	UDBT_TurnToTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)override;

private:

};
