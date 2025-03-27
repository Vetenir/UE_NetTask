// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballPlayerController.h"
#include "BaseballGameMode.h"
#include "Engine/Engine.h"

void ABaseballPlayerController::SendChatMessage(const FString& Message)
{
    // 클라이언트에서 서버로 메시지 전송
    ServerProcessMessage(Message);
}

void ABaseballPlayerController::ClientReceiveMessage_Implementation(const FString& Message)
{
    // 오직 클라이언트에서만 실행 (리스닝 서버의 로컬 플레이어 차단)
    if (GetWorld()->GetNetMode() != NM_Client) return;

    DisplayMessage(Message); // 블루프린트 UI 표시
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
            FString::Printf(TEXT("[Client] %s"), *Message));
    }
}

bool ABaseballPlayerController::ServerProcessMessage_Validate(const FString& Message)
{
    // 기본 검증, 필요시 추가 로직 구현
    return true;
}

void ABaseballPlayerController::ServerProcessMessage_Implementation(const FString& Message)
{
    // 서버 측에서 메시지 처리
    ABaseballGameMode* GameMode = Cast<ABaseballGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->ProcessChatMessage(this, Message);
    }
}