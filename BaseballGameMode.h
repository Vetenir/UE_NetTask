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

    // ä�� �޽��� ó�� (���� �Լ�)
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void ProcessChatMessage(class APlayerController* FromPlayer, const FString& Message);

    // ���� ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void InitializeGame();

    // �޽��� ��ε�ĳ��Ʈ
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void BroadcastMessage(const FString& Message);

protected:
    virtual void BeginPlay() override;

private:
    // ���� �Է� ó��
    void ProcessGameInput(APlayerController* FromPlayer, const FString& Input);

    // �¸� ó��
    void HandleWinner(APlayerController* WinningPlayer);

    // OUT ó��
    void HandleOutCondition(APlayerController* OutPlayer);

    // ���º� ó��
    void HandleDraw();

    // �÷��̾� ���� Ȯ�� (Host �Ǵ� Guest)
    bool IsHostPlayer(APlayerController* Player);

private:
    // ������ ������ 3�ڸ� ����
    TArray<int32> ServerNumbers;

    // ȣ��Ʈ�� �Խ�Ʈ�� ���� �õ� Ƚ��
    int32 HostAttempts;
    int32 GuestAttempts;

    // ���� �� (true = Host, false = Guest)
    bool bIsHostTurn;

    // ���� Ȱ��ȭ ����
    bool bGameActive;

    // ȣ��Ʈ �÷��̾� ����
    APlayerController* HostPlayer;

    // �Խ�Ʈ �÷��̾� ����
    APlayerController* GuestPlayer;
};
