#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

void searchLogs(std::string path, std::string fileName, std::string device, bool& isEmpty){
	std::fstream readFile;
	readFile.open(path);

	while (!readFile.eof()){
		std::string info, time, second = "0", str, deviceName;
		int index = 0;
		getline(readFile, str);

		for (int i = 0; i < str.length(); i++){
			if(str[i] == ':') {
				index = i;
				break;
			}
		}

		second += str.substr(0, index);
		str.replace(0, index + 1, "");
		index = 0;

		for (int i = 0; i < str.length(); i++){
			if(str[i] == ':') {
				index = i;
				break;
			}
		}

		deviceName = str.substr(0, index);
		if (deviceName != device) {
			continue;
		}
		
		str.replace(0, index + 1, "");
		info = str;
		isEmpty = false;
		time = fileName.substr(5, fileName.length() - 10);
		std::cout << "DATA | " << time << "-" << second << " | INFO | " << info << std::endl;
	}
	readFile.close();
}

int main() {
	std::string path, device; std::cin >> device; std::cin >> path;

	std::vector<std::string> v_pathFile;
	std::vector<std::string> v_File;
	bool isEmpty = true;
	try {
		for (auto& pointer : std::filesystem::directory_iterator(path)) {
		auto fileName = pointer.path().filename().string();
		auto pathFile = pointer.path().string();
		v_pathFile.push_back(pathFile);
		v_File.push_back(fileName);
		}
	}
	catch(const std::exception e) {
		std::cout << "Invalid path!" << std::endl;
		return 0;
	}

	for (int i = 0; i < v_File.size(); i++) {
		searchLogs(v_pathFile[i], v_File[i], device, isEmpty);
	}

	if(isEmpty) {
		std::cout << "Not find!" << std::endl;
	}
}

