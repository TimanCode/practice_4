#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Queue.h"
#include "Stack.h"
#include "Set.h"
#include "Locale.h"
#include "HashTable.h"

using namespace std;

void removeLineFromFile(string filePath, string lineToRemove) {
    ifstream inputFile(filePath);
    if (!inputFile) {
        ofstream inputFile(filePath);
        if (!inputFile) throw "Не удалось открыть файл.\n";

    }

    ofstream outputFile("temp.txt");
    if (!outputFile) throw "Не удалось создать временный файл.\n";


    string line;
    while (getline(inputFile, line)) {
        if (line != lineToRemove) {
            outputFile << line << endl;
        }
    }

    inputFile.close();
    outputFile.close();

    if (remove(filePath.c_str()) != 0) throw"Не удалось удалить исходный файл.\n";
    if (rename("temp.txt", filePath.c_str()) != 0) throw "Не удалось переименовать временный файл.\n";

    return;
}

void createFile(string filePath) {
    ofstream inputFile(filePath);
    if (!inputFile) throw "Не удалось создать файл.\n";

    inputFile.close();

}

Queue getQueueFromFile(string filePath, string containerName) {
    string line;
    Queue tempQueue;

    ifstream in(filePath);
    if (in.is_open()) {
        while (getline(in, line)) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string queueName = line.substr(0, colonPos - 1);
                if (queueName == containerName) {
                    string elements = line.substr(colonPos + 1);
                    size_t bracePos1 = elements.find('[');
                    size_t bracePos2 = elements.find(']');
                    string type = elements.substr(bracePos2 + 2);
                    if (type == "queue") {
                        if (bracePos1 != string::npos && bracePos2 != string::npos) {
                            elements = elements.substr(bracePos1 + 1, bracePos2 - bracePos1 - 1);
                            size_t commaPos = elements.find(',');
                            while (commaPos != string::npos) {
                                string element = elements.substr(0, commaPos);
                                tempQueue.push(element);
                                elements = elements.substr(commaPos + 2);
                                commaPos = elements.find(',');
                            }
                            tempQueue.push(elements);
                            break;
                        }
                    }
                }
            }
        }
    }
    in.close();
    removeLineFromFile(filePath, line);
    return tempQueue;
}

Stack getStackFromFile(string filePath, string containerName) {
    string line;
    Stack tempStack;

    ifstream in(filePath);
    if (in.is_open()) {
        while (getline(in, line)) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string queueName = line.substr(0, colonPos - 1);
                if (queueName == containerName) {

                    string elements = line.substr(colonPos + 1);
                    size_t bracePos1 = elements.find('{');
                    size_t bracePos2 = elements.find('}');
                    string type = elements.substr(bracePos2 + 2);
                    if (type == "stack") {
                        if (bracePos1 != string::npos && bracePos2 != string::npos) {
                            elements = elements.substr(bracePos1 + 1, bracePos2 - bracePos1 - 1);
                            size_t commaPos = elements.find(',');
                            while (commaPos != string::npos) {
                                string element = elements.substr(0, commaPos);
                                tempStack.push(element);
                                elements = elements.substr(commaPos + 2);
                                commaPos = elements.find(',');
                            }
                            tempStack.push(elements);
                            break;
                        }
                    }
                }
            }
        }
    }
    in.close();
    removeLineFromFile(filePath, line);
    return tempStack;
}

Set getSetFromFile(string filePath, string containerName, bool removeline = true) {
    string line;
    Set tempStack;

    ifstream in(filePath);
    if (in.is_open()) {
        while (getline(in, line)) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string queueName = line.substr(0, colonPos - 1);
                if (queueName == containerName) {

                    string elements = line.substr(colonPos + 1);
                    size_t bracePos1 = elements.find('{');
                    size_t bracePos2 = elements.find('}');
                    string type = elements.substr(bracePos2 + 2);
                    if (type == "set") {
                        if (bracePos1 != string::npos && bracePos2 != string::npos) {
                            elements = elements.substr(bracePos1 + 1, bracePos2 - bracePos1 - 1);
                            size_t commaPos = elements.find(',');
                            while (commaPos != string::npos) {
                                string element = elements.substr(0, commaPos);
                                tempStack.insert(element);
                                elements = elements.substr(commaPos + 2);
                                commaPos = elements.find(',');
                            }
                            tempStack.insert(elements);
                            break;
                        }
                    }
                }
            }
        }
    }
    in.close();
    if (removeline) {
        removeLineFromFile(filePath, line);
    }
    return tempStack;
}

HashTable getHTFromFile(string filePath, string containerName, bool removeline = true) {
    string line;
    HashTable tempHT = HashTable(10);

    ifstream in(filePath);
    if (in.is_open()) {
        while (getline(in, line)) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string queueName = line.substr(0, colonPos - 1);
                if (queueName == containerName) {
                    string elements = line.substr(colonPos + 1);
                    size_t bracePos1 = elements.find('<');
                    size_t bracePos2 = elements.find('>');
                    string type = elements.substr(bracePos2 + 2);
                    if (type == "hashtable") {
                        if (bracePos1 != string::npos && bracePos2 != string::npos) {
                            elements = elements.substr(bracePos1 + 1, bracePos2 - bracePos1 - 1);
                            size_t commaPos = elements.find('|');
                            while (commaPos != string::npos) {
                                string element = elements.substr(0, commaPos);
                                size_t tempcommaPos = element.find(' ');
                                string tempelement = element.substr(0, tempcommaPos);
                                element = element.substr(tempcommaPos, string::npos);
                                element = element.substr(3, string::npos);
                                tempelement = tempelement.substr(2, string::npos);
                                tempHT.insert(tempelement, element);
                                elements = elements.substr(commaPos + 2);
                                commaPos = elements.find(',');
                            }
                            size_t tempcommaPos = elements.find(' ');
                            string tempelement = elements.substr(0, tempcommaPos);
                            elements = elements.substr(tempcommaPos, string::npos);
                            elements = elements.substr(3, string::npos);
                            tempelement = tempelement.substr(2, string::npos);
                            tempHT.insert(tempelement, elements);
                            break;
                        }
                    }
                }
            }
        }
    }
    in.close();
    if (removeline) {
        removeLineFromFile(filePath, line);
    }

    return tempHT;
}

void sendToClient(int Socket, string data) {
    int dataLen = data.length();
    send(Socket, reinterpret_cast<char*>(&dataLen), sizeof(dataLen), 0);
    send(Socket, data.c_str(), dataLen, 0);
}

void sendToClient(int Socket, bool value) {
    string data = value ? "true" : "false";
    int dataLen = sizeof(data);
    send(Socket, reinterpret_cast<char*>(&dataLen), sizeof(dataLen), 0);
    send(Socket, data.c_str(), dataLen, 0);
}

void processCommand(string filePath, string query, int Socket) {
    size_t colonPos = query.find(' ');
    string command = query.substr(0, colonPos);
    string containerName = query.substr(colonPos+1);
    containerName = containerName.substr(0, containerName.find(' '));
    if (command == "QPUSH") {
        Queue tempQueue = getQueueFromFile(filePath, containerName);
        string data = query.substr(colonPos + 1);
        tempQueue.printQueue();
        auto temp = data.find(' ');
        if (temp < 100) {
            data = data.substr(temp+1);
            tempQueue.push(data);
            tempQueue.writeContainerToFile(filePath, containerName);
            string s = "Удачно";
            cout << "\t->" << s << endl;
            sendToClient(Socket, s);
        }
        else {
            string s = "Мало аргументов";
            cout << "\t->" << s << endl;
            sendToClient(Socket, s);
        }

    }
    else if (command == "QPOP") {
        Queue tempQueue = getQueueFromFile(filePath, containerName);
        string s = tempQueue.pop();
        sendToClient(Socket, s);
        cout << "\t->Удачно" << endl;
        tempQueue.writeContainerToFile(filePath, containerName);
    }
    else if (command == "SPUSH") {
        Stack tempStack = getStackFromFile(filePath, containerName);
        string data = query.substr(colonPos + 1);
        data = data.substr(data.find(' ') + 1);
        tempStack.push(data);
        tempStack.writeContainerToFile(filePath, containerName);
        string s = "Удачно";
        cout << "\t->" << s << endl;
        sendToClient(Socket, s);
    }
    else if (command == "SPOP") {
        Stack tempStack = getStackFromFile(filePath, containerName);
        sendToClient(Socket, tempStack.pop());
        cout << "\t->Удачно" << endl;
        tempStack.writeContainerToFile(filePath, containerName);
    }
    else if (command == "SADD") {
        string s;
        Set tempSet = getSetFromFile(filePath, containerName);
        string data = query.substr(colonPos + 1);
        auto temp = data.find(' ');
        if(temp < 100){
            data = data.substr(temp + 1);
            s = tempSet.insert(data);
            cout << "\t->" << s << endl;
            sendToClient(Socket, s);
            tempSet.writeContainerToFile(filePath, containerName);
        }
        else {
            s = "Мало аргументов";
            cout << "\t->" << s << endl;
            sendToClient(Socket, s);
        }
        
    }
    else if (command == "SREM") {
        Set tempSet = getSetFromFile(filePath, containerName);
        string data = query.substr(colonPos + 1);
        auto temp = data.find(' ');
        if (temp < 100) {
            data = data.substr(temp + 1);
            string s = tempSet.remove(data);
            if (s == "True") {
                tempSet.writeContainerToFile(filePath, containerName);
                cout << "\t->Удачно" << endl;
            }
            else {
                sendToClient(Socket, s);
            }
        }
        else {
            string s = "Мало аргументов";
            cout << "\t->" << s << endl;
            sendToClient(Socket, s);
        }
        
        
    }
    else if (command == "SISMEMBER") {
        Set tempSet = getSetFromFile(filePath, containerName, false);
        string data = query.substr(colonPos + 1);
        auto temp = data.find(' ');
        if (temp < 100) {
            data = data.substr(temp + 1);
            cout << "\t->Удачно" << endl;
            sendToClient(Socket, tempSet.contains(data));
        }
        else {
            string s = "Мало аргументов";
            cout << "\t->" << s << endl;
            sendToClient(Socket, s);
        }
        
    }
    else if (command == "HSET") {
        HashTable tempHashTable = getHTFromFile(filePath, containerName);
        string key = query.substr(colonPos + 1);
        key = key.substr(key.find(' ') + 1);
        auto temp = key.find(' ');
        if (temp < 100) {
            string value = key.substr(temp + 1);;
            key.erase(temp, value.length() + 1);
            string s = tempHashTable.insert(key, value);
            sendToClient(Socket, s);
            cout << "\t->" << s << endl;
            tempHashTable.writeContainerToFile(filePath, containerName);
        }
        else {
            string s = "Мало аргументов";
            cout << "\t->" << s << endl;
            sendToClient(Socket, s);
        }
    }
    else if (command == "HDEL") {
        HashTable tempHashTable = getHTFromFile(filePath, containerName);
        string key = query.substr(colonPos + 1);
        key = key.substr(key.find(' ') + 1);
        string s = tempHashTable.remove(key);
        cout << "\t->" << s << endl;
        sendToClient(Socket, s);
        tempHashTable.writeContainerToFile(filePath, containerName);
    }
    else if (command == "HGET") {
        HashTable tempHashTable = getHTFromFile(filePath, containerName, false);
        string key = query.substr(colonPos + 1);
        key = key.substr(key.find(' ') + 1);
        tempHashTable.display();

        string s = tempHashTable.get(key);
        cout << "\t->" << s << endl;
        sendToClient(Socket, s);
    }
    else {
        string s = "Неверная команда";
        cout << s << endl;
        sendToClient(Socket, s);
    }
}