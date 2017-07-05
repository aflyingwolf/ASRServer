/*
 * stringenc.cpp
 * 2012-04-24
 * some useful function for string manipulations,
 * including:
 *         split,            // split string by delim(such as " .,/")
 *         int2str,        // convert int to string
 *         float2str,        // convert double to string
 *         str2int,        // convert string to int
 *         str2float,        // convert string to double
 *         strtoupper,        // all to upper case
 *         strtolower,        // all to lower case
 *         //strtocapital,        // capital first character
 *
 */


#include "stdafx.h"
#include "stringgenc.h"

/**
 * @brief split a string by delim
 *
 * @param str string to be splited
 * @param c delimiter, const char*, just like " .,/", white space, dot, comma, splash
 *
 * @return a string vector saved all the splited world
 */
/**
 * @brief convert a integer into string through stringstream
 *
 * @param n a integer
 *
 * @return the string form of n
 */
string int2str(int n)
{
    stringstream ss;
    string s;
    ss << n;
    ss >> s;
    return s;
}

string float2str(double f)
{
    stringstream ss;
    string s;
    ss << f;
    ss >> s;
    return s;
}




/**
 * @brief convert string to int by atoi
 *
 * @param s string
 *
 * @return the integer result
 */
int str2int(string& s)
{
    return atoi(s.c_str());
}

double str2float(string& s)
{
    return atof(s.c_str());
}


/**
 * @brief convert string to upper case throught transform method, also can use transform method directly
 *
 * @param s
 *
 * @return the upper case result saved still in s
 */
string& strtoupper(string& s)
{
    transform(s.begin(),s.end(),s.begin(),::toupper);
    return s;
}

/**
 * @brief convert string to upper case through toupper, which transform a char into upper case 
 *
 * @param s
 *
 * @return the upper case result string
 */
string strtoupper(string s)
{
    string t = s;
    int i = -1;
    while(t[i++])
    {
        t[i] = toupper(t[i]);
    }
    return t;
}

/**
 * @brief convert string to lower case throught transform method, also can use transform method directly
 *
 * @param s
 *
 * @return the lower case result saved still in s
 */
string& strtolower(string& s)
{
    transform(s.begin(),s.end(),s.begin(),::tolower);
    return s;
}

/**
 * @brief convert string to lower case through tolower, which transform a char into lower case 
 *
 * @param s
 *
 * @return the lower case result string
 */
string strtolower(string s)
{
    string t = s;
    int i = -1;
    while(t[i++])
    {
        t[i] = tolower(t[i]);
    }
    return t;
}
CString Char2CString(char * srcstr)
{
	USES_CONVERSION;
	return A2T(srcstr);
}
char * CString2Char(CString srcstr)
{
	return (LPTSTR)(LPCTSTR)srcstr;
}


