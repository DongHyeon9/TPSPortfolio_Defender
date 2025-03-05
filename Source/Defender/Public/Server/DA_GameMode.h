#pragma once

#include "../Defender.h"
#include "GameFramework/GameMode.h"
#include "DA_GameMode.generated.h"

class ADA_LevelScript;

//������Ʈ���� �⺻���� ���Ǵ� ���Ӹ��
UCLASS()
class DEFENDER_API ADA_GameMode : public AGameMode
{
	GENERATED_BODY()
public:

protected:

private:
	//Level Script Actor for the current level
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LevelScriptActor", meta = (AllowPrivateAccess = true))
	ADA_LevelScript* levelScript;

public:

protected:
	virtual void BeginPlay()override;

private:

};
