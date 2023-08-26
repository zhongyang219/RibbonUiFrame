#include "StringHelper.h"

int StringHelper::QStringFindFirstOf(const QString& str, const QString& findStr, int index)
{
    if (index < 0)
        index = 0;
    for (int i = index; i < str.size(); i++)
    {
        if (findStr.contains(str[i]))
            return i;
    }
    return -1;
}

int StringHelper::QStringFindLastOf(const QString& str, const QString& findStr, int index)
{
    if (index > str.size() - 1)
        index = str.size() - 1;
    for (int i = index; i >= 0; i-- )
    {
        if (findStr.contains(str[i]))
            return i;
    }
    return -1;
}

bool StringHelper::IsNumber(char ch)
{
    return ch >= '0' && ch <= '9';
}
