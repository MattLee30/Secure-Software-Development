from dataclasses import dataclass
import uuid
from types import MappingProxyType
import re
from catalog import Catalog
from validator import ensure


@dataclass(frozen=True)
class Quantity:
    value : int

    @staticmethod
    def validate(value):
        ensure(value > 0 and value < 100, "Quantity must be between 1 and 99")
        return value

@dataclass(frozen=True)
class CustomerID():
    value : str

    @staticmethod
    def validate(value):
        ensure(re.match(r'^[A-Za-z]{3}\d{5}[A-Za-z]{2}-[AQ]{2}$', value), "Customer ID must be in the format 'AAA12345AA-AQ'")     
        return value   

@dataclass(frozen=True)
class ItemID():
    value : str

    @staticmethod
    def validate(value):
        ensure(re.match(r'^[A-Za-z]+$', value), "Item ID must contain only letters")
        ensure(len(value) > 0 and len(value) < 50, "Item ID must be between 1 and 50 characters")
        return value

class ShoppingCart:

    def __init__(self, customer_id):
        self.cart_id = str(uuid.uuid4())
        self.customer_id = CustomerID.validate(customer_id)
        self.items = {}
    
    @property
    def get_cart_id(self):
        return self.cart_id
    
    @property
    def get_customer_id(self):
        return self.customer_id
    
    def get_cart_items(self):
        return MappingProxyType(self.items)
    
    def add_item(self, item_id, quantity):
        item_id = ItemID.validate(item_id)
        Catalog.ensure_item_exists(item_id)
        quantity = Quantity.validate(quantity)

        if(item_id in self.items):
            self.items[item_id] += quantity
        else:
            self.items[item_id] = quantity
    
    def remove_item(self, item_id, quantity):
        item_id = ItemID.validate(item_id)
        ensure(item_id in self.items, f"Item {item_id} not found in cart")
        quantity = Quantity.validate(quantity)

        if quantity >= self.items[item_id]:
            del self.items[item_id]
        else:
            self.items[item_id] -= quantity

    def update_item_quantity(self, item_id, quantity):
        item_id = ItemID.validate(item_id)
        Catalog.ensure_item_exists(item_id)
        quantity = Quantity.validate(quantity)
        
        self.items[item_id] = quantity

    def calculate_total(self):
        total = 0
        for item_id, quantity in self.items.items():
            price = Catalog.get_price(item_id)
            total += price * quantity
        return total