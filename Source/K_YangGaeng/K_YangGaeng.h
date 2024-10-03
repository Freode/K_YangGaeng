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

// Simple Check : Not Assertion : 단순 조건문 확인 - return 코드 줄이는 매크로
#define K_YG_SIMPLE_CHECK(Expr, ...) {if(!(Expr)) { K_YG_UELOG(Warning, TEXT("The function is pre-termination : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__; }}

// Print text on viewport
#define K_YG_PRINT(Time, Color, Format, ...) {if(GEngine){GEngine->AddOnScreenDebugMessage(-1, Time, Color, FString::Printf(Format, ##__VA_ARGS__));}}

// Print text on viewport during a frame
#define K_YG_PRINT_A_FRAME(Color, Format, ...){if(GEngine){GEngine->AddOnScreenDebugMessage(-1, FApp::GetDeltaTime(), Color, FString::Printf(Format, ##__VA_ARGS__));}}

// Cached client name
static FString CachedClientIdentifier = TEXT("UnknownClient");

//
#define K_YG_CLIENT_IDENTIFIER()(GetClientIdentifier())

//
inline FString GetClientIdentifier()
{
	// When cached client name is valid, return it
	if (CachedClientIdentifier.Equals(TEXT("UnknownClient")) == false)
	{
		return CachedClientIdentifier;
	}

    // GEngine is valid
	if (!GEngine) { return FString("UnknownClient"); }

    // Check current client number
    for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
    {
        // Find game world
        if (WorldContext.World() && WorldContext.World()->IsGameWorld())
        {
            UWorld* World = WorldContext.World();

            // If current controller is server, return server text
            if (World->GetNetMode() == NM_DedicatedServer)
            {
                CachedClientIdentifier = FString("Server");
                return CachedClientIdentifier;
            }
            // When it isn't server, return client & number which is decided by game mode
            else
            {
                APlayerController* PC = World->GetFirstPlayerController();
                if (PC && PC->GetLocalPlayer())
                {
                    CachedClientIdentifier = FString::Printf(TEXT("Client_%d"), PC->GetLocalPlayer()->GetControllerId());
                    return CachedClientIdentifier;
                }
            }
        }
    }
       
    // When it isn't find, return previous name
	return FString("UnknownClient");
}