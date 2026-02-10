from dataclasses import dataclass
from validator import ensure

@dataclass(frozen=True)
class Catalog:
    CATALOG = {
        "apple": 0.5,
        "banana": 0.3,
        "orange": 0.8,
        "milk": 1.5,
        "bread": 2.0,
    }

    @staticmethod
    def has_item(item):
        return item in Catalog.CATALOG

    @staticmethod
    def ensure_item_exists(item):
        ensure(Catalog.has_item(item), f"Item '{item}' not found in catalog")

    @staticmethod
    def get_price(item):
        Catalog.ensure_item_exists(item)
        return Catalog.CATALOG[item]
    
    @staticmethod
    def upsert_item(item, price):
        ensure(isinstance(item, str), f"Item name must be a string")
        ensure(isinstance(price, (int, float)) and price >= 0, f"Price must be a non-negative number")
        Catalog.CATALOG[item] = price