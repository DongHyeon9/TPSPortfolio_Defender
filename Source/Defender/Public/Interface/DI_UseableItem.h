#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DI_UseableItem.generated.h"

class ADA_Character;

//사용가능한 아이템의 인터페이스
UINTERFACE(MinimalAPI)
class UDI_UseableItem : public UInterface
{
	GENERATED_BODY()
};

class DEFENDER_API IDI_UseableItem
{
	GENERATED_BODY()

public:
	/*
	* Interface for using items
	* Logic implementation is done in subclasses.
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "Useable Item")
	void UseItem(ADA_Character* _UsedCharacter);
};
