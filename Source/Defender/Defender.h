#pragma once

#include "CoreMinimal.h"
#include "DF_Struct.h"

DECLARE_LOG_CATEGORY_EXTERN(Defender, Log, All);
#define DFLOG_CALLINFO (FString(__FUNCTION__)+TEXT("(")+FString::FromInt(__LINE__)+TEXT(")"))
#define DFLOG_S(Verbosity) UE_LOG(Defender,Verbosity,TEXT("%s"),*DFLOG_CALLINFO)
#define DFLOG(Verbosity,Format,...) UE_LOG(Defender,Verbosity,TEXT("%s %s"),*DFLOG_CALLINFO, *FString::Printf(Format,##__VA_ARGS__))
#define DFLOG_N(Verbosity) DFLOG(Verbosity, TEXT("%s"), *GetName());
#define DFCHECK(Expr,...) {if(!(Expr)){DFLOG(Error,TEXT("ASSERTION : %s"),TEXT("'"#Expr"'"));return __VA_ARGS__;}}
#define GET_NAME(variable) #variable
#define PRINTSTR(Key,Time,Color,Format,...) GEngine->AddOnScreenDebugMessage(Key,Time,Color,FString::FromInt(Key) + FString(TEXT(" : ")) + FString::Printf(Format,##__VA_ARGS__))

//In game viewport order
#define VO_MAIN_WIDGET 0
#define VO_CROSS_HAIR 1
#define VO_INFOMATION 2
#define VO_SHOP 3
#define VO_EQUIP_DROP 4
#define VO_BUY_SELL 5
#define VO_BUY_SELL_COUNT 6
#define VO_FLOATING_DAMAGE 7
#define VO_INTERACTION 8
#define VO_BOSS_HP 9
#define VO_SCOPE 10
#define VO_RETURN_MENU 10
#define VO_POPUP 11
#define VO_STAGE_CLEAR 12
#define VO_STAGE_START 12
#define VO_STAGE_DEFEATED 12
#define VO_LOADING_SCREEN 13

//Select viewport order
#define VO_MAIN_S 0
#define VO_POPUP_S 1

#define PLAYER_SPEED			 1
#define PLAYER_SENSITIVITY		 2
#define PLAYER_MOVEMENTMODE		 3
#define PLAYER_ATTACK_INPUT		 4
#define WEAPON_MODE_CHANGE		 5
#define PLAYER_ATTACK_HIT		 6
#define LOCK_ON_TARGET			 7
#define PLAYER_STATE			 8
#define TARGET_DISTANCE			 9
#define ATTACK_DISTANCE			 10