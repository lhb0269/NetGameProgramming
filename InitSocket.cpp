#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// ���� �Լ� ���� ��� �� ����
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

	printf("[�˸�] ���� �ʱ�ȭ ����\n");
	printf("���� : %d.%d\n�ִ��������� : %d.%d\nszDescription : %s\nszSystemStatus : %s\n", version_low, version_high, highversion_low, highversion_high, wsa.szDescription, wsa.szSystemStatus);


	//���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[�˸�] ���� ���� ����\n");

	//���ϴݱ�
	closesocket(sock);
	//���� ����
	WSACleanup();
	return 0;
}