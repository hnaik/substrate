from dataclasses import dataclass

from substrate.types import OrderStatus


@dataclass
class ExecutionResponse:
    price: float
    qty: int
    status: OrderStatus


@dataclass
class Fill:
    price: float
    qty: int
