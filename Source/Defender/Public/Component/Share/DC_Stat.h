#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Stat.generated.h"

#define MAX_LEVEL 20
#define MIN_LEVEL 1

class ADA_Character;
class UDC_AnimMontage;
class UDO_AnimInstance;
class UDWP_FloatingDamage;

class UNiagaraSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeHPDelegate, float, _MaxHP, float, _CurrentHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangeEXPDelegate, int32, _TotalEXP, int32, _CurrentExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeLevelDelegate, int32, _Level);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageChangeDelegate, float, _BaseDamage, float, _AdditionalDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpeedChangeDelegate, float, _BaseSpeed, float, _AdditionalSpeed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FonStateChangeDelegate, ECHARACTER_STATE, _CurrentState);

//캐릭터의 스탯을 관리하는 컴포넌트
UCLASS(ClassGroup = (DC_Share), meta = (BlueprintSpawnableComponent))
class DEFENDER_API UDC_Stat : public UDC_PawnComponent
{
	GENERATED_BODY()
public:
	//Delegate called when a character state changes
	UPROPERTY(BlueprintAssignable)
	FonStateChangeDelegate onChangeState;

	//Delegate called when the character's HP changes
	UPROPERTY(BlueprintAssignable)
	FOnChangeHPDelegate	onChangeHP;

	//Delegate called when a character's EXP changes
	UPROPERTY(BlueprintAssignable)
	FOnChangeEXPDelegate onChangeEXP;

	//Delegate called when the character changes level
	UPROPERTY(BlueprintAssignable)
	FOnChangeLevelDelegate onChangeLevel;

	//Delegate called when the character changes level
	UPROPERTY(BlueprintAssignable)
	FOnDamageChangeDelegate onChangeDamage;

	//Delegate called when the character changes level
	UPROPERTY(BlueprintAssignable)
	FOnSpeedChangeDelegate onChangeSpeed;

protected:

private:
	//This is the CSV to manage the character's stats.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "CharacterData", meta = (AllowPrivateAccess = true))
	UDataTable* characterDataTable;

	//Structure for current stats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "CharacterData", meta = (AllowPrivateAccess = true))
	FCharacterStat currentStats;

	//Effects shown when the level increases
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Effect", meta = (AllowPrivateAccess = true))
	UNiagaraSystem* levelUpEffect;

	//Indicates the character's status
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "CharacterState", meta = (AllowPrivateAccess = true))
	ECHARACTER_STATE currentState;

	UPROPERTY()
	UDC_AnimMontage*		montageComp;			//캐릭터가 가지고있는 몽타주 컴포넌트

	FDFCharacterData*		currentDataTable;		//캐릭터의 현재 스탯정보를 담은 데이터 테이블
	float					sturnTime;				//남은 스턴시간
	UDO_AnimInstance*		currentAnimInstance;	//현재 캐릭터의 애니메이션 인스턴스


public:
	UDC_Stat();
	/*
	* Add the character's HP
	* @param[in] _AddHP Adds to the character's current HP. If positive, it is added, if negative, it is minus.
	* @param[in] _EventInstigator Determine which controller is involved with HP.
	* @return return the bool
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	bool AddCurrentHP(float _AddHP, AController* _EventInstigator = nullptr);
	/*
	* Set the character's HP
	* @param[in] _NewHP Sets the character's current HP.
	*/
	void SetCurrentHP(float _NewHP);
	/*
	* Set the character's current level. Stats change according to the current level
	* @param[in] _NewLevel Set the character's current level
	*/
	void SetLevel(int32 _NewLevel);
	/*
	* Set additional attack damage
	* @param[in] _Key Key value to be set for additional attack damage
	* @param[in] _AddAttackDamage Additional attack damage to be set
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	void SetAdditionAttackDamage(const FString& _Key, float _AddAttackDamage);
	/*
	* Set additional move speed
	* @param[in] _Key Key value to be set for additional move speed
	* @param[in] _AddMoveSpeed Additional move speed to be set
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	void SetAdditionMoveSpeed(const FString& _Key, float _AddMoveSpeed);
	/*
	* Get additional attack damage appropriate to Key
	* @param[in] _Key Key of attack to retrieve
	* @return Return float
	*/
	float GetAdditionAttackDamage(const FString& _Key);
	/*
	* Get additional move speed appropriate to Key
	* @param[in] _Key Key of speed to retrieve
	* @return Return float
	*/
	float GetAdditionMoveSpeed(const FString& _Key);
	/*
	* Deletes additional attack damage corresponding to the Key.
	* @param[in] _Key Key of damage to retrieve
	* @return Returns success or failure
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	bool RemoveAttackDamage(const FString& _Key);
	/*
	* Deletes additional move speed corresponding to the Key.
	* @param[in] _Key Key of speed to retrieve
	* @return Returns success or failure
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	bool RemoveMoveSpeed(const FString& _Key);
	/*
	* Damage is inflicted depending on the input type.
	* @param[in] _DataType Type of damage to bring
	* @return Damage according to type
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "StatComp")
	float GetDamage(ESTAT_DATA_TYPE _DataType = ESTAT_DATA_TYPE::DEFAULT);
	/*
	* Speed is inflicted depending on the input type.
	* @param[in] _DataType Type of speed to bring
	* @return speed according to type
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "StatComp")
	float GetSpeed(ESTAT_DATA_TYPE _DataType = ESTAT_DATA_TYPE::DEFAULT);
	/*
	* Return a structure containing the character's current stat information
	* @return Return the const FCharacterStat&
	*/
	FORCEINLINE const FCharacterStat& GetCharacterCurrentStat()const { return currentStats; }
	/*
	* Returns the character's current data table
	* @return Return the FDFCharacterData*
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, category = "StatComp")
	const FDFCharacterData& GetCurrentDataTable()const { return *currentDataTable; }
	/*
	* Set the character's current EXP.
	* @param[in] _NewEXP Set the character's current EXP
	*/
	void SetEXP(int32 _NewEXP);
	/*
	* Add the character's current EXP.
	* @param[in] _AddEXP Add the character's current EXP
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	void AddEXP(int32 _AddEXP);
	/*
	* Add stun time. 
	* If there is still time remaining, the character will be stunned for the remaining time.
	* @param[in] _StunTime Stun time to add
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	void AddStunTime(float _StunTime);
	/*
	* Sets the character's current state
	* @param[in] _NewState Status of character to set
	*/
	UFUNCTION(BlueprintCallable, category = "StatComp")
	void SetState(ECHARACTER_STATE _NewState);
	/*
	* Get the current state of the character
	* @return return the ECHARACTER_STATE
	*/
	FORCEINLINE ECHARACTER_STATE GetState()const { return currentState; }

protected:
	virtual void BeginPlay()override;
	virtual void TickComponent(float _DeltaTime, ELevelTick _TickType, FActorComponentTickFunction* _ThisTickFunction)override;

private:
	//내부 호출 함수

	void CalculateMoveSpeed();

	UFUNCTION()
	void AnimInstanceChange(UDO_AnimInstance* _AnimInstance);

	UFUNCTION()
	void HitEnd();

	UFUNCTION()
	void DeadEnd();
};
