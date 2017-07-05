#include "stdafx.h"
#include "Help.h"


Help::Help(void)
{
}


Help::~Help(void)
{
}

/******************************************************************************
*  函数名  : bool getcurrenttime(char * pFormatType,char * pTime,int nLen) 
*  描  述  : 得到当前时间szFormatType:日志格式字符串，例如%Y%m%d%H%M%S,具体查看CSDN中strftime
*            输出参数pTime得到日志格式字符串,nLen表示pTime的字符串的长度
******************************************************************************/
bool Help::getcurrenttime(char * pFormatType,char * pTime,int nLen) 
{
	try
	{
		time_t   tt;
		struct tm  *today;
		time(&tt);
		today = localtime( &(tt) );
		memset(pTime,0,nLen);
		strftime(pTime,nLen-1,pFormatType, today);
		return true;
	}
	catch(...)
	{
		return false;
	}
	return false;
}
 /***************************************************************************
 *  函数名  :  int Substr(char* dest,char* src,int post)
 *  描  述  :  根据空格取子串，post是空格位置
 *  返  回  :      0 : error,
 *              其他 : 拷贝长度
 ***************************************************************************/
int Help::Substr(char* dest,char* src,int post,char split)
{
	char *tmp = NULL;
	int  i,len;
	try{	
		len =strlen(src);
		if ((!len)|| (post>= len)) return 0;  //取的长度比原来长度长
    
		i = 0;
		tmp = src;
		while (*tmp !=split){    //0x20是空格，这找相应的空格
			if (i == post-1){
				break;
			}
			if (*(tmp+1) ==split){
				i++;
				tmp = tmp+2;
				continue;
			}
			tmp++;
		}
		while (*tmp == split )  tmp++; //是空格往下挪
		
		i = 0;
		while (*tmp != split){  //开始取该字符长度了
			dest[i] = *tmp ;
			if (*(tmp+1) == 0x0 || *(tmp+1) == split ||*(tmp+1) =='\n' ){
					dest[i+1] = '\0';
					i++;
					break;
			}
			i++;
			tmp++;
		}
		tmp = NULL;
	}
	catch(...){
		return 0;
	}
	return i;
}
bool Help::isPhone(const char * str)
{
	if(strlen(str)>=6) return true;
	else return false;
}

/***************************************************************************
*  函数名  :  int copyfile(char* src,char* dest);
*  描  述  :  src 是原文件,dest是目标文件
*	返  回  :   0失败，1成功
***************************************************************************/
int Help::copyfile(char* src,char* dest)
{
	FILE *hsrc, *hdest;
	unsigned *hFileBuffer = NULL;
	LONG file_len;

	try{
		if ((hsrc= fopen(src,"rb")) == NULL) {	
			  return 0;
		} 
		fseek(hsrc,0,SEEK_END);
		file_len = ftell(hsrc);
		if (file_len <= 0) { fclose(hsrc); return 0;	}

		rewind(hsrc);
		hFileBuffer = (unsigned *)malloc(sizeof(char)*file_len);
		
		if (hFileBuffer==NULL) { 
			fclose(hsrc); 
			return 0;
		}
		
		fread(hFileBuffer,file_len,1,hsrc);
		fclose(hsrc);
		if ((hdest = fopen(dest,"wb+")) == NULL){ 
			free(hFileBuffer);
			return 0;
		}
		
		fseek(hdest,0,SEEK_END);
		fwrite(hFileBuffer,file_len,1,hdest);
		free(hFileBuffer);
		fclose(hdest);
		return 1;
	}
	catch(...){
		return 0;
	}
	return 1;
}