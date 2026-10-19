#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Product.h"

class Catalog {
public:
    Product& addProduct(const std::string& name, double price, int stock);
    Product* findById(int id);
    std::vector<Product*> searchByName(const std::string& keyword);
    std::vector<Product*> getActiveProducts();

    bool saveToFile(const std::string& path) const;
    bool loadFromFile(const std::string& path);
    bool isEmpty() const;

private:
    std::vector<std::unique_ptr<Product>> products_;
    int nextId_ = 1;
};
