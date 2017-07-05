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
	//根据请求包，生成响应包，空串则表示无响应包，或稍后响应包异步生成
	virtual string ProcData();
	//生成类的消息字符串
	virtual std::string GetFrameString();
};

