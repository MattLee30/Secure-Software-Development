import pytest
import shopping_cart


@pytest.fixture
def cart():
    return shopping_cart.ShoppingCart("AAA12345AA-AQ")


def test_add_item(cart):
    cart.add_item("apple", 1.50, 2)
    assert len(cart.items) == 1
    assert cart.items[0]["name"] == "apple"


def test_remove_item(cart):
    cart.add_item("apple", 1.50, 2)
    cart.remove_item("apple")
    assert len(cart.items) == 0


def test_calculate_total(cart):
    cart.add_item("apple", 1.50, 2)
    cart.add_item("banana", 0.75, 3)
    total = cart.calculate_total()
    assert total == 5.25


def test_empty_cart(cart):
    cart.add_item("apple", 1.50, 2)
    cart.empty_cart()
    assert len(cart.items) == 0


def test_empty_cart_total(cart):
    total = cart.calculate_total()
    assert total == 0