#pragma once

#include "AI/Service/DB_Service.h"
#include "DBS_FindTarget.generated.h"

//�÷��̾��� Ÿ���� �������� �����ϴ� ����
UCLASS()
class DEFENDER_API UDBS_FindTarget : public UDB_Service
{
	GENERATED_BODY()
public:

protected:

private:

public:
	UDBS_FindTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds)override;

private:

};
