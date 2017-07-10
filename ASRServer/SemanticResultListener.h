#pragma once
#include <string>
#include "Client.h"
#include "FrameASRReq.h"
#include "ClientASRDataReq.h"
using namespace std;

class SemanticResultListener;
class SemanticResultEvent
{
public:
	SemanticResultEvent(void);
	virtual ~SemanticResultEvent(void);
public:
	virtual string getEventName()=0;
public:
	SemanticResultListener * pListener;//ָ��һ��ָ�룬���ָ����������¼������߱���
};


class SementicResultTextEvent : public SemanticResultEvent
{
public:
	SementicResultTextEvent(void);
	SementicResultTextEvent(string text);
	~SementicResultTextEvent(void);
public:
	string getEventName();
public:
	string text;
};

class SemanticResultListener
{
public:
	SemanticResultListener(void);
	virtual ~SemanticResultListener(void);
public:
	ClientASRDataReq req;
public:
	virtual void onEvent(SemanticResultEvent * pEvent)=0;
};

typedef SemanticResultListener ISemanticResultListener;