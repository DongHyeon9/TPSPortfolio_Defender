#pragma once

#include "AI/Service/DB_Service.h"
#include "DBS_SelectAttack.generated.h"

//AI�� �������� ������ų�� �������ϴ� ����
UCLASS()
class DEFENDER_API UDBS_SelectAttack : public UDB_Service
{
	GENERATED_BODY()
public:

protected:

private:

public:
	UDBS_SelectAttack();

protected:
	virtual void TickNode(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory, float _DeltaSeconds)override;

private:

};
