#pragma once
class Help
{
public:
	Help(void);
	~Help(void);
public:
	static bool getcurrenttime(char * pFormatType,char * pTime,int nLen);
	//ȡ�ִ�
	static int Substr(char* dest,char* src,int post,char split);
	//����Ƿ��Ǻ���,���жϺ��볤������6Ϊ
	static bool isPhone(const char * str);
	static int copyfile(char* src,char* dest);
};

