#pragma once
#include <string>
#include <vector>

struct OrderItem {
    int productId;
    std::string name;
    double unitPrice;
    int quantity;
};

enum class OrderStatus { Confirmed, Cancelled };

class Order {
public:
    Order(int id, std::vector<OrderItem> items,
          OrderStatus status = OrderStatus::Confirmed);

    int getId() const;
    const std::vector<OrderItem>& getItems() const;
    double getTotal() const;
    OrderStatus getStatus() const;

private:
    int id_;
    std::vector<OrderItem> items_;
    double total_;
    OrderStatus status_;
};
