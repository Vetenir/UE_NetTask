// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BaseballGameLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API UBaseballGameLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
    // �ߺ� ���� 3�ڸ� 1~9 ���� ���� 
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    static TArray<int32> GenerateUniqueNumbers();

    // �Է� ����: �ߺ� ���� 3�ڸ� 1~9 �������� Ȯ��
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    static bool ValidateInput(const FString& Input, TArray<int32>& OutNumbers);

    // ��Ʈ����ũ/�� ���
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    static void CalculateStrikeAndBall(const TArray<int32>& ServerNumbers, const TArray<int32>& ClientNumbers, int32& OutStrikes, int32& OutBalls);
};
