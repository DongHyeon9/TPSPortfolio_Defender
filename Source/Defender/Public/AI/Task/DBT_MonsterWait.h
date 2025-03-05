#pragma once

#include "../Defender.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "DBT_MonsterWait.generated.h"

class ADA_AIController;

class UBehaviorTreeComponent;

//AI�� �����ð����� ��ٸ������ִ� �׽�ũ
UCLASS()
class DEFENDER_API UDBT_MonsterWait : public UBTTask_Wait
{
	GENERATED_BODY()
public:

protected:

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,category="OwnerAI",meta=(AllowPrivateAccess=true))
	ADA_AIController* aiController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "OwnerAI", meta = (AllowPrivateAccess = true))
	UBehaviorTreeComponent* ownerComp;

public:
	UDBT_MonsterWait();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& _OwnerComp, uint8* _NodeMemory);

private:
	UFUNCTION()
	void OnChangeTraceState(EMONSTER_TRACE_STATE _NewState);
};
