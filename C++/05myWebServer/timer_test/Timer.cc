#include "Timer.h"

std::priority_queue<TimerNode *, std::deque<TimerNode *>, TimerCmp> m_timerNodeQueue;

TimerNode::TimerNode(TimerNodeInfo info, size_t timeOut) : m_info(info), m_delete(false)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	m_expireTime = (now.tv_sec % 10000) * 1000 + (now.tv_usec / 1000) + timeOut;
}

TimerNode::~TimerNode()
{
	std::cout << "~TimerNode" << std::endl;
	if (m_info.length())
		std::cout << m_info << std::endl;
	m_info.clear();
}

// 这个函数的一个作用就是复制一个 timer 记录的信息
TimerNode::TimerNode(TimerNode &timer, size_t timeOut) : m_info(timer.m_info), m_expireTime(0), m_delete(false)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	m_expireTime = (now.tv_sec % 10000) * 1000 + (now.tv_usec / 1000) + timeOut;
}

void TimerNode::update(size_t timeOut)
{
	if (this->isDelete() == true)
	{
		std::cout << "timer has been deleted, and cancel delete" << std::endl;
		this->cancelDelete();
	}
	struct timeval now;
	gettimeofday(&now, NULL);
	m_expireTime = (now.tv_sec % 10000) * 1000 + (now.tv_usec / 1000) + timeOut;
}

bool TimerNode::isValid()
{
	if (this->isDelete() == true)
	{
		std::cout << "timer has been deleted, and is not valid" << std::endl;
		return false;
	}
	struct timeval now;
	gettimeofday(&now, NULL);
	size_t expireTime = (now.tv_sec % 10000) * 1000 + (now.tv_usec / 1000);

	if (m_expireTime < expireTime)
	{
		this->setDelete();
		return false;
	}
	else
	{
		return true;
	}
}

void TimerNode::clearInfo()
{
	m_info.clear();
	this->setDelete();
};

void TimerManage::adderTimer(TimerNode *timer)
{
	m_timerNodeQueue.push(timer);
}

void TimerManage::adderTimer(TimerNode::TimerNodeInfo info, size_t timeOut)
{
	TimerNode *timer = new TimerNode(info, timeOut);
	m_timerNodeQueue.push(timer);
}

void TimerManage::handleExpiredEvent()
{
	while (!m_timerNodeQueue.empty())
	{
		TimerNode *top = m_timerNodeQueue.top();

		if (top->isValid() == false)
		{
			m_timerNodeQueue.pop();
			std::cout << "deque timer " << top << ", info: " << top->getInfo() << std::endl;
		}
		else
			break;
	}
}