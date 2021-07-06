#include <iostream>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <sstream>
#include <bitset>
#include <numeric>
using namespace std;

struct ListNode
{
	int val;
	ListNode *next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode *next) : val(x), next(next) {}
};

ListNode *createList(vector<int> &nums)
{

	ListNode *head = new ListNode(-1, nullptr);
	ListNode *pre = head;
	for (int num : nums)
	{
		pre->next = new ListNode(num, nullptr);
		pre = pre->next;
	}
	return head->next;
}

void printList(ListNode *head)
{
	cout << "[ ";
	while (head != nullptr)
	{
		cout << head->val << " ";
		head = head->next;
	}
	cout << "]" << endl;
}

struct TreeNode
{
	int val;
	TreeNode *left;
	TreeNode *right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

template <class T>
void printfDp2(vector<vector<T>> &dp)
{
	int m = dp.size();
	int n = dp[0].size();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << dp[i][j] << " ";
		}
		cout << endl;
	}
}

template <class T>
void printfDp1(vector<T> &dp)
{
	int m = dp.size();
	for (int i = 0; i < m; i++)
	{
		cout << dp[i] << " ";
	}
	cout << endl;
}

class other
{
public:
	// 主要完成的功能是：
	// 倒序偶数节点
	// [ 1 2 3 4 5 6 ]
	// [ 1 6 3 4 5 2 ]
	void reorderList(ListNode *head)
	{
		if (head == nullptr)
			return;

		ListNode *dummy1 = new ListNode(-1, head);
		ListNode *dummy2 = new ListNode(-1, head->next);
		ListNode *head1 = head;
		ListNode *head2 = head->next;
		while (head1 != nullptr && head2 != nullptr)
		{
			if (head2->next == nullptr)
			{
				head1->next = nullptr;
				break;
			}
			head1->next = head2->next;
			head2->next = head2->next->next;
			head1 = head1->next;
			head2 = head2->next;
		}

		ListNode *pre = dummy2;
		ListNode *cur = pre->next;
		while (cur != nullptr && cur->next != nullptr)
		{
			ListNode *next = cur->next;
			cur->next = next->next;
			next->next = pre->next;
			pre->next = next;
		}

		head1 = dummy1->next;
		head2 = dummy2->next;
		while (head1 != nullptr && head2 != nullptr)
		{
			ListNode *next1 = head1->next;
			ListNode *next2 = head2->next;
			head1->next = head2;
			head2->next = next1;
			head1 = next1;
			head2 = next2;
		}
		return;
	}
};

// 创建一个大顶堆
class MaxHeap
{
private:
	vector<int> data;
	int _capacity;
	int _size;

public:
	MaxHeap(vector<int> &vec)
	{
		_capacity = vec.size();
		data = vector<int>(_capacity, 0);
		_size = 0;

		for (int i = 0; i < _capacity; i++)
		{
			offer(vec[i]);
		}
	}
	MaxHeap(vector<int> &vec, int len)
	{
		_capacity = vec.size();
		data = vector<int>(_capacity, 0);
		_size = vec.size();

		for (int i = 0; i < _capacity; i++)
		{
			data[i] = vec[i];
		}

		for (int i = (_capacity - 1) / 2; i >= 0; i--)
		{
			shiftDown(i);
		}
	}
	int size()
	{
		return _size;
	}
	int capacity()
	{
		return _capacity;
	}
	bool isEmpty()
	{
		return _size == 0;
	}
	int peek()
	{
		if (_size == 0)
			return INT_MIN;
		else
			return data[0];
	}
	void replace(int x)
	{
		if (_size == 0 || _capacity == 0)
		{
			return;
		}
		data[0] = x;
		shiftDown(0);
	}

	void offer(int x)
	{
		if (_size == _capacity)
			return;
		data[_size] = x;
		_size++;
		shiftUp(_size);
	}
	int poll()
	{
		int res = data[0];
		data[0] = data[_size - 1];
		_size--;
		shiftDown(0);
		return res;
	}
	void shiftUp(int low)
	{
		int temp = data[low];

		for (int i = (low - 1) / 2; i >= 0; i = (i - 1) / 2)
		{
			if (data[i] >= temp)
				break;
			data[low] = data[i];
			low = i;
			if (i == 0)
				break;
		}

		data[low] = temp;
	}
	void shiftDown(int high)
	{
		int temp = data[high];

		for (int i = high * 2 + 1; i < _size; i = i * 2 + 1)
		{
			if (i + 1 < _size && data[i] < data[i + 1])
				i++;
			if (data[i] <= temp)
				break;

			data[high] = data[i];
			high = i;
		}
		data[high] = temp;
	}
};
