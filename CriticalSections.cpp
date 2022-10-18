#include<Windows.h>
#include<stdio.h>

#define MAXCNT 10000000
int count = 0;

DWORD WINAPI MyThread1(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; ++i) {
		count += 2;
	}
	return 0;
}

DWORD WINAPI MyThread2(LPVOID arg) {
	for (int i = 0; i < MAXCNT; ++i) {
		count -= 2;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	printf("count = %d\n", count);
	return 0;
}