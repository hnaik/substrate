from abc import ABC, abstractmethod

from substrate.md import BookSnapshot


class MDBase(ABC):
    @abstractmethod
    def get_book_snapshot(self, book_id: str) -> BookSnapshot:
        pass
