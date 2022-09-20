#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	//IPv4 변환
	//원래의 IPv4 주소 출력
	const char* ipv4test = "147.46.114.70";
	printf("IPv4 주소(변환 전) = %s\n", ipv4test);

	//inet_pton() 함수 연습
	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("IPv4 주소(변환 후) = %#x\n", ipv4num.s_addr);

	//inet_ntop() 함수 연습
	char ipv4str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("IPv4 주소(다시 변환 후) = %s\n\n", ipv4str);


	//ipv6 변환
	const char* ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("IPv6 주소( 변환 전) = %s\n", ipv6test);

	//inet_pton() 함수 연습
	struct in6_addr ipv6num;
	inet_pton(AF_INET6, ipv6test, &ipv6num);
	printf("IPv6 주소(변환 후) = 0x");
	for (int i = 0; i < 16; ++i)
		printf("%02x", ipv6num.s6_addr[i]);
	printf("\n");

	//inet_ntop() 함수 연습
	char ipv6str[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &ipv6num, ipv6str, sizeof(ipv6str));
	printf("IPv6 주소(다시 변환 후) = %s\n", ipv6str);

	WSACleanup();
	return 0;
}