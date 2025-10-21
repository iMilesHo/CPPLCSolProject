/*
g++ -std=c++17 -Wall -Iinclude -I/opt/homebrew/include playground.cpp
-L/opt/homebrew/lib -lgtest -lgtest_main -pthread -o playground_test
*/

#include <gtest/gtest.h>

#include <climits>   // 用于 INT_MAX, LONG_MAX
#include <iostream>  // 用于 std::cout
#include <numeric>   // 用于 iota
#include <unordered_map>
#include <vector>

using namespace std;

template <typename K, typename V>
struct Node
{
    K key;
    V val;
    Node* next;
    Node* prev;

    Node(K key, V val) : key(key), val(val), next(nullptr), prev(nullptr) {}
};

template <typename K, typename V>
class MyLinkedHashMap
{
  private:
    unordered_map<K, Node<K, V>*> map;
    Node<K, V>* head;
    Node<K, V>* tail;

    void addToHead(Node<K, V>* node) {}
    void removeNode(Node<K, V>* node) {}
    Node<K, V>* removeTail() {}
    void moveToHead(Node<K, V>* node) {}

  public:
    MyLinkedHashMap()
    {
        head = new Node<K, V>(K{}, V{});
        tail = new Node<K, V>(K{}, V{});
        head->next = tail;
        tail->prev = head;
    }

    // Copy Constructor
    MyLinkedHashMap(const MyLinkedHashMap& other)
    {
        head = new Node<K, V>(K{}, V{});
        tail = new Node<K, V>(K{}, V{});
        head->next = tail;
        head->prev = nullptr;
        tail->prev = head;
        tail->next = nullptr;

        Node<K, V>* current = other.head->next;
        while (current != other.tail)
        {
            put(current->key, current->val);
            current = current->next;
        }
    }
    // Copy Assignment Operator
    MyLinkedHashMap& operator=(const MyLinkedHashMap& other)
    {
        if (&other != this)
        {
            clear();
            Node<K, V>* current = other.head->next;
            while (current != other.tail)
            {
                put(current->key, current->val);
                current = current->next;
            }
        }
        return *this;
    }

    // Move Constructor
    MyLinkedHashMap(MyLinkedHashMap&& other) noexcept
        : map(std::move(other.map)), head(other.head), tail(other.tail)
    {
        // Reset other to valid empty state
        other.head = new Node<K, V>(K{}, V{});
        other.tail = new Node<K, V>(K{}, V{});
        other.head->next = other.tail;
        other.head->prev = nullptr;
        other.tail->prev = other.head;
        other.tail->next = nullptr;
    }


    // Move Assignment Operator
    MyLinkedHashMap& operator=(MyLinkedHashMap&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            delete head;
            delete tail;

            // move to current
            map = std::move(other.map);
            head = other.head;
            tail = other.tail;

            // reset other
            other.head = new Node<K, V>(K{}, V{});
            other.tail = new Node<K, V>(K{}, V{});
            other.head->next = other.tail;
            other.tail->prev = other.head;
        }

        return *this;
    }

    // Destructor
    ~MyLinkedHashMap()
    {
        clear();
        delete head;
        delete tail;
    }

  private:
    // Helper method to clear all content
    void clear() {}

  public:
    // Get value by key
    // Put Key-value pair
    // Remove key
    // Check if key exists
    // Get size
    // Check if empty
}