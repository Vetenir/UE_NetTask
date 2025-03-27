// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseballGameLibrary.h"
#include "BaseballGameMode.generated.h"

UCLASS()
class SAMPLECHAT_API ABaseballGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
    ABaseballGameMode();

    // 채팅 메시지 처리 (서버 함수)
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void ProcessChatMessage(class APlayerController* FromPlayer, const FString& Message);

    // 게임 시작 함수
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void InitializeGame();

    // 메시지 브로드캐스트
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void BroadcastMessage(const FString& Message);

protected:
    virtual void BeginPlay() override;

private:
    // 게임 입력 처리
    void ProcessGameInput(APlayerController* FromPlayer, const FString& Input);

    // 승리 처리
    void HandleWinner(APlayerController* WinningPlayer);

    // OUT 처리
    void HandleOutCondition(APlayerController* OutPlayer);

    // 무승부 처리
    void HandleDraw();

    // 플레이어 역할 확인 (Host 또는 Guest)
    bool IsHostPlayer(APlayerController* Player);

private:
    // 서버가 생성한 3자리 숫자
    TArray<int32> ServerNumbers;

    // 호스트와 게스트의 남은 시도 횟수
    int32 HostAttempts;
    int32 GuestAttempts;

    // 현재 턴 (true = Host, false = Guest)
    bool bIsHostTurn;

    // 게임 활성화 여부
    bool bGameActive;

    // 호스트 플레이어 참조
    APlayerController* HostPlayer;

    // 게스트 플레이어 참조
    APlayerController* GuestPlayer;
};
