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
