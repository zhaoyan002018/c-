#pragma once
void ErrorHandling(const char* message);
extern short HOST_PORT;
#define BUFF_SIZE 1024
//应用协议：1、先传递3个整形数字，2、传送1个char型运算符，3、返回long型结果
#define PROTOCOL_LEN 13


long col(int sub[], int len, char op);

void initWSAS();
