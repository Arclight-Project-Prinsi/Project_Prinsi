// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 *
 */

 //ws-----------------------------------------------------------
#define APP_DEBUG_SCREEN true			// 是否启用AddOnScreenDebugMessage的报错显示

 /* @memo
 *  Debug用Log信息输出格式
 *	- 函数名
 *	- 文件名:行号
 *	- 内容
 */
 // @note
 //ws2--------------------------------------------------------------
DECLARE_LOG_CATEGORY_EXTERN(LogPrinsi, Log, All);

//#define APP_LOG(Format,...)\
//	UE_LOG(LogPrinsi,Log,TEXT("[%s]"Format),TEXT(__FUNCTION__),##_VA_ARGS_)
//
//#define APP_WARNING(Format, ...) \
//	UE_LOG(LogPrinsi, Warning, TEXT("[%s] " Format), TEXT(__FUNCTION__), ##__VA_ARGS__)
//
//#define APP_ERROR(Format, ...) \
//	UE_LOG(LogPrinsi, Error, TEXT("[%s] " Format), TEXT(__FUNCTION__), ##__VA_ARGS__)

// @todo
//1.两个换行是什么意思？
//2.内容能不能在warning下面？

#define APP_ERROR(Format, ...) \
	UE_LOG( \
		LogPrinsi, \
		Error, \
		TEXT("%s\n[%s]\n[%s:%d]"), \
		*FString::Printf(Format, ##__VA_ARGS__), \
		TEXT(__FUNCTION__), \
		TEXT(__FILE__), \
		__LINE__ \
	)

#define APP_WARNING(Format, ...) \
	UE_LOG( \
		LogPrinsi, \
		Warning, \
		TEXT("%s\n[%s]\n[%s:%d]"), \
		*FString::Printf(Format, ##__VA_ARGS__), \
		TEXT(__FUNCTION__), \
		TEXT(__FILE__), \
		__LINE__ \
	)

#define APP_LOG(Format, ...) \
	UE_LOG( \
		LogPrinsi, \
		Log, \
		TEXT("%s\n[%s]\n[%s:%d]"), \
		*FString::Printf(Format, ##__VA_ARGS__), \
		TEXT(__FUNCTION__), \
		TEXT(__FILE__), \
		__LINE__ \
	)

//ws3-------------------------------------------------------

/* @memo
 *  【Debug用AddOnScreenDebugMessage信息输出格式】
 *	- 函数名
 *	- 内容
 */

 //#define APP_SCREEN(Message) \
 //	if (APP_DEBUG_SCREEN&&Engine) { GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, Message); }

#define APP_SCREEN_ERROR(Message) \
	if (GEngine) \
	{ \
		FString DebugText = FString::Printf( \
			TEXT("[%s] %s"), \
			TEXT(__FUNCTION__), \
			Message \
		); \
		GEngine->AddOnScreenDebugMessage( \
			-1, \
			5.f, \
			FColor::Red, \
			DebugText \
		); \
	}
//ws--------------------------------------------------------------

class PROJECT_PRINSI_API AppDefineDebug
{
public:
	AppDefineDebug();
	~AppDefineDebug();
};
