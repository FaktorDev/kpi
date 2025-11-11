'use strict';

class TodoItem {
    constructor(id, userId, title, completed) {
        this.id = id;
        this.userId = userId;
        this.title = title;
        this.completed = completed;
    }
}

class TaskRepository {
    async getTasksAsync() {
        throw new Error("Not implemented");
    }
}

class InMemoryTaskRepository extends TaskRepository {
    constructor() {
        super();
        this._tasks = [
            new TodoItem(1, 1, "Learn Python", true),
            new TodoItem(2, 1, "Learn C#", false),
            new TodoItem(3, 2, "Write game", true),
        ];
    }

    async getTasksAsync() {
        await new Promise(r => setTimeout(r, 100)); // ³ì³òàö³ÿ IO
        return [...this._tasks];
    }
}

class TaskService {
    constructor(repo) {
        this._repo = repo;
    }

    async getCompletedTitlesByUserAsync(userId) {
        const tasks = await this._repo.getTasksAsync();

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

async function printReport(repo) {
    const service = new TaskService(repo);
    const result = await service.getCompletedTitlesByUserAsync(1);
    console.log("Report:", result);
}

printReport(new InMemoryTaskRepository());
