#pragma once

#include "../Defender.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "DBT_MonsterMoveTo.generated.h"

//AI가 특정위치 혹은 타겟에게 움직이게 하는 테스크
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
