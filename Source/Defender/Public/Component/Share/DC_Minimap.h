#pragma once

#include "Component/DC_PawnComponent.h"
#include "DC_Minimap.generated.h"

class USceneCaptureComponent2D;
class USpringArmComponent;
class UPaperSpriteComponent;
class UPaperSprite;
class UTextureRenderTarget2D;

//�÷��̾�� ���͸� �̴ϸʿ� ǥ�����ִ� ������Ʈ
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
	UPaperSpriteComponent*			characterMark;			//�̴ϸʿ� ĳ���͸� ǥ������ ������Ʈ

	UPROPERTY()
	USpringArmComponent*			minimapSpringArm;		//�̴ϸ� ī�޶� �������� ��������

	UPROPERTY()
	USceneCaptureComponent2D*		minimapCamera;			//�̴ϸ��� �׸� ī�޶�
public:
	UDC_Minimap();

protected:
	virtual void BeginPlay()override;
	virtual void InitializeComponent()override;

private:

};
