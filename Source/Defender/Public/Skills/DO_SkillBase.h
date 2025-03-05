#pragma once

#include "../Defender.h"
#include "UObject/NoExportTypes.h"
#include "Tickable.h"
#include "DO_SkillBase.generated.h"

class ADA_Character;

class UTexture2D;
class AWorldSettings;

//모든 스킬의 최상위 클래스
UCLASS(Abstract, NotBlueprintable)
class DEFENDER_API UDO_SkillBase : 
	public UObject,
	public FTickableGameObject
{
	GENERATED_BODY()
public:

protected:
	//Name of skill for storing data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Common")
	FString skillName;

	//Character using skills
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, category = "Skill Common")
	ADA_Character* ownerCharacter;

	//Whether to use ticks
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Skill Common")
	bool bUseTick;

	//Skill icon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, category = "Skill Common")
	UTexture2D* skillIcon;

private:

public:
	UDO_SkillBase();

	/*
	* Get the statID for using the tick
	* @return return the TStatId
	*/
	virtual TStatId GetStatId() const override;
	/*
	* Returns the world containing the current object
	* @return current world	UWorld*
	*/
	virtual UWorld* GetWorld()const override final;

	/*
	* Return the ULevel that this Actor is part of.
	*/
	UFUNCTION(BlueprintCallable, category = "Level")
	ULevel* GetLevel() const;

	/* 
	* Event called every frame, if ticking is enabled 
	*/
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void ReceiveTick(float _DeltaTime);
	/*
	* Get the skill icon
	* @return return the UTexture2D*
	*/
	FORCEINLINE UTexture2D* GetSkillIcon()const { return skillIcon; }
	/*
	* Get the name of the skill
	* @return return the const FString&
	*/
	FORCEINLINE const FString& GetSkillName()const { return skillName; }
	/*
	* Get the character that owns the skill
	* @return return the ADA_Character*
	*/
	FORCEINLINE ADA_Character* GetOwnerCharacter()const { return ownerCharacter; }

protected:
	void Tick(float _DeltaTime) override;

private:
	AWorldSettings* GetWorldSettings() const;
};
