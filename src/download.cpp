#include "download.h"

Downloader::Downloader() {
	curl_global_init(CURL_GLOBAL_ALL);
	handle = curl_easy_init();

	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, Downloader::writeFile);
	curl_easy_setopt(handle, CURLOPT_FAILONERROR, 1);

	// would be very bad for security if this was not set
	curl_easy_setopt(handle, CURLOPT_USE_SSL, CURLUSESSL_ALL);

#ifndef NDEBUG
	curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
#endif
}

Downloader::~Downloader() {
	curl_easy_cleanup(handle);
	curl_global_cleanup();
}

size_t Downloader::writeFile(void *file, size_t size, size_t nmemb, void *stream) {
	return fwrite(file, size, nmemb, (FILE*) stream);
}

CURLcode Downloader::downloadFile(std::string url, std::filesystem::path path) {
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

	// std::filesystem::path::c_str() doesn't work on Windows for some reason
	FILE *file = fopen(path.string().c_str(), "wb");

	if (!file) return CURLE_FAILED_INIT;

	curl_easy_setopt(handle, CURLOPT_WRITEDATA, file);

	CURLcode result = curl_easy_perform(handle);

	fclose(file);

	return result;
}
