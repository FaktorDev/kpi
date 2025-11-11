#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <future>
#include <chrono>
#include <thread>

struct TodoItem {
    int id;
    int userId;
    std::string title;
    bool completed;
};

class TaskRepository {
public:
    virtual std::future<std::vector<TodoItem>> getTasksAsync() = 0;
    virtual ~TaskRepository() = default;
};

class InMemoryTaskRepository : public TaskRepository {
    std::vector<TodoItem> tasks{
        {1, 1, "Learn Python", true},
        {2, 1, "Learn C#", false},
        {3, 2, "Write game", true}
    };

public:
    std::future<std::vector<TodoItem>> getTasksAsync() override {
        return std::async(std::launch::async, [this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return tasks; // копія
            });
    }
};

class TaskService {
    TaskRepository& repo;
public:
    explicit TaskService(TaskRepository& r) : repo(r) {}

    std::future<std::map<int, std::vector<std::string>>>
        getCompletedTitlesByUserAsync(int userId) {
        return std::async(std::launch::async, [this, userId]() {
            auto tasksFuture = repo.getTasksAsync();
            auto tasks = tasksFuture.get();

            std::set<std::string> titlesSet;
            for (const auto& t : tasks) {
                if (t.userId == userId && t.completed) {
                    titlesSet.insert(t.title);
                }
            }

            std::map<int, std::vector<std::string>> result;
            result[userId] = { titlesSet.begin(), titlesSet.end() };
            return result;
            });
    }
};

void printReport(TaskRepository& repo) {
    TaskService service(repo);
    auto futureResult = service.getCompletedTitlesByUserAsync(1);
    auto result = futureResult.get();

    std::cout << "Report:\n";
    for (const auto& [userId, titles] : result) {
        std::cout << "User " << userId << ": ";
        bool first = true;
        for (const auto& title : titles) {
            if (!first) std::cout << ", ";
            std::cout << title;
            first = false;
        }
        std::cout << '\n';
    }
}

int main() {
    InMemoryTaskRepository repo;
    printReport(repo);
}
