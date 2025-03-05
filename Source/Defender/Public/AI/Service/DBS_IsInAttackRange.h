#pragma once

#include "AI/Service/DB_Service.h"
#include "DBS_IsInAttackRange.generated.h"

//AI와 타겟의 거리가 공격가능한 거리인지 체크하는 서비스
UCLASS()
class DEFENDER_API UDBS_IsInAttackRange : public UDB_Service
{
	GENERATED_BODY()
public:

protected:

private:

public:
	UDBS_IsInAttackRange();

protected:
	virtual void TickNode(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds)override;

private:

};
