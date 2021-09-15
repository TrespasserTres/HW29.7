#include <iostream>
#include <thread>
#include <shared_mutex>

using namespace std;

struct Node
{
    int value;
    Node* next;
    Node(int _val) : value(_val), next(nullptr) {}
};

class FineGrainedQueue
{
    Node* head;
    Node* last;
    mutex queue_mutex;
public:
    FineGrainedQueue () : head(nullptr), last(nullptr) {}

    void insertIntoMiddle(int value, int pos)
    {
        // создаем новый узел 
        Node* newNode = new Node(value);
        lock_guard<mutex> l(queue_mutex);
        if (head == nullptr)
        {
            // если список пуст, новый узел и будет началом списка
            head = newNode;
            return;
        }

        if (pos == 0)
        {
            // крайний случай - вставка  в начало списка
            newNode->next = head;
            head = newNode;
            return;
        }

        int currPos = 0;

        Node* current = head;
        // в цикле идем по списку, пока список не кончится, или пока не дойдем до позиции
        while (currPos < pos - 1 && current->next != nullptr)
        {
            current = current->next;
            currPos++;
        }
        // меняем указатель на следующий узел на указатель на новый узел
        Node* next = current->next;
        current->next = newNode;
        // связываем список обратно, меняем указатель на узел, следующий после нового узла, на указатель на узел, следующий за current
        newNode->next = next;
    }

    bool is_empty()
    {
        return head == nullptr;
    }

    void print()
    {
        lock_guard<mutex> l(queue_mutex);
        if (is_empty()) return;
        Node* p = head;
        while (p)
        {
            cout << p->value << " ";
            p = p->next;
        }
        cout << endl;
    }
};

int main()
{
    FineGrainedQueue testing;
    thread t1, t2, t3, t4;
    t1 = thread(&FineGrainedQueue::insertIntoMiddle, &testing, 98, 0);
    t2 = thread(&FineGrainedQueue::insertIntoMiddle, &testing, 96, 0);
    t3 = thread(&FineGrainedQueue::insertIntoMiddle, &testing, 99, 5);
    t4 = thread(&FineGrainedQueue::insertIntoMiddle, &testing, 97, 1);
    if (t1.joinable())
    t1.join();
    if (t2.joinable())
    t2.join();
    if (t3.joinable())
    t3.join();
    if (t4.joinable())
    t4.join();
    testing.print();
    return 0;
}