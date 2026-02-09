#ifndef STRING_PROCESSOR
#define STRING_PROCESSOR
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <ctype.h>
#include <stdio.h>
#include "sort.h"
#define STRING_MAX 512ull
// forward declarations
const size_t strlenn(const char* str);
int32_t strcmpp(const char* fStr, const char* sStr);

// aliases (якщо десь у коді використовуються стандартні імена)
inline size_t strlen(const char* s) { return strlenn(s); }
inline int32_t strcmp(const char* a, const char* b) { return strcmpp(a, b); }

const int32_t get_str(const char const* msg, char* str, const int32_t limit)
{
    if (msg != NULL)
    {
        // повідомлення користувачу (якщо msg передано)
        printf("%s", msg);
        fflush(stdout);
    }

    if (str == NULL || limit <= 0)
    {
        return -1;
    }

    // читаємо рядок з stdin
    if (fgets(str, (int)limit, stdin) == NULL)
    {
        str[0] = '\0';
        return -1;
    }

    // прибираємо \n (і \r\n на Windows)
    size_t len = strlenn(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
    {
        str[len - 1] = '\0';
        --len;
    }

    return (int32_t)len;
}

const size_t strlenn(const char* str)
{
    if (str == NULL) return 0ull;

    const char* p = str;
    while (*p != '\0')
    {
        ++p;
    }
    return (size_t)(p - str);
}

void strcopy(char* fStr, char* sStr, size_t until)
{
    if (fStr == NULL || sStr == NULL) return;

    // копіюємо не більше until символів, завжди ставимо '\0'
    size_t i = 0ull;
    for (; i < until && sStr[i] != '\0'; ++i)
    {
        fStr[i] = sStr[i];
    }
    fStr[i] = '\0';
}

int32_t strcmpp(const char* fStr, const char* sStr)
{
    // порівняння як strcmp: <0, 0, >0
    if (fStr == NULL) fStr = "";
    if (sStr == NULL) sStr = "";

    while (*fStr != '\0' && *sStr != '\0' && *fStr == *sStr)
    {
        ++fStr;
        ++sStr;
    }

    return (int32_t)((unsigned char)(*fStr) - (unsigned char)(*sStr));
}

char* strcatt(char* fStr, const char* sStr)
{
    char* ptr = fStr + strlenn(fStr);
    while (*sStr != '\0')
    {
        *ptr++ = *sStr++;
    }
    *ptr = '\0';
    return fStr;
}
bool is_string_valid(char* str)
{
    bool isThereString = false;
    bool isThereColon = false;
    const size_t size = strlenn(str);
    for (size_t i = 0; i < size; i++)
    {
        if ((str[i] == ':' || str[i] == ';' || str[i] == ',') && i == 0ull)
        {
            return false;
        }
        if (str[i] == ':')
        {
            isThereColon = true;
        }
        else if (isThereColon)
        {
            if (str[i] == ',')
            {
                if (!isalpha(str[i - 1ull]) && str[i - 1ull] != ' ')
                {
                    return false;
                }
                else
                {
                    isThereString = true;
                }
            }
            if (str[i] == ';')
            {
                if (!isThereString || (!isalpha(str[i - 1ull]) && str[i - 1ull] != ' '))
                {
                    return false;
                }
                else
                {
                    isThereColon = false;
                }
            }
            if (str[i] == ':')
            {
                return false;
            }
        }
        else
        {
            isThereString = false;
        }
    }
    return true;
}
char* process_string(char* str)
{
    const size_t length = strlenn(str);
    char* result = (char*)calloc(length, sizeof(char));
    for (size_t i = 0; i < length; i++)
    {
        if (str[i] != ':')
        {
            char temp[2] = { str[i], '\0' };
            strcatt(result, temp);
            continue;
        }
        if (str[i + 1] != ' ')
        {
            return NULL;
        }
        strcatt(result, ": ");
        const size_t strsPreCnt = 100;
        char** arr = (char**)calloc(strsPreCnt, sizeof(char*));
        for (size_t strsCnt = 0; strsCnt < strsPreCnt; strsCnt++)
        {
            arr[strsCnt] = (char*)calloc(STRING_MAX, sizeof(char));
        }
        for (size_t j = i + 2ull, strRealCnt = 0ull; j < length; j++)
        {
            if (str[j] == ',')
            {
                ++strRealCnt;
                ++j;
            }
            else if (str[j] == ';')
            {
                quick_sort(arr, ++strRealCnt);
                for (size_t k = 0; k < strRealCnt; k++)
                {
                    strcatt(result, arr[k]);
                    if (k == strRealCnt - 1ull)
                    {
                        strcatt(result, ";");
                    }
                    else
                    {
                        strcatt(result, ", ");
                    }
                }
                i = j;
                break;
            }
            else
            {
                char temp[2] = { str[j], '\0' };
                strcatt(arr[strRealCnt], temp);
            }
        }
        for (size_t strsCnt = 0; strsCnt < strsPreCnt; strsCnt++)
        {
            free(arr[strsCnt]);
        }
        free(arr);
    }
    return result;
}
#endif