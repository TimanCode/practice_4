#include <iostream>
#include <locale.h>
#include <String>
#include "Client.h"
using namespace std;



string processFilePath(int argc, char* argv[]) {
    if (argc >= 4 && string(argv[1]) == "--file") return argv[2];
    throw "Отсутсвует или неверно указан путь.";
}

string processQuery(int argc, char* argv[]) {
    if (argc >= 4 && string(argv[3]) == "--query") {
        string query;
        for (int i = 4; i < argc; i++) {
            query = query + argv[i] + " ";
        }
        
        query = query.substr(1);
        query.pop_back();
        query.pop_back();
        return query;
    }
    throw "Отсутсвует или неверно указан запрос.";
}

int main(int argc, char* argv[]){
	setlocale(LC_ALL, "Russian");
    Flags dataToSend;
    
	try {
        dataToSend.file_path = processFilePath(argc, argv);
        dataToSend.query = processQuery(argc, argv);
		ClientConnection(dataToSend);
	}
	catch(const char* error_message){
		cout << "->" << error_message << endl;
	}

	return 0;
}