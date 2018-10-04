#pragma once
#include <stdio.h>

#define FM_Fprintf(num, format, ...) _FM_Fprintf(num, format, ##__VA_ARGS__)

/*
// 當想要增加fp時，enum可以自己新增，但是有幾個注意事項
// 1.eFM_TotalNum不能刪除，而且一定要擺在最後一個
// 2.enum元素不能擅自指定數字，這樣eFM_TotalNum所代表的Total Number才會正確
*/
enum{eFM_Test1, eFM_TotalNum};

struct MyFile{
	FILE* fp;
	int flag;
	char* fileName;
	char  fpType[4];
};


int FM_InitialTool();
int FM_DisposeTool();

int FM_Fopen(int enumNum, const char* fileName, const char* mode);
int FM_Fclose(int enumNum);

int FM_Fread(void* ptr, size_t size, size_t count, int enumNum);
int FM_Fwrite(const void * ptr, size_t size, size_t count, int enumNum);

int _FM_Fprintf(int enumNum, const char* format, ...);
