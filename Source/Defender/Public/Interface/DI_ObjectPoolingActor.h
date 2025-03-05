#pragma once

#include "../Defender.h"
#include "UObject/Interface.h"
#include "DI_ObjectPoolingActor.generated.h"

//������Ʈ Ǯ���� ������ ������ �������̽�
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UDI_ObjectPoolingActor : public UInterface
{
	GENERATED_BODY()
};

class DEFENDER_API IDI_ObjectPoolingActor
{
	GENERATED_BODY()

public:
	/*
	* Enables the actor's mesh for object pooling
	* @param[in] _bIsActive If true, activate
	*/
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, category = "ObjectPooling")
	void MeshActive(bool _bIsActive);
	/*
	* Enable collision for actors capable of object pooling
	* @param[in] _bIsActive If true, activate
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ObjectPooling")
	void CollisionActive(bool _bIsActive);
	/*
	* Enables actors capable of object pooling
	* @param[in] _bIsActive If true, activate
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, category = "ObjectPooling")
	void ActiveSetting(bool _bIsActive);
};
