#pragma once
#include <vector>
#include "Product.h"

struct CartItem {
    Product* product;
    int quantity;
};

class Cart {
public:
    bool addItem(Product& product, int quantity);
    bool updateQuantity(int productId, int quantity);
    bool removeItem(int productId);
    void clear();

    double getTotal() const;
    bool isEmpty() const;
    const std::vector<CartItem>& getItems() const;

private:
    std::vector<CartItem> items_;
    CartItem* find(int productId);
};
