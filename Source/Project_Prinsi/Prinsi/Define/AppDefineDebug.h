// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPrinsi, Log, All);


#if !UE_BUILD_SHIPPING

/**
* @brief　	GEngine输出Debug信息
* @memo　	可输出各种参数
*/
#define APP_SCR_ERROR(ParamMessage,...) \
	if (GEngine) \
	{ \
		FString UseMsg =FString::Printf(ParamMessage,##__VA_ARGS__);\
		FString DebugText = FString::Printf(TEXT("%s [%s]"), *UseMsg,TEXT(__FUNCTION__)); \
		GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, DebugText); \
	}


/**
* @brief　	UELog输出Debug信息
* @memo　	可输出各种参数
*/
#define APP_ERROR(ParamMessage, ...) \
	UE_LOG( \
		LogPrinsi,\
		Error, \
		TEXT("%s\n[%s]\n[%s:%d]"), \
		*FString::Printf(ParamMessage, ##__VA_ARGS__), TEXT(__FUNCTION__), TEXT(__FILE__), __LINE__ \
	)

#define APP_WARNING(ParamMessage, ...) \
	UE_LOG( \
		LogPrinsi, \
		Warning, \
		TEXT("%s\n[%s]\n[%s:%d]"), \
		*FString::Printf(ParamMessage, ##__VA_ARGS__), TEXT(__FUNCTION__), TEXT(__FILE__), __LINE__ \
	)

#define APP_LOG(ParamMessage, ...) \
	UE_LOG( \
		LogPrinsi, \
		Log, \
		TEXT("%s\n[%s]\n[%s:%d]"), \
		*FString::Printf(ParamMessage, ##__VA_ARGS__), TEXT(__FUNCTION__), TEXT(__FILE__), __LINE__ \
	)

#else

#define APP_SCR_ERROR(ParamMessage){ }
#define APP_ERROR(ParamMessage, ...) { }
#define APP_WARNING(ParamMessage, ...) { }
#define APP_LOG(ParamMessage, ...) { }

#endif