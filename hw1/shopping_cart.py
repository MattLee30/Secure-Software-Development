from dataclasses import dataclass
import uuid
from types import MappingProxyType
import re
import catalog
from validator import ensure


@dataclass(frozen=True)
class Quantity:
    def __init__(self, value):
        ensure(isinstance(value, int), "Quantity must be an integer")
        ensure(value > 0 and value < 100, "Quantity must be between 1 and 99")
        self.value = value

@dataclass(frozen=True)
class CustomerID():
    def __init__(self, value):

        ensure(isinstance(value, str), "Customer ID must be a string")
        ensure(re.match(r'^[A-Za-z]{3}\d{5}[A-Za-z]{2}-[AQ]$', value), "Customer ID must be in the format 'AAA12345AA-AQ'")
        self.value = value

@dataclass(frozen=True)
class ItemID():
    def __init__(self, value):
        ensure(isinstance(value, str), "Item ID must be a string")
        ensure(re.match(r'^[A-Za-z]+$', value), "Item ID must contain only letters")
        ensure(len(value) > 0 and len(value) < 50, "Item ID must be between 1 and 50 characters")
        self.value = value

class ShoppingCart:

    def __init__(self, customer_id):
        self.cart_id = str(uuid.uuid4())
        self.customer_id = CustomerID(customer_id).value
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
        item_id = ItemID(item_id).value

        catalog.ensure_item_exists(item_id)
        quantity = Quantity(quantity).value

        if(item_id in self.items):
            self.items[item_id] += quantity
        else:
            self.items[item_id] = quantity
    
    def remove_item(self, item_id, quantity):
        item_id = ItemID(item_id).value
        ensure(item_id in self.items, f"Item {item_id} not found in cart")
        quantity = Quantity(quantity).value

        if quantity >= self.items[item_id]:
            del self.items[item_id]
        else:
            self.items[item_id] -= quantity

    def update_item_quantity(self, item_id, quantity):
        item_id = ItemID(item_id).value
        catalog.ensure_item_exists(item_id)
        quantity = Quantity(quantity).value
        
        self.items[item_id] = quantity

    def calculate_total(self):
        total = 0
        for item_id, quantity in self.items.items():
            price = catalog.get_item_price(item_id)
            total += price * quantity
        return total