#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <iomanip>

using namespace std;

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
            cout << "x: " << current->data.x << " y: " << current->data.y << endl;
            current = current->next;
        }
        cout << endl;
    }
    void displayBackward()
    {
        Node* current = tail;
        while (current)
        {
            cout << "x: " << current->data.x << " y: " << current->data.y << endl;
            current = current->prev;
        }
        cout << endl;
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
                    swap(current->data, current->next->data);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
	}
};

void test_sorting()
{
    std::cout << "Enter number of test cases: ";
    int testCount;
    std::cin >> testCount;

    struct TestCase { int n; int iterations; };
    std::vector<TestCase> cases;

    for (int i = 0; i < testCount; i++)
    {
        TestCase tc;
        std::cout << "\nTest case " << (i + 1) << "\n";
        std::cout << "Enter number of elements: ";
        std::cin >> tc.n;
        std::cout << "Enter number of iterations: ";
        std::cin >> tc.iterations;

        cases.push_back(tc);
    }

    std::cout << "\nStarting performance tests...\n\n";

    // зелений колір
    const char* green = "\033[32m";
    const char* reset = "\033[0m";

    struct Result { int n; int iterations; double ms; };
    std::vector<Result> results;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(-1000.0, 1000.0);

    int totalSteps = 0;
    for (auto& c : cases)
        totalSteps += c.iterations;

    int completed = 0;

    for (auto& tc : cases)
    {
        double totalTime = 0;

        for (int it = 0; it < tc.iterations; it++)
        {
            DoubleConnectedList list;
            for (int i = 0; i < tc.n; i++)
            {
                Point p{ dist(gen), dist(gen) };
                list.append(p);
            }

            auto start = std::chrono::high_resolution_clock::now();
            list.sortByDecreaseSumOfSquares();
            auto end = std::chrono::high_resolution_clock::now();

            double elapsedMs =
                std::chrono::duration<double, std::milli>(end - start).count();

            totalTime += elapsedMs;
            list.deleteAll();

            completed++;
            double percent = (completed * 100.0) / totalSteps;

            std::cout << green << "Progress: "
                << std::fixed << std::setprecision(1)
                << percent << "%\r" << reset;

            std::cout.flush();
        }

        double avgMs = totalTime / tc.iterations;
        results.push_back({ tc.n, tc.iterations, avgMs });
    }

    std::cout << "\n\n===========================================\n";
    std::cout << " Performance Results\n";
    std::cout << "===========================================\n";

    std::cout << std::left
        << std::setw(15) << "Elements"
        << std::setw(15) << "Iterations"
        << std::setw(15) << "Avg Time (ms)"
        << "\n";

    for (auto& r : results)
    {
        std::cout << std::left
            << std::setw(15) << r.n
            << std::setw(15) << r.iterations
            << std::setw(15) << r.ms
            << "\n";
    }
}

int main()
{
    cout << "TEST 1: Empty list" << endl;
    {
        DoubleConnectedList list;

        list.displayForward();
        list.displayBackward();
        list.deleteFirst();
        list.deleteAll();
        list.sortByDecreaseSumOfSquares();
    }

    cout << "\nTEST 2: Append one element" << endl;
    {
        DoubleConnectedList list;
        list.append({ 1, 2 });

        list.displayForward();
        list.displayBackward();
    }

    cout << "\nTEST 3: Append several elements" << endl;
    {
        DoubleConnectedList list;
        list.append({ 1, 1 });
        list.append({ 2, 2 });
        list.append({ 3, 3 });

        list.displayForward();
        list.displayBackward();
    }

    cout << "\nTEST 4: Delete first element (several nodes)" << endl;
    {
        DoubleConnectedList list;
        list.append({ 5, 5 });
        list.append({ 6, 6 });
        list.append({ 7, 7 });

        list.deleteFirst();
        list.displayForward();
    }

    cout << "\nTEST 5: Delete first element (one node)" << endl;
    {
        DoubleConnectedList list;
        list.append({ 10, 10 });

        list.deleteFirst();

        list.displayForward();
        list.displayBackward();
    }

    cout << "\nTEST 6: Delete all elements" << endl;
    {
        DoubleConnectedList list;
        list.append({ 1, 2 });
        list.append({ 3, 4 });
        list.append({ 5, 6 });

        list.deleteAll();

        list.displayForward();
        list.displayBackward();
    }

    cout << "\nTEST 7: Sorting simple case" << endl;
    {
        DoubleConnectedList list;

        list.append({ 1, 1 });  // sum = 2
        list.append({ 3, 4 });  // sum = 25
        list.append({ 0, 2 });  // sum = 4

        list.sortByDecreaseSumOfSquares();
        list.displayForward();
    }

    cout << "\nTEST 8: Sorting identical elements" << endl;
    {
        DoubleConnectedList list;

        list.append({ 2, 2 });
        list.append({ 2, 2 });
        list.append({ 2, 2 });

        list.sortByDecreaseSumOfSquares();
        list.displayForward();
    }

    cout << "\nTEST 9: Sorting one element" << endl;
    {
        DoubleConnectedList list;
        list.append({ 7, 7 });

        list.sortByDecreaseSumOfSquares();
        list.displayForward();
    }

    cout << "\nTEST 10: Mixed operations" << endl;
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

    test_sorting();

    return 0;
}


