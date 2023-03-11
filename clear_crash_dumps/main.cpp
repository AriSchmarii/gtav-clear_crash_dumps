#include <iostream>
#include <filesystem>
#include <shlobj.h>

#pragma warning(disable : 4996)

int main() {
	int deleted_file_count = { 0 };
	PWSTR local_path_dummy = { };
	if (const HRESULT local_appdata_path = SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_DEFAULT, NULL, &local_path_dummy); local_appdata_path != S_OK)
		return EXIT_FAILURE;
	char path_dest[MAX_PATH];
	::wcstombs(path_dest, local_path_dummy, MAX_PATH);
	const std::string crashlogs_path = std::string(path_dest) + "\\Rockstar Games\\GTAV\\CrashLogs";
	if (!std::filesystem::exists(crashlogs_path))
		return EXIT_FAILURE;
	std::cout << "Crashlog path found at " << crashlogs_path << "\nStarting to delete files..." << std::endl;
	for (const auto& file_entry : std::filesystem::directory_iterator(crashlogs_path + "\\")) {
		const std::string file_name = file_entry.path().string();
		if (file_name.substr(file_name.find_last_of(".") + 1) != "dmp")
			continue;
		std::remove(file_name.c_str());
		deleted_file_count++;
	}
	std::cout << "Deleted " << deleted_file_count << " .dmp files" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}