// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballPlayerController.h"
#include "BaseballGameMode.h"
#include "Engine/Engine.h"

void ABaseballPlayerController::SendChatMessage(const FString& Message)
{
    // Ŭ���̾�Ʈ���� ������ �޽��� ����
    ServerProcessMessage(Message);
}

void ABaseballPlayerController::ClientReceiveMessage_Implementation(const FString& Message)
{
    // ���� Ŭ���̾�Ʈ������ ���� (������ ������ ���� �÷��̾� ����)
    if (GetWorld()->GetNetMode() != NM_Client) return;

    DisplayMessage(Message); // �������Ʈ UI ǥ��
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
            FString::Printf(TEXT("[Client] %s"), *Message));
    }
}

bool ABaseballPlayerController::ServerProcessMessage_Validate(const FString& Message)
{
    // �⺻ ����, �ʿ�� �߰� ���� ����
    return true;
}

void ABaseballPlayerController::ServerProcessMessage_Implementation(const FString& Message)
{
    // ���� ������ �޽��� ó��
    ABaseballGameMode* GameMode = Cast<ABaseballGameMode>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->ProcessChatMessage(this, Message);
    }
}