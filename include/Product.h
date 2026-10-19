#pragma once
#include <string>

class Product {
public:
    Product(int id, std::string name, double price, int stock);

    int getId() const;
    const std::string& getName() const;
    double getPrice() const;
    int getStock() const;
    bool isActive() const;

    void setPrice(double price);
    void addStock(int quantity);
    bool removeStock(int quantity);
    void deactivate();

private:
    int id_;
    std::string name_;
    double price_;
    int stock_;
    bool active_;
};
