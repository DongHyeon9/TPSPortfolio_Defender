#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_AnimMontage.generated.h"

class UDO_AnimInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAnimInstanceChangeDelegate, UDO_AnimInstance*, _AnimInstance);

//캐릭터의 애님 몽타주와 애님인스턴스를 관리하는 컴포넌트
UCLASS(ClassGroup = (DC_Share), meta = (BlueprintSpawnableComponent))
class DEFENDER_API UDC_AnimMontage : public UDC_PawnComponent
{
	GENERATED_BODY()

public:
	//Delegate called when anim instance changes
	UPROPERTY(BlueprintAssignable)
	FOnAnimInstanceChangeDelegate onAnimInstanceChange;

protected:


private:
	//A Map that stores the AnimMontage of the character.key is FString
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "AnimMontageDefault", meta = (AllowPrivateAccess = true))
	TMap<FString, UAnimMontage*> animMontageMap;

	UDO_AnimInstance*		ownerAnimInstance;			//소유 캐릭터의 애님인스턴스
	UAnimMontage*			currentAnimMontage;			//현재 플레이중인 애님 몽타주

public:
	UDC_AnimMontage();
	/*
	* Load the animation montage stored in the map with the key of FString
	* @param[in] _MontageKey AnimMontage Key
	* @return Returns a montage pointer value that matches the FString
	*/
	UFUNCTION(BlueprintCallable, category = "UDC_AnimMontage")
	UAnimMontage* GetAnimMontage(const FString& _MontageKey);
	/*
	* Change the section of the montage that is playing
	* @param[in] _SectionName Name of montage section to replace
	*/
	UFUNCTION(BlueprintCallable, category = "UDC_AnimMontage")
	void JumpToMontageSection(const FName& _SectionName);
	/*
	* Determines which montage section will be played when the currently ongoing montage section ends.
	* @param[in] _PrevSection Name of the montage section currently in progress
	* @param[in] _NextSection Name of the next montage section
	*/
	UFUNCTION(BlueprintCallable, category = "UDC_AnimMontage")
	void SetNextSection(const FName& _PrevSection, const FName& _NextSection);
	/*
	* Get the currently in progress montage
	* @return Returns UAnimMontage*
	*/
	FORCEINLINE UAnimMontage* GetCurrentAnimMontage() { return currentAnimMontage; }
	/*
	* Play anim montage based on key
	* @param[in] _MontageKey AnimMontage Key
	* @return Successful execution of Animmontage
	*/
	UFUNCTION(BlueprintCallable, category = "UDC_AnimMontage")
	bool PlayAnimMontage(const FString& _MontageKey, float _PlayRate = 1.f);
	/*
	* Play anim montage based on UAnimMontage*
	* @param[in] _MontageAnim AnimMontage pointer
	* @return Successful execution of Animmontage
	*/
	bool PlayAnimMontage(UAnimMontage* _MontageAnim, float _PlayRate = 1.f);
	/*
	* Stops the currently playing Animmontage from playing.
	*/
	void StopAnimMontage();
	/*
	* Add animmontage with key
	* @param[in] _MontageKey Key to call up the montage
	* @param[in] _AddMontage Anim Montage to be saved
	* @return Return the bool If _AddMontage is nullptr, returns false.
	*/
	bool AddAnimMontage(const FString& _MontageKey, UAnimMontage* _AddMontage);
	/*
	* Set animmontage map
	* @param[in] _AnimMontageMap Map to set up the animation montage
	*/
	void SetAnimMontage(TMap<FString, UAnimMontage*> _AnimMontageMap);
	/*
	* Sets the Anim Instance of the owned character.
	* @param[in] _AnimInstance AnimInstance to set
	*/
	void SetOwnerAnimInstance(UDO_AnimInstance* _AnimInstance);
	/*
	* Gets the Anim instance of the owned character.
	* @return return the UDO_AnimInstance*
	*/
	FORCEINLINE UDO_AnimInstance* GetOwnerAnimInstance()const { return ownerAnimInstance; }
	/*
	* Pauses the Animmontage currently in progress.
	*/
	void PauseAnimMontage();
	/*
	* Get the montage map that the character has.
	*/
	FORCEINLINE TMap<FString, UAnimMontage*> GetAnimMontageMap()const { return animMontageMap; }


protected:
	virtual void InitializeComponent()override;

private:
	UFUNCTION()
	void StartAnimMontage(UAnimMontage* _Montage);
};
