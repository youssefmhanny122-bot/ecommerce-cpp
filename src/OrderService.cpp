#include "OrderService.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>

std::optional<int> OrderService::checkout(Cart& cart) {
    if (cart.isEmpty()) return std::nullopt;

    for (const auto& item : cart.getItems()) {
        if (!item.product->isActive() || item.quantity > item.product->getStock()) {
            return std::nullopt;
        }
    }

    std::vector<OrderItem> snapshot;
    for (const auto& item : cart.getItems()) {
        snapshot.push_back({item.product->getId(), item.product->getName(),
                            item.product->getPrice(), item.quantity});
        item.product->removeStock(item.quantity);
    }

    int id = nextId_++;
    orders_.emplace_back(id, std::move(snapshot));
    cart.clear();
    return id;
}

const Order* OrderService::findOrder(int id) const {
    for (const auto& order : orders_) {
        if (order.getId() == id) return &order;
    }
    return nullptr;
}

const std::vector<Order>& OrderService::getOrders() const { return orders_; }

bool OrderService::saveToFile(const std::string& path) const {
    std::ofstream out(path);
    if (!out) return false;
    out << std::setprecision(15);
    for (const auto& order : orders_) {
        for (const auto& item : order.getItems()) {
            out << order.getId() << '|'
                << (order.getStatus() == OrderStatus::Confirmed ? "C" : "X") << '|'
                << item.productId << '|' << item.name << '|'
                << item.unitPrice << '|' << item.quantity << '\n';
        }
    }
    return out.good();
}

bool OrderService::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;

    // نجمّع البنود حسب رقم الطلب، والأسطر التالفة نتجاهلها
    std::map<int, std::vector<OrderItem>> grouped;
    std::map<int, OrderStatus> statuses;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string orderIdStr, status, productIdStr, name, priceStr, qtyStr;
        if (!std::getline(ss, orderIdStr, '|') || !std::getline(ss, status, '|') ||
            !std::getline(ss, productIdStr, '|') || !std::getline(ss, name, '|') ||
            !std::getline(ss, priceStr, '|') || !std::getline(ss, qtyStr, '|')) {
            continue;
        }
        try {
            int orderId = std::stoi(orderIdStr);
            int productId = std::stoi(productIdStr);
            double price = std::stod(priceStr);
            int qty = std::stoi(qtyStr);
            if (orderId <= 0 || price < 0 || qty <= 0) continue;
            grouped[orderId].push_back({productId, name, price, qty});
            statuses[orderId] = (status == "X") ? OrderStatus::Cancelled
                                                : OrderStatus::Confirmed;
        } catch (...) {
            continue;
        }
    }

    orders_.clear();
    int maxId = 0;
    for (auto& [id, items] : grouped) {
        orders_.emplace_back(id, std::move(items), statuses[id]);
        maxId = std::max(maxId, id);
    }
    nextId_ = maxId + 1;
    return true;
}
