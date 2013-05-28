/*
 * strhelpe implementation file
 * Author: Sun Junwen
 * Version: 1.1.1
 * Provides converting from tstring, string and wstring to each other
 * And provides string's utf8 converting.
 * Provides triming function to string and wstring.
 * Provides replacing function to string and wstring.
 * Provides newline fixing to string.
 */
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <clocale>
#include <locale>
#include <vector>

#include <iconv.h>
#include <errno.h>

#include "strhelper.h"

/*
 * 将指定字符集的 std::string 转换为 utf-8 的 std::string
 */
std::string sunjwbase::strtoutf8(const std::string& charset,
		const std::string& str)
{
	char* inptr = new char[str.size() + 1];
	size_t inleft = str.size();
	size_t outleft = inleft * 4;
	char* outptr = new char[outleft];
	bzero(outptr, outleft);

	strcpy(inptr, str.c_str());

	iconv_t cd; /* conversion descriptor */
	if ((cd = iconv_open("UTF-8", charset.c_str())) == (iconv_t) (-1))
	{
		return str;
	}

	char* in = inptr;
	char* out = outptr;
	outleft = iconv(cd, &in, &inleft, &out, &outleft);

	iconv_close(cd);

	std::string strRet(outptr);
	delete[] inptr;
	delete[] outptr;

	return strRet;
}

/*
 * 将 utf-8 的 std::string 转换为 gbk
 */
std::string sunjwbase::strutf8tougbk(const std::string& str)
{
	char* inptr = new char[str.size() + 1];
	size_t inleft = str.size();
	size_t outleft = inleft + 1;
	char* outptr = new char[outleft];
	bzero(outptr, outleft);

	strcpy(inptr, str.c_str());

	iconv_t cd; /* conversion descriptor */
	if ((cd = iconv_open("GBK", "UTF-8")) == (iconv_t) (-1))
	{
		return str;
	}

	char* in = inptr;
	char* out = outptr;
	outleft = iconv(cd, &in, &inleft, &out, &outleft);

	iconv_close(cd);

	std::string strRet(outptr);
	delete[] inptr;
	delete[] outptr;

	return strRet;
}

std::string sunjwbase::wstrtostr(const std::wstring& wstr)
{
	// Convert a Unicode string to an ASCII string
	size_t num_chars = wcstombs(NULL, wstr.c_str(), 0);
	char* char_buf = new char[num_chars + 1];
	wcstombs(char_buf, wstr.c_str(), wstr.size());
	std::string str(char_buf);
	delete[] char_buf;

	return str;
}

std::wstring sunjwbase::strtowstr(const std::string& str)
{
	// Convert an ASCII string to a Unicode String
	size_t num_chars = mbstowcs(NULL, str.c_str(), 0);
	wchar_t* wct_buf = new wchar_t[num_chars + 1];
	mbstowcs(wct_buf, str.c_str(), str.size());
	std::wstring wstr(wct_buf, num_chars);
	delete[] wct_buf;

	return wstr;
}

std::string sunjwbase::strtrim_right(const std::string& s,
		const std::string& spaces)
{
	std::string d(s);
	std::string::size_type i(d.find_last_not_of(spaces));
	if (i == std::string::npos)
		return "";
	else
		return d.erase(d.find_last_not_of(spaces) + 1);
} // end of trim_right

std::string sunjwbase::strtrim_left(const std::string& s,
		const std::string& spaces)
{
	std::string d(s);
	return d.erase(0, s.find_first_not_of(spaces));
} // end of trim_left

std::wstring sunjwbase::strtrim_right(const std::wstring& s,
		const std::wstring& spaces)
{
	std::wstring d(s);
	std::wstring::size_type i(d.find_last_not_of(spaces));
	if (i == std::wstring::npos)
		return L"";
		else
		return d.erase(d.find_last_not_of(spaces) + 1);
	}

std::wstring sunjwbase::strtrim_left(const std::wstring& s,
		const std::wstring& spaces)
{
	std::wstring d(s);
	return d.erase(0, s.find_first_not_of(spaces));
}

std::string sunjwbase::strreplace(const std::string& base,
		const std::string& src, const std::string& des)
{
	std::string ret(base);
	std::string::size_type pos = 0;
	std::string::size_type srcLen = src.size();
	std::string::size_type desLen = des.size();
	pos = ret.find(src, pos);
	while ((pos != std::string::npos))
	{
		ret.replace(pos, srcLen, des);
		pos = ret.find(src, pos + desLen);
	}

	return ret;
}

std::wstring sunjwbase::strreplace(const std::wstring& base,
		const std::wstring& src, const std::wstring& des)
{
	std::wstring ret(base);
	std::wstring::size_type pos = 0;
	std::wstring::size_type srcLen = src.size();
	std::wstring::size_type desLen = des.size();
	pos = ret.find(src, pos);
	while ((pos != std::wstring::npos))
	{
		ret.replace(pos, srcLen, des);
		pos = ret.find(src, pos + desLen);
	}

	return ret;
}

std::string sunjwbase::fixnewline(const std::string& str)
{
	std::string ret;
	std::string strRepairSrc(""), strRepairDst("");
	// 修理换行符, /10->/13/10, /13/13/10->/13/10
	strRepairSrc += (char) 10;
	strRepairDst += (char) 13;
	strRepairDst += (char) 10;
	ret = strreplace(str, strRepairSrc, strRepairDst);
	strRepairSrc = (char) 13;
	strRepairSrc += (char) 13;
	strRepairSrc += (char) 10;
	ret = strreplace(ret, strRepairSrc, strRepairDst);
	// 修理换行符, /13->/13/10, /13/10/10->/13/10
	strRepairSrc = "";
	strRepairDst = "";
	strRepairSrc += (char) 13;
	strRepairDst += (char) 13;
	strRepairDst += (char) 10;
	ret = strreplace(ret, strRepairSrc, strRepairDst);
	strRepairSrc = (char) 13;
	strRepairSrc += (char) 10;
	strRepairSrc += (char) 10;
	ret = strreplace(ret, strRepairSrc, strRepairDst);
	return ret;
}

