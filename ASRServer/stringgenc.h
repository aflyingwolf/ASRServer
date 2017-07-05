#pragma once

#ifndef _STRING_GEN_C
#define _STRING_GEN_C
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;
struct split_e
{
  enum empties_t { empties_ok, no_empties };
};

template <typename Container>
Container& split(
  Container&                            result,
  const typename Container::value_type& s,
  const typename Container::value_type& delimiters,
  split_e::empties_t                      empties = split_e::empties_ok )
{
  result.clear();
  size_t current;
  size_t next = -1;
  do
  {
    if (empties == split_e::no_empties)
    {
      next = s.find_first_not_of( delimiters, next + 1 );
      if (next == Container::value_type::npos) break;
      next -= 1;
    }
    current = next + 1;
    next = s.find_first_of( delimiters, current );
    result.push_back( s.substr( current, next - current ) );
  }
  while (next != Container::value_type::npos);
  return result;
}
template <typename Container>
Container& split(
  Container&                            result,
  const typename Container::value_type& s,
  const typename Container::value_type& delimiters,
  int maxSplitNum,
  split_e::empties_t                      empties = split_e::empties_ok )
{
  result.clear();
  size_t current;
  size_t next = -1;
  int currSplitNum=0;
  do
  {
	if(currSplitNum>=maxSplitNum) return result;
	if(currSplitNum==maxSplitNum-1)
	{
		result.push_back(s.substr( next+1));
		break;
	}
    if (empties == split_e::no_empties)
    {
      next = s.find_first_not_of( delimiters, next + 1 );
      if (next == Container::value_type::npos) break;
      next -= 1;
    }
    current = next + 1;
    next = s.find_first_of( delimiters, current );
    result.push_back( s.substr( current, next - current ) );
	currSplitNum++;
  }
  while (next != Container::value_type::npos);
  return result;
}
string int2str(int n);
string float2str(double f);
template<class Type>
string tostring(Type a)
{
    stringstream ss;
    string s;
    ss << a;
    ss >> s;
}
int str2int(string& s);
double str2float(string& s);
template<class ToType,class FromType>
ToType strconvert(FromType t)
{
    stringstream ss;
    ToType a;
    ss << t;
    ss >> a;
    return a;
}

string& strtoupper(string& s);
string strtoupper(string s);
string& strtolower(string& s);
string strtolower(string s);
CString Char2CString(char * srcstr);
char * CString2Char(CString srcstr);
#endif