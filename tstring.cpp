


#include "tstring.h"


#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <cassert>
#include <algorithm>
#include <functional>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/tokenizer.hpp>

using namespace std;


/* prototype */
int yk51_CharCompareW(wchar_t c1, wchar_t c2);
int yk51_StringICompW(const wstring &s1, const wstring &s2);
int yk51_CharCompareA(char c1, char c2);
int yk51_StringICompA(const string &s1, const string &s2);


/** 
 * @brief	std::string �� std::wstring�ϊ�
 *
 * @param[out]	dst		�ϊ����ʃo�b�t�@
 * @param		src		�ϊ�����}���`�o�C�g������
 */
void ToWideString(std::wstring &dst, const std::string &src)
{
	ToWideString(dst, src, locale());
}

/** 
 * @brief	std::string �� std::wstring�ϊ�
 *
 * @param[out]	dst		�ϊ����ʃo�b�t�@
 * @param		src		�ϊ�����}���`�o�C�g������
 * @param		loc		�ϊ��Ɏg�p����locale
 */
void ToWideString(wstring &dst, const string &src, const locale& loc)
{
	typedef codecvt<wstring::value_type, string::value_type, mbstate_t> cvt_t;
	ToWideString(dst, src, use_facet<cvt_t>(loc));
}

/** 
 * @brief	std::string �� std::wstring�ϊ�
 *
 * @param[out]	dst		�ϊ����ʃo�b�t�@
 * @param		src		�ϊ�����}���`�o�C�g������
 * @param		loc		�ϊ��Ɏg�p����locale
 * @param		cvt		�ϊ��֐���񋟂���codecvt
 */
void ToWideString(wstring &dst, const string &src, const codecvt<wstring::value_type, string::value_type, mbstate_t>& cvt)
{
	typedef wstring::value_type wchar_type;
	typedef string::value_type char_type;
	typedef codecvt<wchar_type, char_type, mbstate_t> cvt_type;

	size_t len = src.length();
	wchar_type * buff = new wchar_type[len*2];
	
	const char_type* const pbegin = src.c_str();
	const char_type* const pend = pbegin + src.length();
	const char_type* pnext = pbegin;
	wchar_type* const pwbegin = &buff[0];
	wchar_type* const pwend = &buff[len*2];
	wchar_type* pwnext = pwbegin;

	dst.clear();
	mbstate_t state(0);
	for(;;){
		cvt_type::result result = cvt.in(state, pbegin, pend, pnext, pwbegin, pwend, pwnext);
		dst.append(pwbegin, pwnext - pwbegin);
		if(result == cvt_type::ok) {
			break;
		}

		assert(result == cvt_type::error);
	}

	delete[] buff;
}


/**
 * @brief	std::wstring �� std::string�ϊ�
 *
 * @param[out]	dst		�ϊ����ʃo�b�t�@
 * @param		src		�ϊ����郏�C�h������
 */
void ToMultString(string &dst, const wstring &src)
{
	ToMultString(dst, src, locale());
}

/**
 * @brief	std::wstring �� std::string�ϊ�
 *
 * @param[out]	dst		�ϊ����ʃo�b�t�@
 * @param		src		�ϊ����郏�C�h������
 * @param		loc		�ϊ��Ɏg�p����locale
 */
void ToMultString(string &dst, const wstring &src, const locale& loc)
{
	typedef wstring::value_type wchar_type;
	typedef string::value_type char_type;
	typedef codecvt<wchar_type, char_type, mbstate_t> cvt_t;
	ToMultString(dst, src, use_facet<cvt_t>(loc));
}

/**
 * @brief	std::wstring �� std::string�ϊ�
 *
 * @param[out]	dst		�ϊ����ʃo�b�t�@
 * @param		src		�ϊ����郏�C�h������
 * @param		loc		�ϊ��Ɏg�p����locale
 * @param		cvt		�ϊ��֐���񋟂���codecvt
 */
void ToMultString(string &dst, const wstring &src, const codecvt<wstring::value_type, string::value_type, mbstate_t>& cvt)
{
	typedef wstring::value_type wchar_type;
	typedef string::value_type char_type;
	typedef codecvt<wchar_type, char_type, mbstate_t> cvt_type;

	size_t len = src.length();
	char_type * buff = new char_type[len*2];

	const wchar_type* const pwbegin = src.c_str();
	const wchar_type* const pwend = pwbegin + src.length();
	const wchar_type* pwnext = pwbegin;

	char_type* const pbegin = &buff[0];
	char_type* const pend = &buff[len*2];
	char_type* pnext = pbegin;

	dst.clear();
	mbstate_t state(0);
	for(;;){
		cvt_type::result result = cvt.out(state, pwbegin, pwend, pwnext, pbegin, pend, pnext);
		dst.append(pbegin, pnext - pbegin);
		if(result == cvt_type::ok){
			break;
		}

		assert(result == cvt_type::error);
	}

	delete[] buff;
}


template <typename StrValue>
int _CharCompareT(StrValue c1, StrValue c2, boost::function1<int, StrValue> f)
{
	int lc1 = f(c1);
	int lc2 = f(c2);
	return (lc1<lc2)?-1:(lc1>lc2)?1:0;
}

template <typename Str, typename StrValue>
int _StringICompT2(const Str &lhs, const Str &rhs, boost::function1<int, StrValue> f)
{
	typedef pair<Str::const_iterator, Str::const_iterator> pairT;

	pairT p = mismatch(lhs.begin(), lhs.end(),
					   rhs.begin(),
					   not2(boost::bind(_CharCompareT<StrValue>, f)));

	if(p.first == lhs.end()){
		if(p.second == rhs.end()) return 0;
		else return -1;
	}
	return _CharCompareT(*p.first, *p.second, f);
}

template <typename Str, typename StrValue>
int _StringICompT(const Str &lhs, const Str &rhs, boost::function1<int, StrValue> f)
{
	return (lhs.size() <= rhs.size())?
		 _StringICompT2(lhs, rhs, f)
		:-_StringICompT2(rhs, lhs, f);
}


/**
 * @brief	�Q�̕�������������Ƃ��Ĕ�r����
 *
 * @param	str1, str2	��r������
 *
 * @return	str1��str2���������̏����Ŕ�r�����֌W������
 * @retval	<0	str1��str2��菬����
 * @retval	=0	str1��str2�͓�����
 * @retval	>0	str2��str1��菬����
 */
int StringICompW(const wstring &str1, const wstring &str2)
{
	return (str1.size() <= str2.size())?
		yk51_StringICompW(str1, str2):-yk51_StringICompW(str2, str1);
}

/**
 * @brief	�Q�̕�������������Ƃ��Ĕ�r����
 *
 * @param	str1, str2	��r������
 *
 * @return	str1��str2���������̏����Ŕ�r�����֌W������
 * @retval	<0	str1��str2��菬����
 * @retval	=0	str1��str2�͓�����
 * @retval	>0	str2��str1��菬����
 */
int StringICompA(const string &str1, const string &str2)
{
	return (str1.size() <= str2.size())?
		yk51_StringICompA(str1, str2):-yk51_StringICompA(str2, str1);
}



/**
 * @brief	���������؂蕶���ŕ�������
 *
 * @tparam		V			�o�̓R���e�i�^
 * @tparam		Str			������^
 * @tparam		StrValue	�����^
 *
 * @param[out]	out		�����㕶������i�[����o�b�t�@
 * @param		str		�����Ώە�����
 * @param		delim	��؂蕶��
 * @param		quote	���p������
 * @param		esc		�G�X�P�[�v����
 *
 * @return	������
 *
 * @warning	�����������out�����ɒǉ�����邽�߁A�߂�l = out.size()�Ƃ͌���Ȃ�!
 */
template <typename V, typename Str, typename StrValue>
size_t _StringSplitT(V &out, const Str &str, const StrValue * delim, const StrValue * quote, const StrValue * esc)
{
	typedef boost::escaped_list_separator<StrValue> escaped_list_separator;
	typedef boost::tokenizer<escaped_list_separator, Str::const_iterator, Str> tokenizer;

	escaped_list_separator sep(esc, delim, quote);
	tokenizer tokens(str, sep);

	std::copy(tokens.begin(), tokens.end(), back_inserter(out));

	return distance(tokens.begin(), tokens.end());
}


/**
 * @brief	���������؂蕶���ŕ�������
 *
 * @param[out]	out		�����㕶������i�[����o�b�t�@
 * @param		str		�����Ώە�����
 * @param		delim	��؂蕶��
 * @param		quote	���p������
 * @param		esc		�G�X�P�[�v����
 *
 * @return	������
 *
 * @warning	�����������out�����ɒǉ�����邽�߁A�߂�l = out.size()�Ƃ͌���Ȃ�!
 */
size_t StringSplitW(vector<wstring> &out, const std::wstring &str, const wchar_t * delim, const wchar_t * quote/*=L""*/, const wchar_t * esc/*=L""*/)
{
	return _StringSplitT<vector<wstring>, std::wstring, wchar_t>(out, str, delim, quote, esc);
}

/**
 * @brief	���������؂蕶���ŕ�������
 *
 * @param[out]	out		�����㕶������i�[����o�b�t�@
 * @param		str		�����Ώە�����
 * @param		delim	��؂蕶��
 * @param		quote	���p������
 * @param		esc		�G�X�P�[�v����
 *
 * @return	������
 *
 * @warning	�����������out�����ɒǉ�����邽�߁A�߂�l = out.size()�Ƃ͌���Ȃ�!
 */
size_t StringSplitA(vector<string>  &out, const string  &str, const char * delim, const char * quote/*=""*/, const char * esc/*=""*/)
{
	return _StringSplitT<vector<string>, std::string, char>(out, str, delim, quote, esc);
}


/**
 * @brief	2�������r����
 *
 * @param	c1, c2	��r����
 *
 * @return	c1��c2���������̏����Ŕ�r�����֌W������
 * @retval	<0	c1��c2��菬����
 * @retval	=0	c1��c2�͓�����
 * @retval	>0	c2��c1��菬����
 */
int yk51_CharCompareW(wchar_t c1, wchar_t c2)
{
	int lc1 = towlower(c1);
	int lc2 = towlower(c2);
	return (lc1<lc2)?-1:(lc1>lc2)?1:0;
}

/**
 * @brief	�Q�̕�������������Ƃ��Ĕ�r����
 *
 * @param	s1, s2	��r������
 *
 * @return	s1��s2���������̏����Ŕ�r�����֌W������
 * @retval	<0	s1��s2��菬����
 * @retval	=0	s1��s2�͓�����
 * @retval	>0	s2��s1��菬����
 */
int yk51_StringICompW(const wstring &s1, const wstring &s2)
{
	typedef pair<wstring::const_iterator, wstring::const_iterator> pairT;

	pairT p = mismatch(s1.begin(), s1.end(),
					   s2.begin(),
					   not2(ptr_fun(yk51_CharCompareW)));

	if(p.first == s1.end()){
		if(p.second == s2.end()) return 0;
		else return -1;
	}
	return yk51_CharCompareW(*p.first, *p.second);
}


/**
 * @brief	2�������r����
 *
 * @param	c1, c2	��r����
 *
 * @return	c1��c2���������̏����Ŕ�r�����֌W������
 * @retval	<0	c1��c2��菬����
 * @retval	=0	c1��c2�͓�����
 * @retval	>0	c2��c1��菬����
 */
int yk51_CharCompareA(char c1, char c2)
{
	int lc1 = tolower(static_cast<unsigned char>(c1));
	int lc2 = tolower(static_cast<unsigned char>(c2));
	return (lc1<lc2)?-1:(lc1>lc2)?1:0;
}

/**
 * @brief	�Q�̕�������������Ƃ��Ĕ�r����
 *
 * @param	s1, s2	��r������
 *
 * @return	s1��s2���������̏����Ŕ�r�����֌W������
 * @retval	<0	s1��s2��菬����
 * @retval	=0	s1��s2�͓�����
 * @retval	>0	s2��s1��菬����
 */
int yk51_StringICompA(const string &s1, const string &s2)
{
	typedef pair<string::const_iterator, string::const_iterator> pairT;

	pairT p = mismatch(s1.begin(), s1.end(),
					   s2.begin(),
					   not2(ptr_fun(yk51_CharCompareA)));

	if(p.first == s1.end()){
		if(p.second == s2.end()) return 0;
		else return -1;
	}
	return yk51_CharCompareA(*p.first, *p.second);
}
