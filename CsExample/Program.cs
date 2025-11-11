using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace CsExample;

public class TodoItem
{
    public int Id { get; }
    public int UserId { get; }
    public string Title { get; }
    public bool Completed { get; }

    public TodoItem(int id, int userId, string title, bool completed)
    {
        Id = id;
        UserId = userId;
        Title = title;
        Completed = completed;
    }
}

public abstract class TaskRepository
{
    public abstract Task<List<TodoItem>> GetTasksAsync();
}

public class InMemoryTaskRepository : TaskRepository
{
    private readonly List<TodoItem> _tasks =
    [
        new TodoItem(1, 1, "Learn Python", true),
        new TodoItem(2, 1, "Learn C#", false),
        new TodoItem(3, 2, "Write game", true),
    ];

    public override async Task<List<TodoItem>> GetTasksAsync()
    {
        await Task.Delay(100); // імітація IO
        return new List<TodoItem>(_tasks);
    }
}

public class TaskService
{
    private readonly TaskRepository _repo;

    public TaskService(TaskRepository repo)
    {
        _repo = repo;
    }

    public async Task<Dictionary<int, List<string>>> GetCompletedTitlesByUserAsync(int userId)
    {
        var tasks = await _repo.GetTasksAsync();

        var titlesSet = new HashSet<string>(
            tasks
                .FindAll(t => t.UserId == userId && t.Completed)
                .ConvertAll(t => t.Title)
        );

        return new Dictionary<int, List<string>>
        {
            [userId] = new List<string>(titlesSet)
        };
    }
}

public static class Program
{
    public static async Task PrintReport(TaskRepository repo)
    {
        var service = new TaskService(repo);
        var result = await service.GetCompletedTitlesByUserAsync(1);
        Console.WriteLine("Report:");
        foreach (var kv in result)
        {
            Console.WriteLine($"User {kv.Key}: {string.Join(", ", kv.Value)}");
        }
    }

    public static async Task Main()
    {
        await PrintReport(new InMemoryTaskRepository());
    }
}
