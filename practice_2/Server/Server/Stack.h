#pragma once
#include <iostream>
#include <string>

using namespace std;

struct NodeS {
    string data;
    NodeS* next;

    NodeS(const string& value) : data(value), next(nullptr) {}
};

struct Stack {
    NodeS* top;

    Stack() : top(nullptr) {}

    void push(const string& value) {
        NodeS* newNodeS = new NodeS(value);

        newNodeS->next = top;
        top = newNodeS;
    }

    string pop() {
        if (isEmpty()) {
            return "Stack is empty.";
        }

        NodeS* temp = top;
        top = top->next;
        string poppedValue = temp->data;
        delete temp;

        return poppedValue;
    }

    string getTop() const {
        if (isEmpty()) {
            return "Stack is empty.";
        }

        return top->data;
    }

    bool isEmpty() const {
        return (top == nullptr);
    }

    void printStack() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return;
        }

        NodeS* current = top;
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
            NodeS* current = top;
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
            temp += "} stack";
            out << temp << endl;
        }
        out.close();
    }
};