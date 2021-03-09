#include <filesystem>
#include "download.h"
#include "execjar.h"

int main() {
	Downloader *dl = new Downloader();

	std::filesystem::path temp = std::filesystem::temp_directory_path();
	temp /= "test.jar";

	CURLcode result = dl->downloadFile("https://jamesvps.tk/static/test.jar", temp);

	delete dl;

	if (result == CURLE_OK) runJar(temp, "Main");
	else return 1;
}
