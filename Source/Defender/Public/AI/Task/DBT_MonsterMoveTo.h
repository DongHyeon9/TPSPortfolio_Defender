#pragma once

#include "../Defender.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "DBT_MonsterMoveTo.generated.h"

//AI�� Ư����ġ Ȥ�� Ÿ�ٿ��� �����̰� �ϴ� �׽�ũ
UCLASS()
class DEFENDER_API UDBT_MonsterMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()
public:

protected:

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,category="TraceState to Set",meta=(AllowPrivateAccess=true))
	EMONSTER_TRACE_STATE traceState;
	

public:
	UDBT_MonsterMoveTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)override;

private:

};
