/*bugging.c*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

// 单指针
int partition1(vector<int> &arr, int left, int right)
{
    swap(arr[left], arr[rand() % (right - left + 1) + left]);
    int pivot = arr[left];

    int lt = left;
    for (int i = left + 1; i <= right; i++)
    {
        if (arr[i] < pivot)
        {
            lt++;
            swap(arr[i], arr[lt]);
        }
    }
    swap(arr[lt], arr[left]);
    return lt;
}

// 双指针标准写法
int partition2(vector<int> &arr, int left, int right)
{
    swap(arr[left], arr[rand() % (right - left + 1) + left]);
    int pivot = arr[left];

    while (left < right)
    {
        while (left < right && arr[right] >= pivot)
            right--;
        arr[left] = arr[right];
        while (left < right && pivot >= arr[left])
            left++;
        arr[right] = arr[left];
    }
    arr[left] = pivot;
    return left;
}

// 双指针一：将重复元素放至数组两边
int partition3(vector<int> &arr, int left, int right)
{
    swap(arr[left], arr[rand() % (right - left + 1) + left]);
    int pivot = arr[left];

    int lt = left + 1;
    int gt = right;
    while (true)
    {
        while (lt <= gt && arr[lt] < pivot)
            lt++;
        while (lt <= gt && pivot < arr[gt])
            gt--;

        if (lt > gt)
            break;
        swap(arr[lt], arr[gt]);
        lt++;
        gt--;
    }
    swap(arr[gt], arr[left]);
    return gt;
}

// 双指针二：将重复元素放至数组中间
void quickSort2(vector<int> &arr, int left, int right)
{
    if (left >= right)
        return;
    swap(arr[left], arr[rand() % (right - left + 1) + left]);
    int pivot = arr[left];
    int lt = left;
    int gt = right + 1;

    int i = left + 1;
    while (i < gt)
    {
        if (arr[i] < pivot)
        {
            lt++;
            swap(arr[i], arr[lt]);
            i++;
        }
        else if (arr[i] == pivot)
        {
            i++;
        }
        else
        {
            gt--;
            swap(arr[i], arr[gt]);
        }
    }
    swap(arr[left], arr[lt]);
    quickSort2(arr, left, lt - 1);
    quickSort2(arr, gt, right);
}

void quickSort1(vector<int> &arr, int left, int right)
{
    if (left >= right)
        return;

    int pivot = partition3(arr, left, right);
    quickSort1(arr, left, pivot - 1);
    quickSort1(arr, pivot + 1, right);
}

int main()
{
    vector<int> arr{4, 2, 5, 1, 6, 7, 3, 3, 3, 3};
    quickSort2(arr, 0, arr.size() - 1);
    for (vector<int>::iterator i = arr.begin(); i != arr.end(); i++)
    {
        cout << *i << " ";
    }
    cout << endl;
}