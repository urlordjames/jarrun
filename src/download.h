#include <string>
#include <filesystem>
#include <curl/curl.h>

class Downloader {
public:
	Downloader();
	CURLcode downloadFile(std::string url, std::filesystem::path path);
	~Downloader();

private:
	CURL *handle;
	static size_t writeFile(void *file, size_t size, size_t nmemb, void *stream);
};
