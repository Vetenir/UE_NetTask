// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameLibrary.h"
#include "Engine/Engine.h"

TArray<int32> UBaseballGameLibrary::GenerateUniqueNumbers()
{
    TArray<int32> Numbers;
    TArray<int32> Available = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // 1~9���� ���� ���� 3�� ����
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
    // "/"�� �����ϴ��� Ȯ��
    if (!Input.StartsWith("/"))
    {
        return false;
    }

    // "/"�� ������ ������ ���� ���ڿ�
    FString NumbersStr = Input.RightChop(1);

    // 3�ڸ� �������� Ȯ��
    if (NumbersStr.Len() != 3)
    {
        return false;
    }

    OutNumbers.Empty();

    // �� �ڸ��� 1~9 ������ �������� Ȯ���ϰ� �ߺ� üũ
    for (int32 i = 0; i < 3; i++)
    {
        TCHAR Char = NumbersStr[i];

        // ���ڰ� �ƴϰų� 0�� ��� 
        if (!FChar::IsDigit(Char) || Char == '0')
        {
            return false;
        }

        int32 Digit = FCString::Atoi(*(NumbersStr.Mid(i, 1)));

        // �ߺ� üũ
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
        // ���� ��ġ, ���� ���� -> ��Ʈ����ũ
        if (ServerNumbers[i] == ClientNumbers[i])
        {
            OutStrikes++;
        }
        // �ٸ� ��ġ�� ���� ���ڰ� �ִ��� -> ��
        else if (ServerNumbers.Contains(ClientNumbers[i]))
        {
            OutBalls++;
        }
    }
}