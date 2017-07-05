#include "stdafx.h"
#include "Help.h"


Help::Help(void)
{
}


Help::~Help(void)
{
}

/******************************************************************************
*  ������  : bool getcurrenttime(char * pFormatType,char * pTime,int nLen) 
*  ��  ��  : �õ���ǰʱ��szFormatType:��־��ʽ�ַ���������%Y%m%d%H%M%S,����鿴CSDN��strftime
*            �������pTime�õ���־��ʽ�ַ���,nLen��ʾpTime���ַ����ĳ���
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
 *  ������  :  int Substr(char* dest,char* src,int post)
 *  ��  ��  :  ���ݿո�ȡ�Ӵ���post�ǿո�λ��
 *  ��  ��  :      0 : error,
 *              ���� : ��������
 ***************************************************************************/
int Help::Substr(char* dest,char* src,int post,char split)
{
	char *tmp = NULL;
	int  i,len;
	try{	
		len =strlen(src);
		if ((!len)|| (post>= len)) return 0;  //ȡ�ĳ��ȱ�ԭ�����ȳ�
    
		i = 0;
		tmp = src;
		while (*tmp !=split){    //0x20�ǿո�������Ӧ�Ŀո�
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
		while (*tmp == split )  tmp++; //�ǿո�����Ų
		
		i = 0;
		while (*tmp != split){  //��ʼȡ���ַ�������
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
*  ������  :  int copyfile(char* src,char* dest);
*  ��  ��  :  src ��ԭ�ļ�,dest��Ŀ���ļ�
*	��  ��  :   0ʧ�ܣ�1�ɹ�
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