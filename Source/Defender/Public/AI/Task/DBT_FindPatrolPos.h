#pragma once

#include "AI/Task/DB_TaskNode.h"
#include "DBT_FindPatrolPos.generated.h"

//AI�� ���� ���������� �����ϵ����ϴ� �׽�ũ
UCLASS()
class DEFENDER_API UDBT_FindPatrolPos : public UDB_TaskNode
{
	GENERATED_BODY()
public:

protected:

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,category="FindPatrolPos",meta=(AllowPrivateAccess=true))
	float patrolRadius;

public:
	UDBT_FindPatrolPos();
	 
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)override;

private:

};
