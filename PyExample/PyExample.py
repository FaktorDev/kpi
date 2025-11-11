
import asyncio
from abc import ABC, abstractmethod
from dataclasses import dataclass
from typing import List, Dict, Set


@dataclass
class TodoItem:
    id: int
    user_id: int
    title: str
    completed: bool


class TaskRepository(ABC):
    @abstractmethod
    async def get_tasks_async(self) -> List[TodoItem]:
        ...


class InMemoryTaskRepository(TaskRepository):
    def __init__(self) -> None:
        self._tasks: List[TodoItem] = [
            TodoItem(1, 1, "Learn Python", True),
            TodoItem(2, 1, "Learn C#", False),
            TodoItem(3, 2, "Write game", True),
        ]

    async def get_tasks_async(self) -> List[TodoItem]:
        await asyncio.sleep(0.1)  # ³ì³òàö³ÿ IO
        return list(self._tasks)


class TaskService:
    def __init__(self, repo: TaskRepository) -> None:
        self._repo = repo

    async def get_completed_titles_by_user_async(
        self, user_id: int
    ) -> Dict[int, List[str]]:
        tasks = await self._repo.get_tasks_async()

        titles_set: Set[str] = {
            t.title for t in tasks if t.user_id == user_id and t.completed
        }

        result: Dict[int, List[str]] = {user_id: sorted(titles_set)}
        return result


async def print_report(repo: TaskRepository) -> None:
    service = TaskService(repo)
    data = await service.get_completed_titles_by_user_async(1)
    print("Report:", data)


if __name__ == "__main__":
    asyncio.run(print_report(InMemoryTaskRepository()))
