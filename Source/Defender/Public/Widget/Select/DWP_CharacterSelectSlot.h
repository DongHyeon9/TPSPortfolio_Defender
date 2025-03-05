#pragma once

#include "Widget/DW_PlayerWidget.h"
#include "DWP_CharacterSelectSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterCheckedDelegate, UDWP_CharacterSelectSlot*, _CheckedSlot);

class UImage;
class UCheckBox;

//캐릭터를 선택할 수 있는 슬롯위젯
UCLASS()
class DEFENDER_API UDWP_CharacterSelectSlot : public UDW_PlayerWidget
{
	GENERATED_BODY()
public:
	//Delegate called when the widget is checked
	UPROPERTY(BlueprintAssignable)
	FOnCharacterCheckedDelegate onChecked;

protected:

private:
	//Image of character to choose
	UPROPERTY(BlueprintReadOnly, category = "Select Character Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UImage* img_Character;

	//Checkbox to indicate check status
	UPROPERTY(BlueprintReadOnly, category = "Select Character Slot", meta = (BindWidget, AllowPrivateAccess = true))
	UCheckBox* cb_Check;

	int32		characterIndex;		//선택할 캐릭터의 인덱스

public:
	/*
	* Set the index of the character
	* @param[in] _Index index to set
	*/
	void SetCharacterIndex(int32 _Index);
	/*
	* Set image for character
	* @param[in] _NewImage Image to set
	*/
	void SetCharacterImage(UTexture2D* _NewImage);
	/*
	* Change check status
	* @param[in] _bIsCheck If true, changes to checked status.
	*/
	void SetCheckState(bool _bIsCheck);
	/*
	* Get the index of the character
	* @return Returns int32
	*/
	FORCEINLINE int32 GetCharacterIndex()const { return characterIndex; }

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;

private:
	UFUNCTION()
	void OnCheck(bool _bIsChecked);

};
