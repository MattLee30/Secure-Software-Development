import pytest
from catalog import Catalog
from shopping_cart import ShoppingCart, Quantity, CustomerID, ItemID


def test_quantity_validate_accepts_valid_range():
	assert Quantity.validate(1) == 1
	assert Quantity.validate(99) == 99


@pytest.mark.parametrize("value", [0, 100, -1])
def test_quantity_validate_rejects_out_of_range(value):
	with pytest.raises(ValueError, match="Quantity must be between 1 and 99"):
		Quantity.validate(value)


def test_customer_id_validate_accepts_expected_format():
	assert CustomerID.validate("ABC12345DE-AQ") == "ABC12345DE-AQ"
	assert CustomerID.validate("xyz99999ZZ-QQ") == "xyz99999ZZ-QQ"


@pytest.mark.parametrize(
	"value",
	[
		"AB12345DE-AQ",   # too short prefix
		"ABCD12345DE-AQ", # too long prefix
		"ABC1234DE-AQ",   # digits too short
		"ABC123456DE-AQ", # digits too long
		"ABC12345D-AQ",   # suffix too short
		"ABC12345DEF-AQ", # suffix too long
		"ABC12345DE-XX",  # invalid end group
		"ABC12345DE-AZ",  # invalid end group
		"ABC12345DE-A1",  # invalid end group
	],
)
def test_customer_id_validate_rejects_invalid_format(value):
	with pytest.raises(ValueError, match="Customer ID must be in the format"):
		CustomerID.validate(value)


def test_item_id_validate_accepts_letters_only():
	assert ItemID.validate("apple") == "apple"
	assert ItemID.validate("BANANA") == "BANANA"


@pytest.mark.parametrize("value", ["", "a" * 50, "apple1", "apple-pear"]) 
def test_item_id_validate_rejects_invalid(value):
	with pytest.raises(ValueError):
		ItemID.validate(value)


def test_cart_initialization_sets_ids_and_empty_items():
	cart = ShoppingCart("ABC12345DE-AQ")
	assert cart.get_customer_id == "ABC12345DE-AQ"
	assert isinstance(cart.get_cart_id, str)
	assert cart.get_cart_items() == {}


def test_get_cart_items_returns_read_only_mapping():
	cart = ShoppingCart("ABC12345DE-AQ")
	cart.add_item("apple", 1)
	items_view = cart.get_cart_items
	with pytest.raises(TypeError):
		items_view["apple"] = 10


def test_add_item_rejects_nonexistent_catalog_item():
	cart = ShoppingCart("ABC12345DE-AQ")
	with pytest.raises(ValueError, match="not found in catalog"):
		cart.add_item("nonexistent", 1)


def test_remove_item_rejects_missing_item():
	cart = ShoppingCart("ABC12345DE-AQ")
	with pytest.raises(ValueError, match="not found in cart"):
		cart.remove_item("apple", 1)


def test_calculate_total_sums_prices_times_quantity():
	cart = ShoppingCart("ABC12345DE-AQ")
	cart.add_item("apple", 2)   # 0.5 * 2 = 1.0
	cart.add_item("bread", 1)   # 2.0 * 1 = 2.0
	cart.add_item("milk", 3)    # 1.5 * 3 = 4.5
	assert cart.calculate_total() == pytest.approx(7.5)


def test_catalog_upsert_item_affects_cart_pricing():
	Catalog.upsert_item("tea", 1.25)
	cart = ShoppingCart("ABC12345DE-AQ")
	cart.add_item("tea", 2)
	assert cart.calculate_total() == pytest.approx(2.5)

