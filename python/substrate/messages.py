from dataclasses import dataclass

from substrate.types import OrderStatus


@dataclass
class ExecutionResponse:
    price: float
    size: int
    status: OrderStatus
