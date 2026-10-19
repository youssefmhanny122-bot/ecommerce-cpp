#include "Cart.h"
#include <algorithm>

CartItem* Cart::find(int productId) {
    for (auto& item : items_) {
        if (item.product->getId() == productId) return &item;
    }
    return nullptr;
}

bool Cart::addItem(Product& product, int quantity) {
    if (quantity <= 0 || !product.isActive()) return false;

    CartItem* existing = find(product.getId());
    int newQuantity = quantity + (existing ? existing->quantity : 0);

    if (newQuantity > product.getStock()) return false;

    if (existing) existing->quantity = newQuantity;
    else items_.push_back({&product, quantity});
    return true;
}

bool Cart::updateQuantity(int productId, int quantity) {
    CartItem* item = find(productId);
    if (!item || quantity <= 0) return false;
    if (quantity > item->product->getStock()) return false;
    item->quantity = quantity;
    return true;
}

bool Cart::removeItem(int productId) {
    auto it = std::remove_if(items_.begin(), items_.end(),
        [productId](const CartItem& i) { return i.product->getId() == productId; });
    if (it == items_.end()) return false;
    items_.erase(it, items_.end());
    return true;
}

void Cart::clear() { items_.clear(); }

double Cart::getTotal() const {
    double total = 0;
    for (const auto& item : items_) {
        total += item.product->getPrice() * item.quantity;
    }
    return total;
}

bool Cart::isEmpty() const { return items_.empty(); }

const std::vector<CartItem>& Cart::getItems() const { return items_; }
