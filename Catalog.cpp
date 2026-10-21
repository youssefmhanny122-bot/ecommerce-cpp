#include "Catalog.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>

static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return s;
}

Product& Catalog::addProduct(const std::string& name, double price, int stock) {
    products_.push_back(std::make_unique<Product>(nextId_, name, price, stock));
    nextId_++;
    return *products_.back();
}

Product* Catalog::findById(int id) {
    for (auto& p : products_) {
        if (p->getId() == id) return p.get();
    }
    return nullptr;
}

std::vector<Product*> Catalog::searchByName(const std::string& keyword) {
    std::vector<Product*> results;
    std::string key = toLower(keyword);
    for (auto& p : products_) {
        if (p->isActive() && toLower(p->getName()).find(key) != std::string::npos) {
            results.push_back(p.get());
        }
    }
    return results;
}

std::vector<Product*> Catalog::getActiveProducts() {
    std::vector<Product*> results;
    for (auto& p : products_) {
        if (p->isActive()) results.push_back(p.get());
    }
    return results;
}

bool Catalog::isEmpty() const { return products_.empty(); }

bool Catalog::saveToFile(const std::string& path) const {
    std::ofstream out(path);
    if (!out) return false;
    out << std::setprecision(15);
    for (const auto& p : products_) {
        out << p->getId() << '|' << p->getName() << '|' << p->getPrice()
            << '|' << p->getStock() << '|' << (p->isActive() ? 1 : 0) << '\n';
    }
    return out.good();
}

bool Catalog::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) return false;

    std::vector<std::unique_ptr<Product>> loaded;
    int maxId = 0;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string idStr, name, priceStr, stockStr, activeStr;
        if (!std::getline(ss, idStr, '|') || !std::getline(ss, name, '|') ||
            !std::getline(ss, priceStr, '|') || !std::getline(ss, stockStr, '|') ||
            !std::getline(ss, activeStr, '|')) {
            continue;
        }
        try {
            int id = std::stoi(idStr);
            double price = std::stod(priceStr);
            int stock = std::stoi(stockStr);
            auto product = std::make_unique<Product>(id, name, price, stock);
            if (activeStr == "0") product->deactivate();
            maxId = std::max(maxId, id);
            loaded.push_back(std::move(product));
        } catch (...) {
            continue;
        }
    }

    products_ = std::move(loaded);
    nextId_ = maxId + 1;
    return true;
}
