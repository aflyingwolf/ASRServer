#pragma once
#include <string>
using namespace std;
class DialogAnswer
{
public:
	DialogAnswer(void);
	~DialogAnswer(void);
public:
	int ret;  //�Ի�����ֵ��0���ɹ�������ʧ��
	string answer;
	string path;
	int parentId;
};
