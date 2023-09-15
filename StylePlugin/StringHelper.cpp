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

bool StringHelper::IsStringContainsForword(const QString& str, const QString& findStr, char stopChar, int index)
{
    bool isContains = false;
    int stopIndex = -1;
    for (int i = index; i >= 0; i--)
    {
        if (str[i].toLatin1() == stopChar)
        {
            stopIndex = i;
            break;
        }
    }
    if (stopIndex >= 0)
    {
        QString midStr = str.mid(stopIndex + 1, index - stopIndex - 1);
        isContains = midStr.contains(findStr);
    }
    return isContains;
}
