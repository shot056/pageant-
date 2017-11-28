
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#if defined(_MSC_VER)
#include <windows.h>
#endif

#include "utf8.h"

/**
 *	@param[in]		src			soruce string 
 *	@param[in]		codePage	like CP_UTF8,CP_ACP
 *								-1�̂Ƃ��ACP_ACP�ƂȂ�
 *	@param[in,out]	*len		string length by wchar_t
 *	@retval			NULL		error
 *	@retval 		other		converted string(malloced)
 */
wchar_t *to_wcs(const char *src, int codePage, int *len)
{
	if (codePage == -1)
	{
		codePage = GetACP();
	}

	//�ϊ���̒����𓾂�B
	// �^�[�~�l�[�^�[��('\0')���܂܂��l���Ԃ��Ă���
	int DestLen = MultiByteToWideChar(
		codePage,
		0,
		src, -1,
		0, 0);
	if (DestLen <= 0)
	{	// ���s
		if (len != NULL)
			*len = 0;
		return NULL;
	}

	// �m��
	wchar_t *wcs = (wchar_t *)malloc(DestLen * sizeof(wchar_t));

	// ���ۂ̕ϊ�
	DestLen = MultiByteToWideChar(
		codePage,
		0,
		src, -1,
		wcs, DestLen);
	if (DestLen <= 0)
	{	// ���s
		if (len != NULL)
			*len = 0;
		free(wcs);
		return NULL;
	}

	if (len != NULL)
		*len = DestLen;
	return wcs;
}

char *to_string(const wchar_t *src, int codePage, int *len)
{
	if (codePage == -1)
	{
		codePage = GetACP();
	}

	int DestLen;

	//�ϊ���̒����𓾂�B
	// �^�[�~�l�[�^�[��('\0')���܂܂��l���Ԃ��Ă���
	DestLen = WideCharToMultiByte(
		codePage,
		0,
		src, -1,
		0, 0,
		NULL, NULL);
	if (DestLen <= 0)
	{	// ���s
		if (len != NULL)
			*len = 0;
		return NULL;;
	}

	// �m��
	char *mbs = (char *)malloc(DestLen*sizeof(char));

	// ���ۂ̕ϊ�
	DestLen = WideCharToMultiByte(
		codePage,
		0,
		src,-1,
		mbs, DestLen,
		NULL, NULL);
	if (DestLen <= 0)
	{	// ���s
		free(mbs);
		if (len != NULL)
			*len = 0;
		return NULL;
	}

	return mbs;
}


void to_wstring(const char *src, std::wstring &dest, int codePage)
{
	wchar_t *wcs = to_wcs(src, codePage, NULL);
	dest = wcs;
	free(wcs);
}

void to_utf8(const wchar_t *src, std::string &utf8)
{
	char *str_utf8 = to_string(src, CP_UTF8, NULL);
	utf8 = str_utf8;
	free(str_utf8);
}

void to_utf8(const char *src, std::string &utf8, int codePage)
{
	wchar_t *wcs = to_wcs(src, codePage, NULL);
	to_utf8(wcs, utf8);
	free(wcs);
}
