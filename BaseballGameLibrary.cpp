// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameLibrary.h"
#include "Engine/Engine.h"

TArray<int32> UBaseballGameLibrary::GenerateUniqueNumbers()
{
    TArray<int32> Numbers;
    TArray<int32> Available = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // 1~9에서 랜덤 숫자 3개 선택
    for (int32 i = 0; i < 3; i++)
    {
        int32 RandomIndex = FMath::RandRange(0, Available.Num() - 1);
        Numbers.Add(Available[RandomIndex]);
        Available.RemoveAt(RandomIndex);
    }

    return Numbers;
}

bool UBaseballGameLibrary::ValidateInput(const FString& Input, TArray<int32>& OutNumbers)
{
    // "/"로 시작하는지 확인
    if (!Input.StartsWith("/"))
    {
        return false;
    }

    // "/"를 제외한 이후의 숫자 문자열
    FString NumbersStr = Input.RightChop(1);

    // 3자리 숫자인지 확인
    if (NumbersStr.Len() != 3)
    {
        return false;
    }

    OutNumbers.Empty();

    // 각 자리가 1~9 사이의 숫자인지 확인하고 중복 체크
    for (int32 i = 0; i < 3; i++)
    {
        TCHAR Char = NumbersStr[i];

        // 숫자가 아니거나 0인 경우 
        if (!FChar::IsDigit(Char) || Char == '0')
        {
            return false;
        }

        int32 Digit = FCString::Atoi(*(NumbersStr.Mid(i, 1)));

        // 중복 체크
        if (OutNumbers.Contains(Digit))
        {
            return false;
        }

        OutNumbers.Add(Digit);
    }

    return true;
}

void UBaseballGameLibrary::CalculateStrikeAndBall(const TArray<int32>& ServerNumbers, const TArray<int32>& ClientNumbers, int32& OutStrikes, int32& OutBalls)
{
    OutStrikes = 0;
    OutBalls = 0;

    for (int32 i = 0; i < 3; i++)
    {
        // 같은 위치, 같은 숫자 -> 스트라이크
        if (ServerNumbers[i] == ClientNumbers[i])
        {
            OutStrikes++;
        }
        // 다른 위치에 같은 숫자가 있는지 -> 볼
        else if (ServerNumbers.Contains(ClientNumbers[i]))
        {
            OutBalls++;
        }
    }
}