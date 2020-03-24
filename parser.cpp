#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
using namespace std;

int concurrency;

void searchFile(string path, string device);

void searchLogs(string path, string device, string fileName);

void searchIndexColon(string& str, int& index);

void printInfoLogs(string& fileName, string& info, string& second);

int main() {
	string deviceName;
	while (1) {
		cin >> deviceName;
		searchFile("logs/", deviceName);
		if (concurrency == 0) {
			cout << "Not find!" << endl;
		}
	}
}

void searchFile(string path, string device) {
	concurrency = 0;
	for (auto& pointer : filesystem::directory_iterator(path)) {
		auto fileName = pointer.path().filename().string();
		searchLogs(pointer.path().string(), device, fileName);
	}
}

void searchLogs(string path, string device, string fileName) {
	fstream readFile;
	readFile.open(path);
	while(!readFile.eof()) {
		string str, info, deviceName;
		string second = "0";
		getline(readFile, str);
		int index = 0;
		searchIndexColon(str, index);
		second += str.substr(0, index);
		str.replace(0, index + 1, "");
		index = 0;
		searchIndexColon(str, index);
		deviceName = str.substr(0, index);
		if(deviceName != device) {
			continue;
		}
		str.replace(0, index + 1, "");
		info = str;
		concurrency++;
		printInfoLogs(fileName, info, second);
	}
	readFile.close();
}

void printInfoLogs(string& fileName, string& info, string& second) {
	string numberOfDate = fileName.substr(5, fileName.length() - 10);
	cout << "DATA | " << numberOfDate << "-" << second << " | INFO | " << info << "\n";
}

void searchIndexColon(string& str, int& index) {
	for (int i = 0; i < str.length(); i++) {
		if(str[i] == ':') {
			index = i;
			break;
		}
	}
}