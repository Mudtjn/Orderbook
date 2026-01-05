from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class Side(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    BUY: _ClassVar[Side]
    SELL: _ClassVar[Side]

class OrderType(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    GOOD_TILL_CANCEL: _ClassVar[OrderType]
    FILL_AND_KILL: _ClassVar[OrderType]
    FILL_OR_KILL: _ClassVar[OrderType]
    GOOD_FOR_DAY: _ClassVar[OrderType]
BUY: Side
SELL: Side
GOOD_TILL_CANCEL: OrderType
FILL_AND_KILL: OrderType
FILL_OR_KILL: OrderType
GOOD_FOR_DAY: OrderType

class AddMarketOrderRequest(_message.Message):
    __slots__ = ("side", "quantity")
    SIDE_FIELD_NUMBER: _ClassVar[int]
    QUANTITY_FIELD_NUMBER: _ClassVar[int]
    side: Side
    quantity: int
    def __init__(self, side: _Optional[_Union[Side, str]] = ..., quantity: _Optional[int] = ...) -> None: ...

class AddOrderRequest(_message.Message):
    __slots__ = ("side", "order_type", "price", "quantity")
    SIDE_FIELD_NUMBER: _ClassVar[int]
    ORDER_TYPE_FIELD_NUMBER: _ClassVar[int]
    PRICE_FIELD_NUMBER: _ClassVar[int]
    QUANTITY_FIELD_NUMBER: _ClassVar[int]
    side: Side
    order_type: OrderType
    price: int
    quantity: int
    def __init__(self, side: _Optional[_Union[Side, str]] = ..., order_type: _Optional[_Union[OrderType, str]] = ..., price: _Optional[int] = ..., quantity: _Optional[int] = ...) -> None: ...

class AddOrderResponse(_message.Message):
    __slots__ = ("order_id", "order_type", "side", "price", "initial_quantity", "filled_quantity")
    ORDER_ID_FIELD_NUMBER: _ClassVar[int]
    ORDER_TYPE_FIELD_NUMBER: _ClassVar[int]
    SIDE_FIELD_NUMBER: _ClassVar[int]
    PRICE_FIELD_NUMBER: _ClassVar[int]
    INITIAL_QUANTITY_FIELD_NUMBER: _ClassVar[int]
    FILLED_QUANTITY_FIELD_NUMBER: _ClassVar[int]
    order_id: int
    order_type: OrderType
    side: Side
    price: int
    initial_quantity: int
    filled_quantity: int
    def __init__(self, order_id: _Optional[int] = ..., order_type: _Optional[_Union[OrderType, str]] = ..., side: _Optional[_Union[Side, str]] = ..., price: _Optional[int] = ..., initial_quantity: _Optional[int] = ..., filled_quantity: _Optional[int] = ...) -> None: ...
