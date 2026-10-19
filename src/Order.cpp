#include "Order.h"

Order::Order(int id, std::vector<OrderItem> items, OrderStatus status)
    : id_(id), items_(std::move(items)), total_(0), status_(status) {
    for (const auto& item : items_) {
        total_ += item.unitPrice * item.quantity;
    }
}

int Order::getId() const { return id_; }
const std::vector<OrderItem>& Order::getItems() const { return items_; }
double Order::getTotal() const { return total_; }
OrderStatus Order::getStatus() const { return status_; }
