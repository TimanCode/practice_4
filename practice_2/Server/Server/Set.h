#pragma once
#include <iostream>
#include <string>
using namespace std;

struct Node {
    string data;
    Node* next;

    Node(const string& value) : data(value), next(nullptr) {}
};

struct Set {
    Node* head;

    Set() : head(nullptr) {}

    string insert(const string& value) {
        if (contains(value))
            return "В set уже есть данный элемент.";

        Node* newNode = new Node(value);

        newNode->next = head;
        head = newNode;
        return "Удачно";
    }

    string remove(const string& value) {
        if (isEmpty())
            return "Set пуст\0";

        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return "Удачно";
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->data == value) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return "Удачно";
            }
            current = current->next;
        }
    }

    bool contains(const string& value) const {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value)
                return true;
            current = current->next;
        }
        return false;
    }

    bool isEmpty() const {
        return (head == nullptr);
    }

    void printSet() {
        if (isEmpty()) {
            cout << "Set is empty." << endl;
            return;
        }

        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void writeContainerToFile(string filePath, string containerName) {
        string temp;
        ofstream out(filePath, ios::app);
        if (out.is_open()) {
            temp = containerName + " : {";
            Node* current = head;
            if (current == nullptr) {
                out << "" << endl;
                out.close();
                return;
            }
            while (current != nullptr) {
                temp += current->data + ", ";
                current = current->next;
            }
            temp.pop_back();
            temp.pop_back();
            temp += "} set";
            out << temp << endl;
        }
        out.close();
    }
};