#include <iostream>

struct Point
{
    double x;
	double y;
};
struct DoubleConnectedList
{
    struct Node
    {
        Point data;
        Node* prev;
        Node* next;
    };
    Node* head;
    Node* tail;
    DoubleConnectedList() : head(nullptr), tail(nullptr) {}

    void append(Point value)
    {
        Node* newNode = new Node{value, tail, nullptr};
        if (tail)
            tail->next = newNode;
        tail = newNode;
        if (!head)
            head = newNode;
    }
    void displayForward()
    {
        Node* current = head;
        while (current)
        {
            std::cout << "x: " << current->data.x << " y: " << current->data.y << std::endl;
            current = current->next;
        }
        std::cout << std::endl;
    }
    void displayBackward()
    {
        Node* current = tail;
        while (current)
        {
            std::cout << "x: " << current->data.x << " y: " << current->data.y << std::endl;
            current = current->prev;
        }
        std::cout << std::endl;
    }
    void deleteFirst()
    {
        if (!head)
            return;
        Node* temp = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
        delete temp;
	}
    void deleteAll()
    {
        Node* current = head;
        while (current)
        {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
	}
    void sortByDecreaseSumOfSquares()
    {
        if (!head || !head->next)
            return;
        bool swapped;
        do
        {
            swapped = false;
            Node* current = head;
            while (current->next)
            {
                double sum1 = current->data.x * current->data.x + current->data.y * current->data.y;
                double sum2 = current->next->data.x * current->next->data.x + current->next->data.y * current->next->data.y;
                if (sum1 < sum2)
                {
                    std::swap(current->data, current->next->data);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
	}
};

int main()
{
    std::cout << "TEST 1: Empty list" << std::endl;
    {
        DoubleConnectedList list;

        list.displayForward();
        list.displayBackward();
        list.deleteFirst();
        list.deleteAll();
        list.sortByDecreaseSumOfSquares();
    }

    std::cout << "\nTEST 2: Append one element" << std::endl;
    {
        DoubleConnectedList list;
        list.append({ 1, 2 });

        list.displayForward();
        list.displayBackward();
    }

    std::cout << "\nTEST 3: Append several elements" << std::endl;
    {
        DoubleConnectedList list;
        list.append({ 1, 1 });
        list.append({ 2, 2 });
        list.append({ 3, 3 });

        list.displayForward();
        list.displayBackward();
    }

    std::cout << "\nTEST 4: Delete first element (several nodes)" << std::endl;
    {
        DoubleConnectedList list;
        list.append({ 5, 5 });
        list.append({ 6, 6 });
        list.append({ 7, 7 });

        list.deleteFirst();
        list.displayForward();
    }

    std::cout << "\nTEST 5: Delete first element (one node)" << std::endl;
    {
        DoubleConnectedList list;
        list.append({ 10, 10 });

        list.deleteFirst();

        list.displayForward();
        list.displayBackward();
    }

    std::cout << "\nTEST 6: Delete all elements" << std::endl;
    {
        DoubleConnectedList list;
        list.append({ 1, 2 });
        list.append({ 3, 4 });
        list.append({ 5, 6 });

        list.deleteAll();

        list.displayForward();
        list.displayBackward();
    }

    std::cout << "\nTEST 7: Sorting simple case" << std::endl;
    {
        DoubleConnectedList list;

        list.append({ 1, 1 });  // sum = 2
        list.append({ 3, 4 });  // sum = 25
        list.append({ 0, 2 });  // sum = 4

        list.sortByDecreaseSumOfSquares();
        list.displayForward();
    }

    std::cout << "\nTEST 8: Sorting identical elements" << std::endl;
    {
        DoubleConnectedList list;

        list.append({ 2, 2 });
        list.append({ 2, 2 });
        list.append({ 2, 2 });

        list.sortByDecreaseSumOfSquares();
        list.displayForward();
    }

    std::cout << "\nTEST 9: Sorting one element" << std::endl;
    {
        DoubleConnectedList list;
        list.append({ 7, 7 });

        list.sortByDecreaseSumOfSquares();
        list.displayForward();
    }

    std::cout << "\nTEST 10: Mixed operations" << std::endl;
    {
        DoubleConnectedList list;

        list.append({ 1,1 });
        list.append({ 4,5 });
        list.append({ 2,2 });

        list.displayForward();

        list.sortByDecreaseSumOfSquares();
        list.displayForward();

        list.deleteFirst();
        list.displayForward();

        list.deleteAll();
        list.displayForward();
    }

    return 0;
}


