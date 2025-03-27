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
    // Ŭ���̾�Ʈ���� ä�� �޽��� ���� (������ RPC ȣ��)
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    void SendChatMessage(const FString& Message);

    // �������� Ŭ���̾�Ʈ�� ä�� �޽��� ���� (Ŭ���̾�Ʈ RPC)
    UFUNCTION(Client, Reliable, BlueprintCallable, Category = "Baseball Game")
    void ClientReceiveMessage(const FString& Message);

protected:
    // �������� �޽��� ó�� (Ŭ���̾�Ʈ �� ����)
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerProcessMessage(const FString& Message);
    bool ServerProcessMessage_Validate(const FString& Message);
    void ServerProcessMessage_Implementation(const FString& Message);

    // UI�� �޽��� ǥ�� (�������Ʈ���� ����)
    UFUNCTION(BlueprintImplementableEvent, Category = "Baseball Game")
    void DisplayMessage(const FString& Message);
};
