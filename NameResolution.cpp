#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define TESTNAME "www.naver.com"


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

// 도메인이름 -> IPv4 주소
bool GetIPAddr(const char* name, struct in_addr* addr)
{
	struct hostent* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}
	printf("도메인 이름 = %s\n", name);
	int i = 0;
	while (ptr->h_aliases[i] != NULL) {
		printf("별칭 이름 = %s\n", ptr->h_aliases[i]);
		++i;
	}  
	printf("\n");
	i = 0;
	while (ptr->h_addr_list[i] != NULL) {
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET,ptr->h_addr_list[i], str, sizeof(str));
		printf("IP주소 = %s\n", str);
		printf("IP주소 = %s\n", inet_ntoa(*(IN_ADDR*)ptr->h_addr_list[i]));
		++i;
	}
	if (ptr->h_addrtype != AF_INET)
		return false;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return true;
}

//// IPv4 주소 -> 도메인 이름
//bool GetDomainName(struct in_addr addr, char* name, int namelen)
//{
//	struct hostent* ptr = gethostbyaddr((const char*)&addr,
//		sizeof(addr), AF_INET);
//	if (ptr == NULL) {
//		err_display("gethostbyaddr()");
//		return false;
//	}
//	if (ptr->h_addrtype != AF_INET)
//		return false;
//	strncpy(name, ptr->h_name, namelen);
//	return true;
//}

int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	while (1) {
		char name[50];
		printf("도메인을 입력하시오 : ");
		gets_s(name);
		printf("\n");
		struct in_addr addr;
		GetIPAddr(name, &addr);
		printf("\n");
	}
	WSACleanup();
	return 0;
}