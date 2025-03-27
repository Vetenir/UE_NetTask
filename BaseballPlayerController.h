// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    // 클라이언트에서 채팅 메시지 전송 (서버에 RPC 호출)
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void SendChatMessage(const FString& Message);

    // 서버에서 클라이언트로 채팅 메시지 수신 (클라이언트 RPC)
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Baseball Game")
    void ClientReceiveMessage(const FString& Message);

protected:
    // 서버에서 메시지 처리 (클라이언트 → 서버)
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerProcessMessage(const FString& Message);
    bool ServerProcessMessage_Validate(const FString& Message);
    void ServerProcessMessage_Implementation(const FString& Message);

    // UI에 메시지 표시 (블루프린트에서 구현)
    UFUNCTION(BlueprintImplementableEvent, Category = "Baseball Game")
    void DisplayMessage(const FString& Message);
};
