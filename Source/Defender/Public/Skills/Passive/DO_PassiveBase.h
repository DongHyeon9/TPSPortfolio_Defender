#pragma once

#include "Skills/DO_SkillBase.h"
#include "UObject/NoExportTypes.h"
#include "DO_PassiveBase.generated.h"

//패시브 스킬의 최상위 클래스
UCLASS(Blueprintable, BlueprintType)
class DEFENDER_API UDO_PassiveBase : public UDO_SkillBase
{
	GENERATED_BODY()
public:
	
protected:

private:

public:
	UDO_PassiveBase();
	/*
	* Register a skill to change the status of the owned character.
	* Logic is implemented in subclasses
	* @param[in] _OwnerCharacter Character to own and register passive skills
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "PassiveSkill", meta = (HideSelfPin = true))
	void RegistPassive(ADA_Character* _OwnerCharacter);
	virtual void RegistPassive_Implementation(ADA_Character* _OwnerCharacter);
	/*
	* Delete a registered skill
	* Logic is implemented in subclasses
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "PassiveSkill", meta = (HideSelfPin = true))
	void RemovePassive();
	virtual void RemovePassive_Implementation();

protected:

private:

};
