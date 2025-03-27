// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseballGameMode.h"
#include "BaseballPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

ABaseballGameMode::ABaseballGameMode()
{
    // 기본 초기화
    HostAttempts = 3;
    GuestAttempts = 3;
    bIsHostTurn = true;
    bGameActive = false;
    HostPlayer = nullptr;
    GuestPlayer = nullptr;
}

void ABaseballGameMode::BeginPlay()
{
    Super::BeginPlay();
    //if (GEngine)
    //{
    //    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("게임 시작!"));
    //}

    // 서버에서만 게임 초기화
    if (GetLocalRole() == ROLE_Authority)
    {
        // 게임 시작 지연 (플레이어 접속 대기)
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseballGameMode::InitializeGame, 2.0f, false);
    }
}

void ABaseballGameMode::InitializeGame()
{
    // 서버에서만 게임 초기화
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // 플레이어 참조 저장
    TArray<AActor*> FoundPlayers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), FoundPlayers);

    if (FoundPlayers.Num() > 0)
    {
        HostPlayer = Cast<APlayerController>(FoundPlayers[0]);
    }

    if (FoundPlayers.Num() > 1)
    {
        GuestPlayer = Cast<APlayerController>(FoundPlayers[1]);
    }

    // 3자리 난수 생성
    ServerNumbers = UBaseballGameLibrary::GenerateUniqueNumbers();

    // 게임 상태 초기화
    HostAttempts = 3;
    GuestAttempts = 3;
    bIsHostTurn = true;
    bGameActive = true;

    // 디버깅용 (실제 게임에서는 제거)
    FString DebugString = FString::Printf(TEXT("서버 숫자: %d%d%d"), ServerNumbers[0], ServerNumbers[1], ServerNumbers[2]);
    UE_LOG(LogTemp, Warning, TEXT("%s"), *DebugString);

    // 게임 시작 메시지
    BroadcastMessage(TEXT("새 게임이 시작되었습니다! Host 차례입니다."));
}

void ABaseballGameMode::ProcessChatMessage(APlayerController* FromPlayer, const FString& Message)
{
    // 서버에서만 처리
    if (GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // 게임이 준비되지 않은 경우
    if (!bGameActive || !HostPlayer || (!GuestPlayer && FromPlayer != HostPlayer))
    {
        BroadcastMessage(TEXT("게임이 준비되지 않았습니다. 모든 플레이어가 접속할 때까지 기다려주세요."));
        return;
    }

    // 현재 턴이 아닌 경우
    bool isHost = IsHostPlayer(FromPlayer);
    if ((bIsHostTurn && !isHost) || (!bIsHostTurn && isHost))
    {
        FString playerName = isHost ? TEXT("Host") : TEXT("Guest");
        BroadcastMessage(playerName + TEXT("의 차례가 아닙니다."));
        return;
    }

    ProcessGameInput(FromPlayer, Message);

	// 채팅도 가능하게 사용하려면 아래 주석 해제
    // 게임 입력 처리
    //if (Message.StartsWith("/"))
    //{
    //    ProcessGameInput(FromPlayer, Message);
    //}
    //else
    //{
    //    // 일반 채팅 메시지 처리
    //    FString playerName = isHost ? TEXT("Host") : TEXT("Guest");
    //    BroadcastMessage(playerName + TEXT(": ") + Message);
    //}
}

void ABaseballGameMode::ProcessGameInput(APlayerController* FromPlayer, const FString& Input)
{
    TArray<int32> ClientNumbers;
    bool isValid = UBaseballGameLibrary::ValidateInput(Input, ClientNumbers);

    FString playerName = IsHostPlayer(FromPlayer) ? TEXT("Host") : TEXT("Guest");

    if (!isValid)
    {
        // 입력 오류 메시지 분기
        if (!Input.StartsWith("/"))
        {
            BroadcastMessage(TEXT("입력이 '/'로 시작하지 않습니다."));
        }
        else if (Input.RightChop(1).Len() != 3)
        {
            BroadcastMessage(TEXT("입력이 3자리 숫자가 아닙니다."));
        }
        else
        {
            BroadcastMessage(TEXT("유효하지 않은 입력입니다. 중복된 숫자나 0이 포함되어 있습니다."));
        }

        HandleOutCondition(FromPlayer);
        return;
    }

    int32 Strikes = 0;
    int32 Balls = 0;
    UBaseballGameLibrary::CalculateStrikeAndBall(ServerNumbers, ClientNumbers, Strikes, Balls);

    // 결과 메시지 생성
    FString ResultMessage = playerName + TEXT(": ") + Input + TEXT(" => ");
    if (Strikes == 0 && Balls == 0)
    {
        ResultMessage += TEXT("OUT");
    }
    else
    {
        ResultMessage += FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
    }
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ResultMessage);
    BroadcastMessage(ResultMessage);

    // 승리 조건 체크
    if (Strikes == 3)
    {
        HandleWinner(FromPlayer);
        return;
    }

    // 턴 관리 및 시도 횟수 감소
    if (IsHostPlayer(FromPlayer))
    {
        HostAttempts--;
        bIsHostTurn = false;

        if (HostAttempts <= 0 && GuestAttempts <= 0)
        {
            HandleDraw();
            return;
        }
        //FString DebugMessage = FString::Printf(TEXT("Guest 차례입니다. 남은 기회: %d"), GuestAttempts);
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
        BroadcastMessage(FString::Printf(TEXT("Guest 차례입니다. 남은 기회: %d"), GuestAttempts));
    }
    else  // Guest
    {
        GuestAttempts--;
        bIsHostTurn = true;

        if (HostAttempts <= 0 && GuestAttempts <= 0)
        {
            HandleDraw();
            return;
        }
        //FString DebugMessage = FString::Printf(TEXT("Host 차례입니다. 남은 기회: %d"), HostAttempts);
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
        BroadcastMessage(FString::Printf(TEXT("Host 차례입니다. 남은 기회: %d"), HostAttempts));
    }
}

void ABaseballGameMode::HandleOutCondition(APlayerController* OutPlayer)
{
    if (IsHostPlayer(OutPlayer))
    {
        //FString DebugMessage = TEXT("Guest Won!! 다시 게임이 시작됐다.");
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
        BroadcastMessage(TEXT("Guest Won!! 다시 게임이 시작됐다."));
    }
    else
    {
        //FString DebugMessage = TEXT("Host Won!! 다시 게임이 시작됐다.");
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
        BroadcastMessage(TEXT("Host Won!! 다시 게임이 시작됐다."));
    }

    InitializeGame();
}

void ABaseballGameMode::HandleWinner(APlayerController* WinningPlayer)
{
    if (IsHostPlayer(WinningPlayer))
    {
        //FString DebugMessage = TEXT("Host Won!! 다시 게임이 시작됐다.");
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
        BroadcastMessage(TEXT("Host Won!! 다시 게임이 시작됐다."));
    }
    else
    {
        //FString DebugMessage = TEXT("Guest Won!! 다시 게임이 시작됐다.");
        //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
        BroadcastMessage(TEXT("Guest Won!! 다시 게임이 시작됐다."));
    }

    InitializeGame();
}

void ABaseballGameMode::HandleDraw()
{
    //FString DebugMessage = TEXT("무승부군. 다시 게임을 시작하지");
    //GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, DebugMessage);
    BroadcastMessage(TEXT("무승부군. 다시 게임을 시작하지"));
    InitializeGame();
}

void ABaseballGameMode::BroadcastMessage(const FString& Message)
{
    const ENetMode NetMode = GetWorld()->GetNetMode();

    // 모든 플레이어 컨트롤러 처리
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(It->Get());
        if (!PC) continue;

        // 데디케이트 서버: 모든 클라이언트에 RPC 전송
        if (NetMode == NM_DedicatedServer)
        {
            PC->ClientReceiveMessage(Message);
        }
        // 리스닝 서버: 로컬 플레이어 제외하고 RPC 전송
        else if (NetMode == NM_ListenServer)
        {
            if (!PC->IsLocalController())
            {
                PC->ClientReceiveMessage(Message);
            }
        }
    }


    // 디버그 메시지 (옵션)
    //if (GEngine)
    //{
    //    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("[Broadcast] %s"), *Message));
    //}
}

bool ABaseballGameMode::IsHostPlayer(APlayerController* Player)
{
    return Player == HostPlayer;
}