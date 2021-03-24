#include <sys/epoll.h>
#include <unistd.h>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <pthread.h>

class MimeType
{
private:
	MimeType();
	MimeType(const MimeType &m);

	static void init();

private:
	static std::unordered_map<std::string, std::string> mime;
	static pthread_once_t once_control;

public:
	static std::string getMime(const std::string &suffix);
};