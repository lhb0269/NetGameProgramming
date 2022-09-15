#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

bool IsLittleEndian();
bool IsBigEndian();

int main(int argc, char* argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	u_short x1 = 0x1234;
	u_long  y1 = 0x12345678;
	u_short x2 = htons(x1);
	u_long  y2;

	//// 호스트 바이트 -> 네트워크 바이트
	//printf("[호스트 바이트 -> 네트워크 바이트]\n");
	//printf("%#x -> %#x\n", x1, x2 = htons(x1));
	//printf("%#x -> %#x\n", y1, y2 = htonl(y1));

	//// 네트워크 바이트 -> 호스트 바이트
	//printf("\n[네트워크 바이트 -> 호스트 바이트]\n");
	//printf("%#x -> %#x\n", x2, ntohs(x2));
	//printf("%#x -> %#x\n", y2, ntohl(y2));

	//// 잘못된 예
	//printf("\n[잘못된 사용의 예]\n");
	//printf("%#x -> %#x\n", x1, htonl(x1));

	//printf("%d",IsBigEndian());
	// 윈속 종료
	printf("리틀엔디언? : %d\n", IsLittleEndian());
	printf("빅엔디언? : %d\n", IsBigEndian());
	WSACleanup();
	return 0;
}

//n to h
//big to 호슽의 정렬방식
//h to n
//big to 상수
bool IsLittleEndian() {
	u_short x = 0x1234;
	u_short x2 = htons(x);
	if (x == x2)
		return false;
	return true;
}
bool IsBigEndian() {
	u_short x = 0x1234;
	u_short x2 = htons(x);
	if (x == x2)
		return true;
	return false;
}