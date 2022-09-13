#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int main(int argc, char* argv[]) {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(1,1), &wsa) != 0)
		return 1;

	int mask = 255;
	int version = wsa.wVersion;
	int highversion = wsa.wHighVersion;

	int version_low = version >> 8;
	int version_high = version & mask;

	int highversion_low = highversion >> 8;
	int highversion_high = highversion & mask;

	printf("[알림] 윈속 초기화 성공\n");
	printf("버전 : %d.%d\n최대지원버전 : %d.%d\nszDescription : %s\nszSystemStatus : %s\n", version_low, version_high, highversion_low, highversion_high, wsa.szDescription, wsa.szSystemStatus);


	//소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] 소켓 생성 성공\n");

	//소켓닫기
	closesocket(sock);
	//윈속 종료
	WSACleanup();
	return 0;
}