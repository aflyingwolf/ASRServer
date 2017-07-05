#pragma once
#include <string>
using namespace std;
class FrameConst
{
public:
	FrameConst(void);
	~FrameConst(void);
public:
	static const string CONNECT_TEST_REQ;
	static const string CONNECT_TEST_RSP;
	static const string TTS_REQ;
	static const string TTS_RSP;
};

