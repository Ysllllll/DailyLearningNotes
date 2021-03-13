#ifndef TIMER
#define TIMER

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <queue>

class TimerNode
{
public:
	typedef std::string TimerNodeInfo;

public:
	TimerNode() = delete;
	TimerNode(TimerNodeInfo info, size_t timeOut);
	TimerNode(TimerNode &timer, size_t timeOut);
	~TimerNode();

	void update(size_t timeOut);
	bool isValid();

	bool isDelete() { return m_delete; }
	void setDelete() { m_delete = true; }
	void cancelDelete() { m_delete = false; };

	void clearInfo();

	size_t getExpireTime() const { return m_expireTime; };
	TimerNodeInfo getInfo() { return m_info; };

private:
	bool m_delete;
	size_t m_expireTime;
	TimerNodeInfo m_info;
};

struct TimerCmp
{
	bool operator()(TimerNode *&t1, TimerNode *&t2) const
	{
		return t1->getExpireTime() > t2->getExpireTime();
	}
};

class TimerManage
{
public:
	static std::priority_queue<TimerNode *, std::deque<TimerNode *>, TimerCmp> m_timerNodeQueue;

public:
	static TimerNode *adderTimer(TimerNode *timer);
	static TimerNode *adderTimer(TimerNode::TimerNodeInfo info, size_t timeOut);
	static void handleExpiredEvent();
	static void closeAllTimer(){};

private:
	TimerManage(){};
	~TimerManage(){};
};
#endif