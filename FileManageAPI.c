#include <stdlib.h>
#include <string.h>
//#include <varargs.h>
#include <Windows.h>
#include "FileManageAPI.h"

//0:初始值、1:已初始化、-1:錯誤
static int gInitialFlag = 0;

static struct MyFile gMyFile[eFM_TotalNum];

int FM_InitialTool(){
	if(gInitialFlag != 0){
		return gInitialFlag;
	}
	for(int i = 0; i < eFM_TotalNum; ++i){
		gMyFile[i].fp = NULL;
		gMyFile[i].flag = 0;
		gMyFile[i].fileName = NULL;
		memset(gMyFile[i].fpType, 0, 4);
	}
	gInitialFlag = 1;
	return gInitialFlag;
}

int FM_DisposeTool(){
	if(gInitialFlag != 1){
		return gInitialFlag;
	}
	for(int i = 0; i < eFM_TotalNum; ++i){
		if(gMyFile[i].flag == 1 && gMyFile[i].fp){
			fclose(gMyFile[i].fp);
			free(gMyFile[i].fileName);
		}
		gMyFile[i].fp = NULL;
		gMyFile[i].flag = 0;
		gMyFile[i].fileName = NULL;
		memset(gMyFile[i].fpType, 0, 4);
	}
	gInitialFlag = 0;
	return gInitialFlag;
}


int FM_Fopen(int enumNum, const char* fileName, const char* mode){
	//enumNum不正確
	if(enumNum < 0 || enumNum >= eFM_TotalNum){
		return -1;
	}
	//這個enum不是空的
	if(gMyFile[enumNum].flag == 1 && gMyFile[enumNum].fp){
		return -2;
	}
	//fopen失敗
	if((gMyFile[enumNum].fp = fopen(fileName, mode)) == NULL){
		return -3;
	}

	//若fopen成功
	gMyFile[enumNum].flag = 1;
	strcpy(gMyFile[enumNum].fpType, mode);
	gMyFile[enumNum].fileName = (char*)malloc(sizeof(char)*(strlen(fileName) + 1));
	strcpy(gMyFile[enumNum].fileName, fileName);
	
	return gMyFile[enumNum].flag;
}

int FM_Fclose(int enumNum){
	//enumNum不正確
	if(enumNum < 0 || enumNum >= eFM_TotalNum){
		return -1;
	}
	//這個enum沒東西或異常
	if(gMyFile[enumNum].flag != 1){
		return -2;
	}
	//釋放動態記憶體
	fclose(gMyFile[enumNum].fp);
	free(gMyFile[enumNum].fileName);

	//初始化參數
	gMyFile[enumNum].fp = NULL;
	gMyFile[enumNum].flag = 0;
	gMyFile[enumNum].fileName = NULL;
	memset(gMyFile[enumNum].fpType, 0, 4);
	
	return gMyFile[enumNum].flag;	
}

//size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
int FM_Fread(void* ptr, size_t size, size_t count, int enumNum){
	//enumNum不正確
	if(enumNum < 0 || enumNum >= eFM_TotalNum){
		return -1;
	}
	//這個enum沒東西或異常
	if(gMyFile[enumNum].flag != 1){
		return -2;
	}
	return fread(ptr, size, count, gMyFile[enumNum].fp);
}

//size_t fwrite ( const void * ptr, size_t size, size_t count, FILE * stream );
int FM_Fwrite(const void * ptr, size_t size, size_t count, int enumNum){
	//enumNum不正確
	int rv;
	if(enumNum < 0 || enumNum >= eFM_TotalNum){
		return -1;
	}
	//這個enum沒東西或異常
	if(gMyFile[enumNum].flag != 1){
		return -2;
	}
	rv = fwrite(ptr, size, count, gMyFile[enumNum].fp);
	fflush(gMyFile[enumNum].fp);
	return rv;
}

//int fprintf ( FILE * stream, const char * format, ... );
int _FM_Fprintf(int enumNum, const char* format, ...){
	va_list args;
	int rv;
	//enumNum不正確
	if(enumNum < 0 || enumNum >= eFM_TotalNum){
		return -1;
	}
	//這個enum沒東西或異常
	if(gMyFile[enumNum].flag != 1){
		return -2;
	}

	va_start (args, format);
	rv = vfprintf (gMyFile[enumNum].fp, format, args);
	va_end (args);
	fflush(gMyFile[enumNum].fp);
	return rv;
}
