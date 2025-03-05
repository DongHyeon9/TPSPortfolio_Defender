#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Minimap.generated.h"

class USceneCaptureComponent2D;
class USpringArmComponent;
class UPaperSpriteComponent;
class UPaperSprite;
class UTextureRenderTarget2D;

//플레이어와 몬스터를 미니맵에 표시해주는 컴포넌트
UCLASS()
class DEFENDER_API UDC_Minimap : public UDC_PawnComponent
{
	GENERATED_BODY()
public:

protected:

private:
	//Image to be displayed on minimap
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "MinimapSetting", meta = (AllowPrivateAccess))
	UPaperSprite* spriteSource;

	//color of image
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "MinimapSetting", meta = (AllowPrivateAccess))
	FLinearColor spriteColor;

	//Height at which to display the minimap
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "PlayerMinimapSetting", meta = (AllowPrivateAccess))
	float minimapHeight;

	//The length of the minimap up, down, left and right
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "PlayerMinimapSetting", meta = (AllowPrivateAccess))
	float orthoWidth;

	//Render texture to draw the minimap
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "PlayerMinimapSetting", meta = (AllowPrivateAccess))
	UTextureRenderTarget2D* textureTarget;

	UPROPERTY()
	UPaperSpriteComponent*			characterMark;			//미니맵에 캐릭터를 표시해줄 컴포넌트

	UPROPERTY()
	USpringArmComponent*			minimapSpringArm;		//미니맵 카메라를 고정해줄 스프링암

	UPROPERTY()
	USceneCaptureComponent2D*		minimapCamera;			//미니맵을 그릴 카메라
public:
	UDC_Minimap();

protected:
	virtual void BeginPlay()override;
	virtual void InitializeComponent()override;

private:

};
