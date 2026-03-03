from abc import ABC, abstractmethod

from substrate.md.snapshot import BookSnapshot


class MDBase(ABC):
    @abstractmethod
    def get_book_snapshot(self, book_id: str) -> BookSnapshot:
        pass


class DefaultMarketData(MDBase):
    def get_book_snapshot(self, book_id: str) -> BookSnapshot:
        raise NotImplementedError(
            'DefaultMarketData does not implement get_book_snapshot'
        )
