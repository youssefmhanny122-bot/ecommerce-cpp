#pragma once
#include <optional>
#include <string>
#include <vector>
#include "Cart.h"
#include "Order.h"

class OrderService {
public:
    std::optional<int> checkout(Cart& cart);
    const Order* findOrder(int id) const;
    const std::vector<Order>& getOrders() const;

    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);

private:
    std::vector<Order> orders_;
    int nextId_ = 1;
};
