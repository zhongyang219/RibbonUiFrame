#pragma once
#include <QString>

class StringHelper
{
public:
    /**
        @brief  利用QString实现std::string中的find_fisrt_of函数的功能
        @param  str     - 被查找的字符串
        @param  findStr - 要查找的字符串
        @param  index   - 查找的起始位置
        @retval         - 查找到的位置
    **/
    static int QStringFindFirstOf(const QString& str, const QString& findStr, int index);

    /**
        @brief  利用QString实现std::string中的find_last_of函数的功能
        @param  str     - 被查找的字符串
        @param  findStr - 要查找的字符串
        @param  index   - 查找的起始位置
        @retval         - 查找到的位置
    **/
    static int QStringFindLastOf(const QString& str, const QString& findStr, int index);

    static bool IsNumber(char ch);

    /**
        @brief  对于字符串str，从index的位置开始向前查找，直到找到stopChar为止，判断这中间是否包含findStr
        @param  str     - 被查找的字符串
        @param  findStr - 要查找的字符串
        @param  stopChar - 查找结束的字符
        @param  index   - 查找的起始位置
        @retval         - 是否包含要查找的字符串
    **/
    static bool IsStringContainsForword(const QString& str, const QString& findStr, char stopChar, int index);
};

