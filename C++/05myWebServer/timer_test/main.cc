#include "Timer.h"

int main()
{
	std::string string1 = "string 1";
	std::string string2 = "string 2";
	std::string string3 = "string 3";
	std::string string4 = "string 4";

	TimerManage::adderTimer(string1, 2000);
	TimerManage::adderTimer(string2, 4000);
	TimerManage::adderTimer(string3, 6000);
	TimerManage::adderTimer(string4, 8000);
	return 0;
}