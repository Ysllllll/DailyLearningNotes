<a id="singly_linked_list"></a>

> **题目汇总**
>
> - **[234. 回文链表](https://leetcode-cn.com/problems/palindrome-linked-list/)** [速览题解](#234)
> - **[328. 奇偶链表](https://leetcode-cn.com/problems/odd-even-linked-list/)** [速览题解](#328)
>   - 案例：[1 2 3 4]、[1 2 3 4 5]

<a id="234"></a>

### [234. 回文链表](#palindrome_string)

```C++{.line-numbers}
// 第一种，先装进数组中，再判断。时间空间复杂度都是O(n)
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        vector<int> vec;
        while(head!= nullptr)
        {
            vec.push_back(head->val);
            head = head->next;
        }
        for(int i = 0, j = vec.size() - 1; i <= j; i++, j--)
        {
            if(vec[i] != vec[j])
                return false;
        }

        return true;
    }
};

// 空间复杂度为O(1)，时间复杂度为O(n)，同时能够复原链表
class Solution {
public:
    bool isPalindrome(ListNode* head) {
        if(head == nullptr || head->next == nullptr) return true;

        // 不区分奇偶，slow指针走到中间，此时以slow为分界点
        // headATail是为了辅助还原链表
        ListNode *slow = head, *fast = head, *headATail = head;
        while(fast != nullptr && fast->next != nullptr)
        {
            headATail = slow;
            slow = slow->next;
            fast = fast->next->next;
        }
        headATail->next = nullptr;

        // 以slow为头节点，反转链表得到headB
        ListNode *headB = nullptr;
        while(slow != nullptr)
        {
            ListNode *next = slow->next;
            slow->next = headB;
            headB = slow;
            slow = next;
        }
        // 得到headA
        ListNode *headA = head;

        // 对比headA和headB两个链表判断是否为回文串
        // 同时复原链表
        bool res = true;
        ListNode *headBNext = nullptr;
        while(headA != nullptr)
        {
            if(res && (headA->val != headB->val))
                res = false;
            headA = headA->next;

            ListNode *next = headB->next;
            headB->next = headBNext;
            headBNext = headB;
            headB = next;
        }

        if(headB != nullptr)
        {
            headB->next = headBNext;
            headBNext = headB;
        }
        headATail->next = headBNext;
        
        
        return res;
    }
};
```

<a id="328"></a>

### [328. 奇偶链表](#singly_linked_list)

```C++{.line-numbers}
// 案例：[1 2 3 4] [1 2 3 4 5]
class Solution {
public:
    ListNode* oddEvenList(ListNode* head) {
        if(head == nullptr || head->next == nullptr) return head;
        ListNode* oddHead = head;
        ListNode* evenHead = head->next;

        ListNode* oddPointer = oddHead, *evenPointer = evenHead;
        // 分为奇尾巴和偶尾巴
        // [1 2 3 4]、[1 2 3 4 5]
        // 奇尾巴一定有值最后
        while(oddPointer->next != nullptr && evenPointer->next != nullptr)
        {
            oddPointer->next = evenPointer->next;
            evenPointer->next = oddPointer->next->next;

            oddPointer = oddPointer->next;
            evenPointer = evenPointer->next;
        }
        oddPointer->next = evenHead;
        return oddHead;
    }
};
```
