from dataclasses import dataclass

from substrate.types import OrderSide


@dataclass
class QuoteSnapshot:
    price: float
    size: int


@dataclass
class BookSnapshot:
    side: OrderSide
    qty: int
    bid: QuoteSnapshot
    ask: QuoteSnapshot
