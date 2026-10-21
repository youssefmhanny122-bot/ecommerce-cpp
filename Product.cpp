#include "Product.h"
#include <stdexcept>

Product::Product(int id, std::string name, double price, int stock)
    : id_(id), name_(std::move(name)), price_(price), stock_(stock), active_(true) {
    if (name_.empty()) throw std::invalid_argument("Name cannot be empty");
    if (price_ < 0)    throw std::invalid_argument("Price cannot be negative");
    if (stock_ < 0)    throw std::invalid_argument("Stock cannot be negative");
}

int Product::getId() const { return id_; }
const std::string& Product::getName() const { return name_; }
double Product::getPrice() const { return price_; }
int Product::getStock() const { return stock_; }
bool Product::isActive() const { return active_; }

void Product::setPrice(double price) {
    if (price < 0) throw std::invalid_argument("Price cannot be negative");
    price_ = price;
}

void Product::addStock(int quantity) {
    if (quantity <= 0) throw std::invalid_argument("Quantity must be positive");
    stock_ += quantity;
}

bool Product::removeStock(int quantity) {
    if (quantity <= 0 || quantity > stock_) return false;
    stock_ -= quantity;
    return true;
}

void Product::deactivate() { active_ = false; }
