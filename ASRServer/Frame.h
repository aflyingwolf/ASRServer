#pragma once
#include <string>
using namespace std;
class Frame
{
public:
	Frame(void);
public:
	Frame(std::string strHead,std::string strBody);
	virtual ~Frame(void);
public:
	std::string head;
	std::string body;
public:
	//�����������������Ӧ�����մ����ʾ����Ӧ�������Ժ���Ӧ���첽����
	virtual string ProcData();
	//���������Ϣ�ַ���
	virtual std::string GetFrameString();
};

