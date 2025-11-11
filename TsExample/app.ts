class TodoItem {
    constructor(
        public id: number,
        public userId: number,
        public title: string,
        public completed: boolean
    ) { }
}

abstract class TaskRepository {
    abstract getTasksAsync(): Promise<TodoItem[]>;
}

class InMemoryTaskRepository extends TaskRepository {
    private _tasks: TodoItem[] = [
        new TodoItem(1, 1, "Learn Python", true),
        new TodoItem(2, 1, "Learn C#", false),
        new TodoItem(3, 2, "Write game", true),
    ];

    async getTasksAsync(): Promise<TodoItem[]> {
        await new Promise<void>(r => setTimeout(r, 100));
        return [...this._tasks];
    }
}

class TaskService {
    constructor(private repo: TaskRepository) { }

    async getCompletedTitlesByUserAsync(
        userId: number
    ): Promise<Record<number, string[]>> {
        const tasks = await this.repo.getTasksAsync();

        const titlesSet = new Set(
            tasks
                .filter(t => t.userId === userId && t.completed)
                .map(t => t.title)
        );

        return {
            [userId]: Array.from(titlesSet)
        };
    }
}

async function printReport(repo: TaskRepository): Promise<void> {
    const service = new TaskService(repo);
    const result = await service.getCompletedTitlesByUserAsync(1);
    console.log("Report:", result);
}

printReport(new InMemoryTaskRepository());
