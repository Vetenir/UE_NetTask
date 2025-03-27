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
    // 중복 없는 3자리 1~9 숫자 생성 
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    static TArray<int32> GenerateUniqueNumbers();

    // 입력 검증: 중복 없는 3자리 1~9 숫자인지 확인
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    static bool ValidateInput(const FString& Input, TArray<int32>& OutNumbers);

    // 스트라이크/볼 계산
    UFUNCTION(BlueprintCallable, Category = "Baseball Game")
    static void CalculateStrikeAndBall(const TArray<int32>& ServerNumbers, const TArray<int32>& ClientNumbers, int32& OutStrikes, int32& OutBalls);
};
