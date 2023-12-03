#pragma once
#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <thread>
#include <iostream>
#include <winsock2.h>

using namespace std;
SOCKET Connection;

struct Flags {
	string file_path;
	string query;
};

enum Packet {
	P_ChatMessage,
	P_Test
};


bool ProcessPacket(Packet packettype) {
	switch (packettype) {
	case P_ChatMessage:
	{
		int msg_size;
		recv(Connection, (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connection, msg, msg_size, NULL);
		cout << msg << endl;
		delete[] msg;
		break;
	}
	case P_Test:
		cout << "Test packet.\n";
		break;
	default:
		cout << "Unrecognized packet: " << packettype << endl;
		break;
	}

	return true;
}

void ClientHandler() {
	int queryLength;
	recv(Connection, reinterpret_cast<char*>(&queryLength), sizeof(queryLength), 0);
	char* queryBuffer = new char[queryLength + 1];
	recv(Connection, queryBuffer, queryLength, 0);
	queryBuffer[queryLength] = '\0'; // ���������� ������������ �������� �������
	cout << "->" << queryBuffer << endl;
	delete[] queryBuffer; 
}


void sendFlags(int sock, const Flags& data) {
	// �������� ����� ���� � �����
	int pathLength = data.file_path.length();
	if (send(sock, reinterpret_cast<char*>(&pathLength), sizeof(pathLength), 0) == SOCKET_ERROR) {
		cout << "������ �������� ����� ���� � �����.\n";
		return;
	}
	// �������� ���� � �����
	if (send(sock, data.file_path.c_str(), pathLength, 0) == SOCKET_ERROR) {
		cout << "������ �������� ���� � �����.\n";
		return;
	}

	// �������� ����� �������
	int queryLength = data.query.length();
	if (send(sock, reinterpret_cast<char*>(&queryLength), sizeof(queryLength), 0) == SOCKET_ERROR) {
		cout << "������ �������� ����� �������.\n";
		return;
	}

	// �������� �������
	if (send(sock, data.query.c_str(), queryLength, 0) == SOCKET_ERROR) {
		cout << "������ �������� �������.\n";
		return;
	}


}

int ClientConnection(const Flags& data) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) throw "������:\n";

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(6379);
	addr.sin_family = AF_INET;

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) throw "������: ���������� ����������� � �������.\n";
	cout << "��������� � �������!\n";
	
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
	sendFlags(Connection, data);
	Sleep(50);
	// �������� ���������� � ������������ ��������
	closesocket(Connection);
	WSACleanup();

	return 0;
}