#pragma once
#include <stddef.h>
#include <stdint.h>

// оголосимо strcmpp (вона реалізована у stringProcessor.h)
int32_t strcmpp(const char* fStr, const char* sStr);

// swap для виклику swap(a,b) де a,b типу char*
inline void swap(char*& a, char*& b)
{
    char* tmp = a;
    a = b;
    b = tmp;
}

// swap для виклику swap(&a,&b) або swap(&arr[i], &arr[j])
inline void swap(char** a, char** b)
{
    char* tmp = *a;
    *a = *b;
    *b = tmp;
}

static int partition(char** arr, int low, int high)
{
    char* pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        // сортування за зростанням (як у тестах)
        if (strcmpp(arr[j], pivot) <= 0)
        {
            ++i;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

static void quick_sort_impl(char** arr, int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quick_sort_impl(arr, low, pi - 1);
        quick_sort_impl(arr, pi + 1, high);
    }
}

// фінальна функція, яку викликають у твоєму коді: quick_sort(arr, count)
inline void quick_sort(char** arr, size_t count)
{
    if (!arr || count < 2) return;
    quick_sort_impl(arr, 0, (int)count - 1);
}
