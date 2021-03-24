#include "HttpData.h"

pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::mime;

void MimeType::init()
{
	mime["default"] = "text/html";
	mime[".html"] = "text/html";
	mime[".htm"] = "text/html";
	mime[".c"] = "text/plain";
	mime[".txt"] = "text/plain";
	mime[".bmp"] = "image/bmp";
	mime[".gif"] = "image/gif";
	mime[".jpg"] = "image/jpeg";
	mime[".png"] = "image/png";
	mime[".ico"] = "image/x-icon";
	mime[".doc"] = "application/msword";
	mime[".gz"] = "application/x-gzip";
	mime[".mp3"] = "audio/mp3";
	mime[".avi"] = "video/x-msvideo";
}
std::string MimeType::getMime(const std::string &suffix)
{
	pthread_once(&once_control, MimeType::init);
	if (mime.find(suffix) == mime.end())
		return mime["default"];
	else
		return mime[suffix];
}