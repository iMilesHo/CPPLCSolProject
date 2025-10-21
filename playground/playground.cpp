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

    void addToHead(Node<K, V>* node)
    {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(Node<K, V>* node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    Node<K, V>* removeTail()
    {
        Node<K, V>* lastNode = tail->prev;
        removeNode(lastNode);
        return lastNode;
    }

    void moveToHead(Node<K, V>* node)
    {
        removeNode(node);
        addToHead(node);
    }

  public:
    // Constructor
    MyLinkedHashMap()
    {
        head = new Node<K, V>(K{}, V{});
        tail = new Node<K, V>(K{}, V{});
        head->next = tail;
        tail->prev = head;
    }

    // Copy Constructor (拷贝构造函数)
    MyLinkedHashMap(const MyLinkedHashMap& other)
    {
        head = new Node<K, V>(K{}, V{});
        tail = new Node<K, V>(K{}, V{});
        head->next = tail;
        tail->prev = head;

        // Deep copy all nodes from other
        Node<K, V>* current = other.head->next;
        while (current != other.tail)
        {
            put(current->key, current->val);
            current = current->next;
        }
    }

    // Copy Assignment Operator (拷贝赋值运算符)
    MyLinkedHashMap& operator=(const MyLinkedHashMap& other)
    {
        if (this != &other)  // Self-assignment check
        {
            // Clear current content
            clear();

            // Deep copy from other
            Node<K, V>* current = other.head->next;
            while (current != other.tail)
            {
                put(current->key, current->val);
                current = current->next;
            }
        }
        return *this;
    }

    // Move Constructor (移动构造函数)
    MyLinkedHashMap(MyLinkedHashMap&& other) noexcept
        : map(std::move(other.map)), head(other.head), tail(other.tail)
    {
        // Reset other to valid empty state
        other.head = new Node<K, V>(K{}, V{});
        other.tail = new Node<K, V>(K{}, V{});
        other.head->next = other.tail;
        other.tail->prev = other.head;
    }

    // Move Assignment Operator (移动赋值运算符)
    MyLinkedHashMap& operator=(MyLinkedHashMap&& other) noexcept
    {
        if (this != &other)
        {
            // Clean up current resources
            clear();
            delete head;
            delete tail;

            // Move resources from other
            map = std::move(other.map);
            head = other.head;
            tail = other.tail;

            // Reset other to valid empty state
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
    void clear()
    {
        Node<K, V>* current = head->next;
        while (current != tail)
        {
            Node<K, V>* next = current->next;
            delete current;
            current = next;
        }
        head->next = tail;
        tail->prev = head;
        map.clear();
    }

  public:
    // Get value by key
    V get(const K& key)
    {
        auto it = map.find(key);
        if (it != map.end())
        {
            Node<K, V>* node = it->second;
            moveToHead(node);
            return node->val;
        }
        throw runtime_error("Key not found");
    }

    // Put key-value pair
    void put(const K& key, const V& value)
    {
        auto it = map.find(key);
        if (it != map.end())
        {
            // Key exists, update value
            Node<K, V>* node = it->second;
            node->val = value;
            moveToHead(node);
        }
        else
        {
            // Key doesn't exist, create new node
            Node<K, V>* newNode = new Node<K, V>(key, value);
            map[key] = newNode;
            addToHead(newNode);
        }
    }

    // Remove key
    bool remove(const K& key)
    {
        auto it = map.find(key);
        if (it != map.end())
        {
            Node<K, V>* node = it->second;
            removeNode(node);
            map.erase(it);
            delete node;
            return true;
        }
        return false;
    }

    // Check if key exists
    bool contains(const K& key) const
    {
        return map.find(key) != map.end();
    }

    // Get size
    size_t size() const
    {
        return map.size();
    }

    // Check if empty
    bool empty() const
    {
        return map.empty();
    }

    // Print all elements in insertion order
    void printInOrder() const
    {
        Node<K, V>* current = head->next;
        cout << "LinkedHashMap contents: ";
        while (current != tail)
        {
            cout << "[" << current->key << ":" << current->val << "] ";
            current = current->next;
        }
        cout << endl;
    }
};

// Test cases for MyLinkedHashMap
TEST(MyLinkedHashMapTest, BasicOperations)
{
    MyLinkedHashMap<int, string> lhm;

    // Test empty
    EXPECT_TRUE(lhm.empty());
    EXPECT_EQ(lhm.size(), 0);

    // Test put and get
    lhm.put(1, "one");
    lhm.put(2, "two");
    lhm.put(3, "three");

    EXPECT_FALSE(lhm.empty());
    EXPECT_EQ(lhm.size(), 3);

    EXPECT_EQ(lhm.get(1), "one");
    EXPECT_EQ(lhm.get(2), "two");
    EXPECT_EQ(lhm.get(3), "three");

    // Test contains
    EXPECT_TRUE(lhm.contains(1));
    EXPECT_TRUE(lhm.contains(2));
    EXPECT_TRUE(lhm.contains(3));
    EXPECT_FALSE(lhm.contains(4));

    // Test update existing key
    lhm.put(2, "TWO");
    EXPECT_EQ(lhm.get(2), "TWO");
    EXPECT_EQ(lhm.size(), 3);

    // Test remove
    EXPECT_TRUE(lhm.remove(2));
    EXPECT_FALSE(lhm.contains(2));
    EXPECT_EQ(lhm.size(), 2);
    EXPECT_FALSE(lhm.remove(2));  // Remove non-existing key

    // Print to see insertion order
    lhm.printInOrder();
}

TEST(MyLinkedHashMapTest, AccessOrder)
{
    MyLinkedHashMap<string, int> lhm;

    // Insert in order: a, b, c
    lhm.put("a", 1);
    lhm.put("b", 2);
    lhm.put("c", 3);

    cout << "Initial order: ";
    lhm.printInOrder();

    // Access 'a' - it should move to most recently used
    lhm.get("a");
    cout << "After accessing 'a': ";
    lhm.printInOrder();

    // Update 'b' - it should move to most recently used
    lhm.put("b", 20);
    cout << "After updating 'b': ";
    lhm.printInOrder();
}

TEST(MyLinkedHashMapTest, CopySemantics)
{
    MyLinkedHashMap<int, string> original;
    original.put(1, "one");
    original.put(2, "two");
    original.put(3, "three");

    cout << "Original: ";
    original.printInOrder();

    // Test copy constructor
    MyLinkedHashMap<int, string> copied(original);
    cout << "Copied (copy constructor): ";
    copied.printInOrder();

    // Modify original - should not affect copy
    original.put(4, "four");
    original.put(1, "ONE");  // Update existing

    cout << "Original after modification: ";
    original.printInOrder();
    cout << "Copied after original modification: ";
    copied.printInOrder();

    // Test copy assignment
    MyLinkedHashMap<int, string> assigned;
    assigned.put(99, "temp");  // Put something first
    assigned = original;
    cout << "Assigned (copy assignment): ";
    assigned.printInOrder();

    // Test that they are independent
    EXPECT_EQ(original.size(), 4);
    EXPECT_EQ(copied.size(), 3);
    EXPECT_EQ(assigned.size(), 4);

    EXPECT_EQ(original.get(1), "ONE");
    EXPECT_EQ(copied.get(1), "one");  // Should still be original value
    EXPECT_EQ(assigned.get(1), "ONE");
}

TEST(MyLinkedHashMapTest, MoveSemantics)
{
    MyLinkedHashMap<int, string> original;
    original.put(1, "one");
    original.put(2, "two");
    original.put(3, "three");

    cout << "Original before move: ";
    original.printInOrder();

    // Test move constructor
    MyLinkedHashMap<int, string> moved(std::move(original));
    cout << "Moved (move constructor): ";
    moved.printInOrder();
    cout << "Original after move constructor: ";
    original.printInOrder();  // Should be empty

    // Test move assignment
    MyLinkedHashMap<int, string> move_assigned;
    move_assigned.put(99, "temp");
    move_assigned = std::move(moved);
    cout << "Move assigned: ";
    move_assigned.printInOrder();
    cout << "Moved after move assignment: ";
    moved.printInOrder();  // Should be empty

    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(moved.size(), 0);
    EXPECT_EQ(move_assigned.size(), 3);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}