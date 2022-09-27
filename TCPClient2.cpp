#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512
#define LOCALLOOP "127.0.0.1"
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
int main(int argc, char* argv[]) {
	int retval;
	int mPort;
	int MPort;
	//명령행 인수가 있으면 IP 주소로 이동
	if (argc > 1) SERVERIP = argv[1];

	printf("ip 주소 : ");
	char ip[50];
	gets_s(ip);
	printf("최소 포트 : ");
	scanf_s("%d", &mPort);
	printf("최대 포트 : ");
	scanf_s("%d", &MPort);
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//소켓 생성
	//connect()
	for (int i = mPort; i <= MPort; ++i) {
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		struct sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		inet_pton(AF_INET, ip, &serveraddr.sin_addr);
		serveraddr.sin_port = htons(i);
		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) printf("포트번호 : %d connect 실패\n", i);
		else printf("포트번호 : %d connect 성공\n", i);
		closesocket(sock);
	}
	WSACleanup();
	return 0;
}