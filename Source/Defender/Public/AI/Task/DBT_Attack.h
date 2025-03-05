#pragma once

#include "AI/Task/DB_TaskNode.h"
#include "DBT_Attack.generated.h"

class UDC_Skill;
class ADA_AIController;

class UBehaviorTreeComponent;

//AI�� �����ϵ��� �ϴ� �׽�ũ
UCLASS()
class DEFENDER_API UDBT_Attack : public UDB_TaskNode
{
	GENERATED_BODY()
public:

protected:

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "OwnerAI", meta = (AllowPrivateAccess = true))
	UBehaviorTreeComponent* ownerComp;

	ADA_AIController* aiController;

public:
	UDBT_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory)override;

private:
	UFUNCTION()
	void OnEndSkill();
};