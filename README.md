# UE_NetTask
언리얼 네트워크 공부. 야구 게임 구현.

https://youtu.be/VIVDMa66bKA

#

## BaseballGameLibrary
* 게임 유틸리티 함수들을 포함하는 정적 라이브러리 클래스
* GenerateUniqueNumbers(): 1~9 사이 중복 없는 3자리 숫자 생성
* CalculateStrikeAndBall(): 입력된 숫자와 서버 숫자 비교하여 스트라이크/볼 계산

## BaseballGameMode
* 게임의 전체적인 흐름과 규칙을 관리하는 주요 게임 모드 클래스
* 게임 초기화, 턴 관리, 승패 판정 로직 구현
* ProcessChatMessage(): 채팅 및 게임 입력 처리
* ProcessGameInput(): 게임 입력 검증 및 게임 로직 처리
* InitializeGame(): 게임 시작 시 초기 설정 (난수 생성, 시도 횟수 초기화 등)
* BroadcastMessage(): 모든 플레이어에게 메시지 전송
* HandleWinner(), HandleOutCondition(), HandleDraw(): 게임 종료 상황 처리

## BaseballPlayerController
* 플레이어 컨트롤러 클래스
* 채팅 메시지 송수신 관련 RPC(원격 프로시저 호출) 함수 구현
* 클라이언트와 서버 간 메시지 통신 처리
* SendChatMessage(): 메시지 전송 함수
* ClientReceiveMessage(): 클라이언트에서 메시지 수신 함수

## WidgetBlueprint
주어진 채팅 프로젝트 위젯블루프린트를 수정하였습니다.
![image](https://github.com/user-attachments/assets/f6147a8a-7800-4cde-b84b-7a428ad96a73)
