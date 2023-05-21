#pragma once
#include <Windows.h>
#include <ShlObj.h>
//将TCHAR转为char   
//*tchar是TCHAR类型指针，*_char是char类型指针 
char _char[MAX_PATH] = { 0 };
int judge = 0;//判断是否取消
void TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void GetFile() {
    TCHAR szBuffer[MAX_PATH] = { 0 };   //存放选择文件的路径 
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));
    bi.hwndOwner = NULL;
    bi.pszDisplayName = szBuffer;
    bi.ulFlags = BIF_BROWSEINCLUDEFILES;
    LPITEMIDLIST idl = SHBrowseForFolder(&bi);   //开始选择文件或文件夹
    if (NULL == idl)
    {
        return;
    }
    judge = SHGetPathFromIDList(idl, szBuffer);
    TcharToChar(szBuffer, _char);
}