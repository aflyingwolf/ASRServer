#pragma once
#include <string>
using namespace std;
class DialogAnswer
{
public:
	DialogAnswer(void);
	~DialogAnswer(void);
public:
	int ret;  //对话返回值，0，成功，其他失败
	string answer;
	string path;
	int parentId;
};
