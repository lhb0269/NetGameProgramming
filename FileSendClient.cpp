#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

char SERVERIP[50];
#define SERVERPORT 9000
#define BUFSIZE 32768

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
typedef struct Files {
	char fname[100];
	long long fsize;
}Files;
int main(int argc, char* argv[]) {
	int retval;
	long long send_datasize = 0;
	//명령행 인수가 있으면 IP 주소로 이동
	//if (argc > 1) SERVERIP = argv[1];

	printf("서버의 IP 입력  에시)127.0.0.1     :");
	scanf("%s", &SERVERIP);
	printf("\n");

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//connect()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	while (1) {
		SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET) err_quit("socket()");

		retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) err_quit("connect()");
		FILE* fp;
		Files f;
		char buf[BUFSIZE];
		send_datasize = 0;
		printf("보낼 파일 이름 입력\n");
		scanf_s("%s", f.fname);
		if ((fp = fopen(f.fname, "rb")) == NULL)
			printf("파일을 읽을 수 없습니다.\n");
		//파일의 크기 계산 파일의 포인터를 끝으로 이동함
		fseek(fp, 0, SEEK_END);
		f.fsize = _ftelli64(fp);
		//계산이 끝나면 다시 돌려놔야함.
		fseek(fp, 0, SEEK_SET);
		printf("파일의 크기  = %lld\n", f.fsize);
		retval = send(sock, (char*)&f, sizeof(f), 0);

		while (BUFSIZE < f.fsize - send_datasize) {
			fread(buf, 1, BUFSIZE, fp);
			send_datasize += send(sock, buf, BUFSIZE, 0);
			if (send_datasize == SOCKET_ERROR) err_quit("send()");
			printf("전송률 = %f %%\r", (float)send_datasize / (float)f.fsize * 100);
		}

		fread(buf, 1, f.fsize - (f.fsize / BUFSIZE * BUFSIZE), fp);
		retval = send(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) err_quit("send()");
		send_datasize += f.fsize - (f.fsize / BUFSIZE * BUFSIZE);
		printf("\n 전송률 = %f %%", (float)send_datasize / (float)f.fsize * 100);
		printf("파일 %s 전송완료\n", f.fname);
		closesocket(sock);
	}
	WSACleanup();
	return 0;
}