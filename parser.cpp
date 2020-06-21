#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>


void searchLogsByName(std::string path, std::string fileName, std::string device, bool& isEmpty, int i){
	std::fstream readFile; //создаем переменную типа fstream для чтения файлов
	readFile.open(path); //открываем файл для чтения

	while (!readFile.eof()){ 
		/*
		eof() - грубо говоря, индикатор того, дошли ли мы до конца файла
		пока не дошли он в позиции false, когда дошли true
		Грубо говоря, цикл будет выполняться, пока НЕ дойдем до конца файла
		*/ 
		std::string info, time, second, str, deviceName;
		int index = 0; //счетчик
		getline(readFile, str);//считывает СТРОКУ из файла в переменную str
		if (str.length() == 0 || str.find("garbage") != -1) {
			//если длина = 0 ИЛИ мы нашли "garbage" в строке, то continue - код не будет продолжаться а запустится новая итерация цикла
			//str.find() - вернет -1, если ничего не было найдено, в нашем условии если он вернет что-то, отличное от -1, значит он нашел garbage
			continue;
		}
		

		for (int i = 0; i < str.length(); i++){ 
			/*
			цикл по строке, чтобы найти первое двоеточие
			по формату логов мы знаем, что до первого двоеточего у нас записывается секунда
			*/
			if(str[i] == ':') { //если нашли двоеточие
				index = i; //запоминаем номер её позиции в строке
				break; //останавливаем цикл
			}
		}
		if(index == 2) {
			/*
			Тут просто сделано для более удобного отображения секунды в консоле, так как у нас в логах есть файлы с двухзначным числом
			секунд
			str.substr() - возвращает отрезок строки, который мы указываем, первым аргументом принимает начало, вторым, длину строки
			которую надо вернуть
			И присваиваем это значение переменной second 
			*/
			second = str.substr(0, index);
		} else {
			second = "0" + str.substr(0, index);
		}
		
		str.replace(0, index + 1, "");
		/*
		replace() - первый аргумент начало, второй длина строки, третье чем нужно изменить
		грубо говоря replace заменяет указанный нами участок строки на тот, что мы указали в качестве 3-го аргумента
		в данном случае просто заменяет на пустоту
		*/
		index = 0; //обнуляем счетчик

		for (int i = 0; i < str.length(); i++){ //снова ищем уже второе двоеточие
			if(str[i] == ':') {
				index = i; 
				break;
			}
		}
		/*
		когда нашли, то используем substr, чтобы вернуть тот участок строки, который находиться до второго двоеточего
		*/
		deviceName = str.substr(0, index); 
		/*
		между первым и вторым двоеточим находится имя девайса
		*/
		if (deviceName != device) { //проверяем, совпадает ли имя девайса с тем, что указывал пользователь, если не совпадает,
									//то заходим в блок IF и выполняем оператор continue - после него код не выполняется, а идет на след. итерацию, т.е. сначала цикла
			continue;
		}
		//если прошли блок IF, значит имена девайсов совпали, и мы нашли то, что нужно
		str.replace(0, index + 1, ""); //снова убираем уже пройденный участок строки
		info = str; //оставшийся участок строки соответствует полю INFO 
		isEmpty = false; //ставим булевый флаг в позицию false, логика его в том, что если бы ничего не нашлось во всех файлах, 
		//он бы был true, и это обрабатывает в main 112 строка, если он true, то значит ничего не нашлось
		time = fileName.substr(5, fileName.length() - 10);//время находиться в имени файла лога, вырезаем кусок, который нам нужен
		std::cout << i << " DATA | " << time << "-" << second << " | INFO | " << info << std::endl; //выводим
	}
	readFile.close(); //закрываем файл
}

int main() {
	std::string path, device;
	std::cout << "Enter device name: "; getline(std::cin, device);
	std::cout << "Enter path:"; getline(std::cin, path);

	std::vector<std::string> v_pathFile;
	std::vector<std::string> v_File;
	bool isEmpty = true;
	try {//блок try-catch служит обработчиком ошибок, если в блоке try произойдет какая-то ошибка, то мы сразу же перейдем в блок catch
		//в данном случае try-catch обрабатывает то, правильно ли указан путь до папки с логами
		for (auto& pointer : std::filesystem::directory_iterator(path)) { //директори итератор просто перебирает файлы каталога, он их не открывает
		auto fileName = pointer.path().filename().string(); //заносим в переменную filename имя файла с логом путем вызова метода filename(), который возвращает имя файла
		auto pathFile = pointer.path().string(); //и метода string(), который приводит его к типу строки, метод path() возвращает путь до файла
		v_pathFile.push_back(pathFile); //заносим имя файла и путь до файла в отдельные вектора(массивы)
		v_File.push_back(fileName);
		}
	}
	catch(const std::exception e) {
		std::cout << "Invalid path!" << std::endl;
		return 0;
	}

	for (int i = 0; i < v_File.size(); i++) { //цикл который вызывает функцию равную числу файлов в папке с логами
		searchLogsByName(v_pathFile[i], v_File[i], device, isEmpty, i);
	}

	if(isEmpty) { //если мы ничего не нашли, то сообщаем об этом
		std::cout << "Not find!" << std::endl;
	}
}

