/*
  KeePass Password Safe - The Open-Source Password Manager
  Copyright (C) 2003-2007 Dominik Reichl <dominik.reichl@t-online.de>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "StdAfx.h"
#include <math.h>
#include <map>

#include "PwUtil.h"
#include "MemUtil.h"
#include "TranslateEx.h"

#define CHARSPACE_ESCAPE      60
#define CHARSPACE_ALPHA       26
#define CHARSPACE_NUMBER      10
#define CHARSPACE_SIMPSPECIAL 16
#define CHARSPACE_EXTSPECIAL  17
#define CHARSPACE_HIGH       112

// Very simple password quality estimation function
DWORD EstimatePasswordBits(LPCTSTR pszPassword)
{
	DWORD i, dwLen, dwCharSpace, dwBits;
	BOOL bChLower = FALSE, bChUpper = FALSE, bChNumber = FALSE;
	BOOL bChSimpleSpecial = FALSE, bChExtSpecial = FALSE, bChHigh = FALSE;
	BOOL bChEscape = FALSE;
	TCHAR tch;
	double dblBitsPerChar, dblEffectiveLength = 0.0;
	std::map<TCHAR, unsigned int> vCharCounts;
	std::map<int, unsigned int> vDifferences;

	ASSERT(pszPassword != NULL); if(pszPassword == NULL) return 0;

	dwLen = (DWORD)_tcslen(pszPassword);
	if(dwLen == 0) return 0; // Zero bits of information :)

	for(i = 0; i < dwLen; i++) // Get character types
	{
		tch = pszPassword[i];

		if(tch < _T(' ')) bChEscape = TRUE;
		if((tch >= _T('A')) && (tch <= _T('Z'))) bChUpper = TRUE;
		if((tch >= _T('a')) && (tch <= _T('z'))) bChLower = TRUE;
		if((tch >= _T('0')) && (tch <= _T('9'))) bChNumber = TRUE;
		if((tch >= _T(' ')) && (tch <= _T('/'))) bChSimpleSpecial = TRUE;
		if((tch >= _T(':')) && (tch <= _T('@'))) bChExtSpecial = TRUE;
		if((tch >= _T('[')) && (tch <= _T('`'))) bChExtSpecial = TRUE;
		if((tch >= _T('{')) && (tch <= _T('~'))) bChExtSpecial = TRUE;
		if(tch > _T('~')) bChHigh = TRUE;

		double dblDiffFactor = 1.0;
		if(i >= 1)
		{
			int iDiff = (int)tch - (int)pszPassword[i - 1];

			if(vDifferences.find(iDiff) == vDifferences.end())
				vDifferences[iDiff] = 1;
			else
			{
				vDifferences[iDiff] = vDifferences[iDiff] + 1;
				dblDiffFactor /= (double)vDifferences[iDiff];
			}
		}

		if(vCharCounts.find(tch) == vCharCounts.end())
		{
			vCharCounts[tch] = 1;
			dblEffectiveLength += dblDiffFactor;
		}
		else
		{
			vCharCounts[tch] = vCharCounts[tch] + 1;
			dblEffectiveLength += dblDiffFactor * (1.0 / (double)vCharCounts[tch]);
		}
	}

	dwCharSpace = 0;
	if(bChEscape == TRUE) dwCharSpace += CHARSPACE_ESCAPE;
	if(bChUpper == TRUE) dwCharSpace += CHARSPACE_ALPHA;
	if(bChLower == TRUE) dwCharSpace += CHARSPACE_ALPHA;
	if(bChNumber == TRUE) dwCharSpace += CHARSPACE_NUMBER;
	if(bChSimpleSpecial == TRUE) dwCharSpace += CHARSPACE_SIMPSPECIAL;
	if(bChExtSpecial == TRUE) dwCharSpace += CHARSPACE_EXTSPECIAL;
	if(bChHigh == TRUE) dwCharSpace += CHARSPACE_HIGH;

	ASSERT(dwCharSpace != 0); if(dwCharSpace == 0) return 0;

	dblBitsPerChar = log((double)dwCharSpace) / log(2.00);
	dwBits = (DWORD)(ceil(dblBitsPerChar * dblEffectiveLength));

	ASSERT(dwBits != 0);
	return dwBits;
}

BOOL LoadHexKey32(FILE *fp, BYTE *pBuf)
{
	char buf[65], ch1, ch2;
	BYTE bt;
	int i;

	ASSERT(fp != NULL); if(fp == NULL) return FALSE;
	ASSERT(pBuf != NULL); if(pBuf == NULL) return FALSE;

	buf[64] = 0;
	if(fread(buf, 1, 64, fp) != 64) { ASSERT(FALSE); return FALSE; }

	for(i = 0; i < 32; i++)
	{
		ch1 = buf[i * 2];
		ch2 = buf[i * 2 + 1];

		if(ConvertStrToHexEx(ch1, ch2, bt) == FALSE) return FALSE;

		pBuf[i] = bt;
	}

	mem_erase((BYTE *)buf, 64);
	return TRUE;
}

BOOL SaveHexKey32(FILE *fp, BYTE *pBuf)
{
	char buf[65], ch1, ch2;
	int i;
	BYTE bt;

	ASSERT(fp != NULL); if(fp == NULL) return FALSE;
	ASSERT(pBuf != NULL); if(pBuf == NULL) return FALSE;

	buf[64] = 0;

	for(i = 0; i < 32; i++)
	{
		bt = pBuf[i];

		ConvertHexToStrEx(bt, ch1, ch2);

		buf[i * 2] = ch1;
		buf[i * 2 + 1] = ch2;
	}

	fwrite(buf, 1, 64, fp);

	mem_erase((BYTE *)buf, 64);
	return TRUE;
}

BOOL ConvertStrToHexEx(char ch1, char ch2, BYTE& bt)
{
	if((ch1 >= '0') && (ch1 <= '9')) bt = (BYTE)(ch1 - '0');
	else if((ch1 >= 'a') && (ch1 <= 'f')) bt = (BYTE)(ch1 - 'a' + 10);
	else if((ch1 >= 'A') && (ch1 <= 'F')) bt = (BYTE)(ch1 - 'A' + 10);
	else return FALSE;

	bt <<= 4;

	if((ch2 >= '0') && (ch2 <= '9')) bt |= (BYTE)(ch2 - '0');
	else if((ch2 >= 'a') && (ch2 <= 'f')) bt |= (BYTE)(ch2 - 'a' + 10);
	else if((ch2 >= 'A') && (ch2 <= 'F')) bt |= (BYTE)(ch2 - 'A' + 10);
	else return FALSE;

	return TRUE;
}

void ConvertHexToStrEx(BYTE bt, char& ch1, char& ch2)
{
	char chq = (char)(bt >> 4);
	if(chq < 10) ch1 = (char)(chq + '0');
	else ch1 = (char)(chq - 10 + 'a');

	chq = (char)(bt & 0x0F);
	if(chq < 10) ch2 = (char)(chq + '0');
	else ch2 = (char)(chq - 10 + 'a');
}

CString PWM_FormatStaticError(int nErrorCode, DWORD dwFlags)
{
	CString str;

	if(nErrorCode != PWE_SUCCESS)
	{
		TCHAR tszTemp[24];
		_stprintf_s(tszTemp, _countof(tszTemp), _T("%08X"),
			(unsigned int)nErrorCode);

		if((dwFlags & PWFF_NO_INTRO) == 0)
		{
			str = TRL("An error occured"); str += _T("!\r\n");
		}

		str += TRL("Error code"); str += _T(": 0x");
		str += tszTemp;

		if((dwFlags & PWFF_NO_INTRO) == 0) str += _T("\r\n\r\n");
		else str += _T("\r\n");
	}

	switch(nErrorCode)
	{
	case PWE_UNKNOWN:
		str += TRL("Unknown error");
		break;
	case PWE_SUCCESS:
		str += TRL("Success");
		break;
	case PWE_INVALID_PARAM:
		str += TRL("Invalid parameter");
		break;
	case PWE_NO_MEM:
		str += TRL("Too few memory (RAM) available");
		break;
	case PWE_INVALID_KEY:
		str += TRL("Invalid/wrong key");
		break;
	case PWE_NOFILEACCESS_READ:
		str += TRL("File access error: failed to open file in read mode");
		break;
	case PWE_NOFILEACCESS_WRITE:
		str += TRL("File access error: failed to open file in write mode");
		break;
	case PWE_FILEERROR_READ:
		str += TRL("File error: error while reading from the file");
		break;
	case PWE_FILEERROR_WRITE:
		str += TRL("File error: error while writing to the file");
		break;
	case PWE_INVALID_RANDOMSOURCE:
		str += TRL("Internal error"); str += _T(": ");
		str += TRL("Invalid random source");
		break;
	case PWE_INVALID_FILESTRUCTURE:
		str += TRL("Invalid/corrupted file structure");
		break;
	case PWE_CRYPT_ERROR:
		str += TRL("Encryption/decryption error");
		break;
	case PWE_INVALID_FILESIZE:
		str += TRL("Invalid/corrupted file structure");
		break;
	case PWE_INVALID_FILESIGNATURE:
		str += TRL("Invalid/corrupted file structure");
		break;
	case PWE_INVALID_FILEHEADER:
		str += TRL("Invalid/corrupted file structure");
		break;
	case PWE_NOFILEACCESS_READ_KEY:
		str += TRL("File access error: failed to open file in read mode");
		break;
	default:
		ASSERT(FALSE);
		str += TRL("Unknown error");
		break;
	}
	str += _T(".");

	if(nErrorCode == PWE_NOFILEACCESS_READ_KEY)
	{
		str += _T("\r\n\r\n");
		str += TRL("Make sure the media that contains the key file is inserted and that KeePass has the rights to access it (check file access rights, ensure that no other application is blocking the file, ...).");
	}

	return str;
}

/* CPwErrorInfo::CPwErrorInfo()
{
	m_nPwErrorCode = PWE_SUCCESS;
	m_dwLastError = 0;
}

void CPwErrorInfo::SetInfo(const std::basic_string<TCHAR>& strText,
	int nPwErrorCode, BOOL bUseLastError)
{
	m_strText = strText;
	m_nPwErrorCode = nPwErrorCode;

	if(bUseLastError == TRUE) m_dwLastError = GetLastError();
	else m_dwLastError = 0;

	this->FormatInfo();
}

void CPwErrorInfo::FormatInfo()
{
	std::basic_string<TCHAR> str;

	if(m_strText.size() > 0)
	{
		str += m_strText;
		str += _T("\r\n\r\n");
	}

	str += PWM_FormatStaticError(m_nPwErrorCode, 0);
	str += _T("\r\n\r\n");

	if(m_dwLastError != 0)
	{
		LPTSTR lpBuffer = NULL;

		if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, m_dwLastError, 0, (LPTSTR)&lpBuffer, 1, NULL) != 0)
		{
			str += lpBuffer;
		}

		if(lpBuffer != NULL) { LocalFree(lpBuffer); lpBuffer = NULL; }
	}

	m_strFinal = str;
}

std::basic_string<TCHAR> CPwErrorInfo::ToString() const
{
	return m_strFinal;
}
*/
