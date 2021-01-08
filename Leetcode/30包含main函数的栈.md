#

>**剑指offer 30.包含min函数的栈**：定义栈的数据结构，请在该类型中实现一个能够得到栈的最小元素的 min 函数在该栈中，调用 min、push 及 pop 的时间复杂度都是 O(1)。
示例:
　MinStack minStack = new MinStack();
　minStack.push(-2);
　minStack.push(0);
　minStack.push(-3);
　minStack.min();--> 返回 -3.
　minStack.pop();
　minStack.top();--> 返回 0.
　minStack.min();--> 返回 -2.
提示：
　各函数的调用总次数不超过 20000 次
注意：[本题与主站 155 题相同](https://leetcode-cn.com/problems/min-stack/)

这道题的关键是需要用到两个栈，一个栈用来记录常规压栈出栈操作，另外一个栈用来记录前一个栈的最小值。

模型如下：

```shell

normal: |  -4  |           min: |  -4  |            index：7
        |   0  |                |  -3  |            index：6
        |  -3  |                |  -3  |            index：5
        |  -3  |                |  -3  |            index：4
        |   0  |                |  -1  |            index：3
        |  -1  |                |  -1  |            index：2
        |   2  |                |   2  |            index：1
        +------+                +------+
```

针对某一个 `index`，`min[index]` 记录的一定是 `normal[1~index]` 中的最小值。它不会记录 `normal[index+1~top]` 的最小值，这里要明白我们访问到 `index` 时，`index+1~top` 的元素必定已经全部出栈。

```C++
//by：me
class MinStack {
private:
    stack<int> stack_normal;
    stack<int> stack_min;

public:
    /** initialize your data structure here. */
    MinStack() {

    }
    
    void push(int x) {
        if(!stack_min.size() || stack_min.top() > x)
            stack_min.push(x);
        else
            stack_min.push(stack_min.top());
        stack_normal.push(x);
    }
    
    void pop() {
        if(!stack_normal.size())
            return;
        stack_min.pop();
        stack_normal.pop();
    }
    
    int top() {
        return stack_normal.top();
    }
    
    int min() {
        return stack_min.top();
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(x);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->min();
 */
```

上面stack_normal和stack_min两个栈的长度始终一样，其实我们可以只在stack_normal中的最小值更新时将该最小值压入stack_min中，都是一样的，只是这个模型占用的空间可能少一点。

```shell

normal: |  -4  |           min:                     index：7
        |   0  |                                    index：6
        |  -3  |                |  -4  |            index：5
        |  -3  |                |  -3  |            index：4  这里注意，是为了保证弹出操作的正确性。
        |   0  |                |  -3  |            index：3
        |  -1  |                |  -1  |            index：2
        |   2  |                |   2  |            index：1
        +------+                +------+
```

```C++
//by：Krahets
class MinStack {
private:
    stack<int> stack_normal;
    stack<int> stack_min;

public:
    /** initialize your data structure here. */
    MinStack() {

    }
    
    void push(int x) {
        if(!stack_min.size() || stack_min.top() >= x)
            stack_min.push(x);
        stack_normal.push(x);
    }
    
    void pop() {
        if(stack_min.top() == stack_normal.top())
            stack_min.pop();
        stack_normal.pop();
    }
    
    int top() {
        return stack_normal.top();
    }
    
    int min() {
        return stack_min.top();
    }
};
```
