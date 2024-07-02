// © 2024 Team 'K_YangGaeng'. All rights reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(K_YG, Log, All);

// 호출 지점 출력
#define K_YG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// 간단한 로그 출력
#define K_YG_SIMPLE(Verbosity) UE_LOG(K_YG, Verbosity, TEXT("%s"), *K_YG_CALLINFO)

// 텍스트 형식을 추가한 로그 출력
#define K_YG_UELOG(Verbosity, Format, ...) UE_LOG(K_YG, Verbosity, TEXT("%s %s"), *K_YG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

// Assertion : 확인
#define K_YG_CHECK(Expr, ...) {if(!(Expr)) { K_YG_UELOG(Error, TEXT("Assertion : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}