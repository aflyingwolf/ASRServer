#include "StdAfx.h"
#include "SemanticResultListener.h"

SemanticResultEvent::SemanticResultEvent(void)
{
}

SemanticResultEvent::~SemanticResultEvent(void)
{
}

SementicResultTextEvent::SementicResultTextEvent(void)
{
}

SementicResultTextEvent::SementicResultTextEvent(string text):text(text)
{
}

SementicResultTextEvent::~SementicResultTextEvent(void)
{
}
string SementicResultTextEvent::getEventName()
{
	return "SementicResultTextEvent";
}

SemanticResultListener::SemanticResultListener(void)
{
}

SemanticResultListener::~SemanticResultListener(void)
{
}
