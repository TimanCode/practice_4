#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <thread>
#include "Locale.h"
#include "FileProcess.h"
#include <mutex>
#pragma comment(lib, "ws2_32.lib")

using namespace std;
SOCKET Connections[10000000];
mutex counterMutex; 
int Counter = 0;


struct Flags {
	string file_path;
	string query;
};

enum Packet {
	P_ChatMessage,
	P_Test
};

bool ProcessPacket(int index, Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);
		cout << msg << endl;

		delete[] msg;
		break;
	}
	default:
		cout << "Unrecognized packet: " << packettype << std::endl;
		break;
	}

	return true;
}

	Flags receiveFlags(int clientSocket) {
		Flags receivedData;

		// Получение длины пути к файлу
		int pathLength;
		if (recv(clientSocket, reinterpret_cast<char*>(&pathLength), sizeof(pathLength), 0) == SOCKET_ERROR) throw "Ошибка: ошибка  чтения длины пути к файлу.\n";

		// Получение длины запроса
		int queryLength;
		if (recv(clientSocket, reinterpret_cast<char*>(&queryLength), sizeof(queryLength), 0) == SOCKET_ERROR) throw "Ошибка: ошибка  чтения длины запроса.\n";

		// Получение пути к файлу
		char* pathBuffer = new char[pathLength + 1];
		if (recv(clientSocket, pathBuffer, pathLength, 0) == SOCKET_ERROR) throw "Ошибка: ошибка чтения пути к файлу.\n";

		pathBuffer[pathLength] = '\0'; // Добавляем завершающий символ строки
		receivedData.file_path = std::string(pathBuffer);
		delete[] pathBuffer;

		

		// Получение запроса
		char* queryBuffer = new char[queryLength + 1];
		if (recv(clientSocket, queryBuffer, queryLength, 0) == SOCKET_ERROR) throw "Ошибка: ошибка  чтения запроса.\n";

		queryBuffer[queryLength] = '\0'; // Добавляем завершающий символ строки
		receivedData.query = std::string(queryBuffer);
		delete[] queryBuffer;

		return receivedData;
	}


void ClientHandler(int index) {
	Flags receivedData = receiveFlags(Connections[index]);;

	cout << "\t->file_path: " << receivedData.file_path << "\t query: " << receivedData.query << endl;
	{
		lock_guard<mutex> lock(counterMutex);
		processCommand(receivedData.file_path, receivedData.query, Connections[index]);
		closesocket(Connections[index]);
		Counter++;
	}
	
}

int server() {
	// Инициализация Winsock
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) throw "Ошибка: проблема инициализации Winsock.\n";

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(6379);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	cout << "->Сервер создан." << endl;
	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) throw "Ошибка: проблема создания сокета.\n";
		else {
			cout << "Новое подключение\n";
			{
				lock_guard<mutex> lock(counterMutex);
				Connections[i] = newConnection;
			}
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}

	return 0;
}


int main() {
	setlocale(LC_ALL, "Russian");
	server();
	
	
	return 0;
}