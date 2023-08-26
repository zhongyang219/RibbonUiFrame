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
};

