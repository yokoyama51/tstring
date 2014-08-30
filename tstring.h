

/*! @file
 * @brief	�ėp�������`����уw���p�֐���`�t�@�C��
 */

#ifndef __YOKOYAMA51_TSTRING_H__
#define __YOKOYAMA51_TSTRING_H__


#include <string>
#include <locale>
#include <vector>



namespace std
{
/**
 * @typedef	std::basic_string<char>	tstring
 * @brief	�R���p�C���I�v�V�����ɂ���Ĉȉ��ɕϊ������<br>
 *			Unicode: std::wstring<br>
 *			MBCS: std::string
 */
#ifdef _UNICODE
	typedef std::basic_string<wchar_t>	tstring;
#else
	typedef std::basic_string<char>		tstring;	
#endif
}

#ifdef _UNICODE
typedef wchar_t				tchar;
#define	StringIComp			StringICompW
#define StringSplit			StringSplitW
#define _T2W(dst, src)		(dst = src)
#define _T2A(dst, src)		_W2A(dst, src)
#define _W2T(dst, src)		(dst = src)
#define _A2T(dst, src)		_A2W(dst, src)
#else
typedef char				tchar;			///< �R���p�C���I�v�V�����ɂ���Ĉȉ��ɕϊ������ _TCHAR�Ɠ���<br> Unicode:wchar_t<br>	MBCS:�@�@char
#define	StringIComp			StringICompA	///< �R���p�C���I�v�V�����ɂ���Ĉȉ��ɕϊ������ <br> Unicode:StringICompW<br>	MBCS:�@�@StringICompA
#define StringSplit			StringSplitA	///< �R���p�C���I�v�V�����ɂ���Ĉȉ��ɕϊ������ <br> Unicode:StringSplitW<br>	MBCS:�@�@StringSplitA
#define _T2W(dst, src)		_A2W(dst, src)	///< tstring��wstring�ϊ�����
#define _T2A(dst, src)		(dst = src)		///< tstring��string�ϊ�����
#define _W2T(dst, src)		_W2A(dst, src)	///< wstring��tstring�ϊ�����
#define _A2T(dst, src)		(dst = src)		///< string��tstring�ϊ�����
#endif

#define _A2W(dst, src)	ToWideString(dst, src)	///< string��wstring�ϊ�����
#define _W2A(dst, src)	ToMultString(dst, src)	///< wstring��string�ϊ�����


/* �����t�t�H�[�}�b�g */
size_t StringFormatW(std::wstring &out, const wchar_t * format, ...);
size_t StringFormatA(std::string &out, const char * format, ...);

/* wstring �� string�̑��ݕϊ� */
void ToWideString(std::wstring &dst, const std::string &src);
void ToWideString(std::wstring &dst, const std::string &src, const std::locale& loc);
void ToWideString(std::wstring &dst, const std::string &src, const std::codecvt<std::wstring::value_type, std::string::value_type, std::mbstate_t>& cvt);
void ToMultString(std::string &dst, const std::wstring &src);
void ToMultString(std::string &dst, const std::wstring &src, const std::locale& loc);
void ToMultString(std::string &dst, const std::wstring &src, const std::codecvt<std::wstring::value_type, std::string::value_type, std::mbstate_t>& cvt);

/* �������r */
int StringICompW(const std::wstring &str1, const std::wstring &str2);
int StringICompA(const std::string  &str1, const std::string  &str2);

/* �����񕪊� */
size_t StringSplitW(std::vector<std::wstring> &out, const std::wstring &str, const wchar_t * delim, const wchar_t * quote = L"", const wchar_t * esc = L"");
size_t StringSplitA(std::vector<std::string>  &out, const std::string  &str, const char *    delim, const char * quote = "", const char * esc = "");

#endif /* __YOKOYAMA51_TSTRING_H__ */