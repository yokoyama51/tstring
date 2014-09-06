

/*! @file
 * @brief	汎用文字列定義およびヘルパ関数定義ファイル
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
 * @brief	コンパイルオプションによって以下に変換される<br>
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
typedef char				tchar;			///< コンパイルオプションによって以下に変換される _TCHARと同じ<br> Unicode:wchar_t<br>	MBCS:　　char
#define	StringIComp			StringICompA	///< コンパイルオプションによって以下に変換される <br> Unicode:StringICompW<br>	MBCS:　　StringICompA
#define StringSplit			StringSplitA	///< コンパイルオプションによって以下に変換される <br> Unicode:StringSplitW<br>	MBCS:　　StringSplitA
#define _T2W(dst, src)		_A2W(dst, src)	///< tstring→wstring変換処理
#define _T2A(dst, src)		(dst = src)		///< tstring→string変換処理
#define _W2T(dst, src)		_W2A(dst, src)	///< wstring→tstring変換処理
#define _A2T(dst, src)		(dst = src)		///< string→tstring変換処理
#endif

#define _A2W(dst, src)	ToWideString(dst, src)	///< string→wstring変換処理
#define _W2A(dst, src)	ToMultString(dst, src)	///< wstring→string変換処理


/* wstring ⇔ stringの相互変換 */
void ToWideString(std::wstring &dst, const std::string &src);
void ToWideString(std::wstring &dst, const std::string &src, const std::locale& loc);
void ToWideString(std::wstring &dst, const std::string &src, const std::codecvt<std::wstring::value_type, std::string::value_type, std::mbstate_t>& cvt);
void ToMultString(std::string &dst, const std::wstring &src);
void ToMultString(std::string &dst, const std::wstring &src, const std::locale& loc);
void ToMultString(std::string &dst, const std::wstring &src, const std::codecvt<std::wstring::value_type, std::string::value_type, std::mbstate_t>& cvt);

/* 文字列比較 */
int StringICompW(const std::wstring &str1, const std::wstring &str2);
int StringICompA(const std::string  &str1, const std::string  &str2);

/* 文字列分割 */
size_t StringSplitW(std::vector<std::wstring> &out, const std::wstring &str, const wchar_t * delim, const wchar_t * quote = L"", const wchar_t * esc = L"");
size_t StringSplitA(std::vector<std::string>  &out, const std::string  &str, const char *    delim, const char * quote = "", const char * esc = "");


/**
 * @brief	文字列結合
 *
 * @tparam	C	文字型 (char, wchar_t)
 * @tparam	V	入力コンテナ (Sequence containers. array, vector, deque, list)
 *
 * @param	dst			結果出力バッファ
 * @param	src			結合対象コンテナ <br> operator<< でstreamに文字列を送れるならばどんな要素でもOKなはず。。
 * @param	separator	結合文字
 *
 * @bug		入力コンテナが[forward_list]の場合動作しない。
 */
template <typename C, typename V>
void StringJoin(std::basic_string<C> &dst, V &src, const C * separator)
	{
		using namespace std;
		typedef std::basic_string<C>	stringT;
		typedef basic_stringstream<C, char_traits<C>, allocator<C> > stringstreamT;

		if(src.empty()) { dst = stringT(); return; }
		
		stringstreamT ss;
		for_each(src.begin(), prev(src.end()),
				 [&](int e){ ss << e << separator; });
		ss << src.back();

		dst = ss.str();
	}


#endif /* __YOKOYAMA51_TSTRING_H__ */
