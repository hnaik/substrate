from enum import Enum


class OrderSide(Enum):
    BUY = 'BUY'
    SELL = 'SELL'


class OrderStatus(Enum):
    NEW = 'NEW'
    PENDING = 'PENDING'
    FILLED = 'FILLED'
    PARTIALLY_FILLED = 'PARTIALLY_FILLED'
    CANCELED = 'CANCELED'
    REJECTED = 'REJECTED'
