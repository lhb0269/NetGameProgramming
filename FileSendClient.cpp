//EOR 방식

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

char* SERVERIP = (char*)"127.0.0.1";
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

	//명령행 인수가 있으면 IP 주소로 이동
	if (argc > 1) SERVERIP = argv[1];

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

		long long percent = f.fsize / BUFSIZE;
		unsigned long long count = f.fsize / BUFSIZE;

		while (count) {
			fread(buf, 1, BUFSIZE, fp);
			retval = send(sock, buf, BUFSIZE, 0);
			if (retval == SOCKET_ERROR) err_quit("send()");
			printf("전송률 = %f %%\n", (float)(percent - count) * 100 / percent);
			count--;
			//system("cls");
		}
		printf("\n 전송률 = %f %%", (float)(percent - count) * 100 / percent);
		printf("파일 %s 전송완료\n", f.fname);
		count = f.fsize - (percent * BUFSIZE);
		fread(buf, 1, count, fp);
		retval = send(sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) err_quit("send()");
		closesocket(sock);
	}
	WSACleanup();
	return 0;
}