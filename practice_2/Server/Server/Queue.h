#pragma once
#include <iostream>
#include <string>

using namespace std;

struct NodeQ {
    string data;
    NodeQ* next;

    NodeQ(const string& value) : data(value), next(nullptr) {}
};

struct Queue {
    NodeQ* front;
    NodeQ* rear;

    Queue() : front(nullptr), rear(nullptr) {}

    void push(const string& value) {
        NodeQ* newNodeQ = new NodeQ(value);

        if (isEmpty()) {
            front = rear = newNodeQ;
        }
        else {
            rear->next = newNodeQ;
            rear = newNodeQ;
        }
    }

    string pop() {
        if (isEmpty()) {
            string s = "queue is empty";
            return s;
        }

        NodeQ* temp = front;
        front = front->next;
        string poppedValue = temp->data;
        delete temp;

        if (front == nullptr) {
            rear = nullptr;
        }

        return poppedValue;
    }


    string getFront() const {
        if (isEmpty()) {
            return "Queue is empty.";
        }

        return front->data;
    }

    bool isEmpty() const {
        return (front == nullptr);
    }

    void printQueue() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }

        NodeQ* current = front;
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
            temp = containerName + " : [";
            NodeQ* current = front;
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
            temp += "] queue";
            out << temp << endl;
        }
        out.close();
    }


};
