#pragma once
#include <Windows.h>
#include <ShlObj.h>
//��TCHARתΪchar   
//*tchar��TCHAR����ָ�룬*_char��char����ָ�� 
char _char[MAX_PATH] = { 0 };
int judge = 0;//�ж��Ƿ�ȡ��
void TcharToChar(const TCHAR* tchar, char* _char)
{
	int iLength;
	//��ȡ�ֽڳ���   
	iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
	//��tcharֵ����_char    
	WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
}
void GetFile() {
    TCHAR szBuffer[MAX_PATH] = { 0 };   //���ѡ���ļ���·�� 
    BROWSEINFO bi;
    ZeroMemory(&bi, sizeof(BROWSEINFO));
    bi.hwndOwner = NULL;
    bi.pszDisplayName = szBuffer;
    bi.ulFlags = BIF_BROWSEINCLUDEFILES;
    LPITEMIDLIST idl = SHBrowseForFolder(&bi);   //��ʼѡ���ļ����ļ���
    if (NULL == idl)
    {
        return;
    }
    judge = SHGetPathFromIDList(idl, szBuffer);
    TcharToChar(szBuffer, _char);
}