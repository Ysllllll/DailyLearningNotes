#include "Timer.h"

int main1()
{
	std::string string1 = "string 1";
	std::string string2 = "string 2";
	std::string string3 = "string 3";
	std::string string4 = "string 4";

	TimerManage::adderTimer(string1, 2000);
	TimerManage::adderTimer(string2, 4000);
	TimerManage::adderTimer(string3, 6000);
	TimerManage::adderTimer(string4, 8000);
	while (1)
		TimerManage::handleExpiredEvent();
	return 0;
}

int main()
{
	std::string string1 = "string 1";
	std::string string2 = "string 2";
	std::string string3 = "string 3";
	std::string string4 = "string 4";

	TimerNode *t1 = TimerManage::adderTimer(string1, 4000);
	TimerNode *t2 = TimerManage::adderTimer(string2, 8000);
	TimerNode *t3 = TimerManage::adderTimer(string3, 12000);
	TimerNode *t4 = TimerManage::adderTimer(string4, 16000);

	sleep(9);
	std::cout << "t1->isValid(): " << t1->isValid() << std::endl;
	std::cout << "t1->isDelete(): " << t1->isDelete() << std::endl;
	t1->update(1000);
	// 将这里改为10000试试，输出会一次性显示4个timer被删除。
	//如果为1000，则t1和t2会过1000ms后的同一时刻显示被删除，
	//然后再12000-9000-1000=2000ms后显示t3被删除，再过16000-9000-1000后显示t4被删除
	//这里表明，delete的timer需要等到：1.超时；2.它前面的timer全部被删除后它才会被删除
	std::cout << "t1->isValid(): " << t1->isValid() << std::endl;
	std::cout << "t1->isDelete(): " << t1->isDelete() << std::endl;

	while (1)
		TimerManage::handleExpiredEvent();
	return 0;
}