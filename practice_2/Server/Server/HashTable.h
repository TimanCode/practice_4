#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct NodeHT {
    string key;
    string value;

    NodeHT(const string& k, const string& v) : key(k), value(v) {}
};

struct HashTable {
    vector<NodeHT*> table;
    int size;

    HashTable(int initialSize) : size(initialSize) {
        table.resize(size, nullptr);
    }

    int hashFunction(const string& key) const {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % size;
    }

    int secondHashFunction(const string& key) const {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return 1 + (hash % (size - 1));
    }

    string insert(const string& key, const string& value) {
        int index = hashFunction(key);
        int step = secondHashFunction(key);

        while (table[index] != nullptr) {
            if (table[index]->key == key) {
                string s = "Ключ уже существует.";
                return s;
            }
            index = (index + step) % size;
        }

        table[index] = new NodeHT(key, value);
        return "Удачно";
    }

    string get(const string& key) const {
        int index = hashFunction(key);
        int step = secondHashFunction(key);

        while (table[index] != nullptr && table[index]->key != key) {
            index = (index + step) % size;
        }

        if (table[index] != nullptr && table[index]->key == key) {
            return table[index]->value;
        }

        return "No key.";
    }

    string remove(const string& key) {
        int index = hashFunction(key);
        int step = secondHashFunction(key);

        while (table[index] != nullptr && table[index]->key != key) {
            index = (index + step) % size;
        }

        if (table[index] != nullptr && table[index]->key == key) {
            delete table[index];
            table[index] = nullptr;
            return "Удачно";
        }
        return "Хэштаблица пуста";
    }

    void display() {
        for (int i = 0; i < size; i++) {
            if (table[i] != NULL) {
                cout << "ключ: " << table[i]->key << "\t значение: " << table[i]->value << endl; // Выводим ключ и значение записи, если запись существует и не удалена
            }
        }
    }

    void writeContainerToFile(string filePath, string containerName) {
        string temp;
        int error = 0;
        ofstream out(filePath, ios::app);
        if (out.is_open()) {
            temp = containerName + " : <";
            
            for (int i = 0; i < size; i++) {
                if (table[i] != NULL) {
                    temp += "k:" + table[i]->key + " v:" + table[i]->value + "| ";
                }
                else {
                    error++;
                }
            }
            if (error == size) {
                out << "" << endl;
                out.close();
                return;
            }
            temp.pop_back();
            temp.pop_back();
            temp += "> hashtable";
            out << temp << endl;
        }
        out.close();
    }

};