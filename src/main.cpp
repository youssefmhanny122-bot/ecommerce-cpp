#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include "Cart.h"
#include "Catalog.h"
#include "OrderService.h"

using namespace std;

// ---------- دوال الإدخال ----------
static string readLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        cout << "\nGoodbye!\n";
        exit(0);
    }
    return line;
}

static bool readInt(const string& prompt, int& value) {
    string line = readLine(prompt);
    try {
        size_t pos;
        value = stoi(line, &pos);
        if (pos != line.size()) throw invalid_argument("extra characters");
        return true;
    } catch (...) {
        cout << "Invalid number.\n";
        return false;
    }
}

static bool readDouble(const string& prompt, double& value) {
    string line = readLine(prompt);
    try {
        size_t pos;
        value = stod(line, &pos);
        if (pos != line.size() || !isfinite(value)) throw invalid_argument("bad number");
        return true;
    } catch (...) {
        cout << "Invalid number.\n";
        return false;
    }
}

// ---------- دوال العرض ----------
static void printProducts(const vector<Product*>& list) {
    if (list.empty()) {
        cout << "No products found.\n";
        return;
    }
    cout << fixed << setprecision(2);
    for (auto* p : list) {
        cout << "#" << p->getId() << "  " << p->getName()
             << "  -  " << p->getPrice()
             << "  (stock: " << p->getStock() << ")\n";
    }
}

static void printCart(const Cart& cart) {
    if (cart.isEmpty()) {
        cout << "Your cart is empty.\n";
        return;
    }
    cout << fixed << setprecision(2);
    for (const auto& item : cart.getItems()) {
        cout << "#" << item.product->getId() << "  " << item.product->getName()
             << "  x" << item.quantity
             << "  =  " << item.product->getPrice() * item.quantity << "\n";
    }
    cout << "Total: " << cart.getTotal() << "\n";
}

static void printOrder(const Order& order) {
    cout << fixed << setprecision(2);
    cout << "Order #" << order.getId() << " ["
         << (order.getStatus() == OrderStatus::Confirmed ? "Confirmed" : "Cancelled")
         << "]\n";
    for (const auto& item : order.getItems()) {
        cout << "   " << item.name << "  x" << item.quantity
             << "  @ " << item.unitPrice << "\n";
    }
    cout << "   Total: " << order.getTotal() << "\n";
}

static void printOrders(const OrderService& service) {
    if (service.getOrders().empty()) {
        cout << "No orders yet.\n";
        return;
    }
    for (const auto& order : service.getOrders()) printOrder(order);
}

// ---------- قائمة العميل ----------
static void customerMenu(Catalog& catalog, Cart& cart, OrderService& service) {
    while (true) {
        cout << "\n--- Customer Menu ---\n"
             << "1) Browse products\n"
             << "2) Search products\n"
             << "3) Add to cart\n"
             << "4) View cart\n"
             << "5) Update cart quantity\n"
             << "6) Remove from cart\n"
             << "7) Checkout\n"
             << "8) My orders\n"
             << "0) Back\n";
        int choice;
        if (!readInt("Choose: ", choice)) continue;

        if (choice == 0) return;

        if (choice == 1) {
            printProducts(catalog.getActiveProducts());
        } else if (choice == 2) {
            string keyword = readLine("Search for: ");
            printProducts(catalog.searchByName(keyword));
        } else if (choice == 3) {
            int id, qty;
            if (!readInt("Product id: ", id) || !readInt("Quantity: ", qty)) continue;
            Product* p = catalog.findById(id);
            if (!p || !p->isActive()) {
                cout << "Product not found.\n";
            } else if (cart.addItem(*p, qty)) {
                cout << "Added to cart.\n";
            } else {
                cout << "Could not add. Check the quantity and available stock.\n";
            }
        } else if (choice == 4) {
            printCart(cart);
        } else if (choice == 5) {
            int id, qty;
            if (!readInt("Product id: ", id) || !readInt("New quantity: ", qty)) continue;
            cout << (cart.updateQuantity(id, qty) ? "Updated.\n"
                                                  : "Could not update. Check the id, quantity and stock.\n");
        } else if (choice == 6) {
            int id;
            if (!readInt("Product id: ", id)) continue;
            cout << (cart.removeItem(id) ? "Removed.\n" : "That item is not in your cart.\n");
        } else if (choice == 7) {
            auto orderId = service.checkout(cart);
            if (orderId) {
                cout << "Checkout successful!\n";
                printOrder(*service.findOrder(*orderId));
            } else {
                cout << "Checkout failed. The cart is empty or some items are unavailable.\n";
            }
        } else if (choice == 8) {
            printOrders(service);
        } else {
            cout << "Unknown option.\n";
        }
    }
}

// ---------- قائمة الأدمن ----------
static void adminMenu(Catalog& catalog, OrderService& service) {
    while (true) {
        cout << "\n--- Admin Menu ---\n"
             << "1) View catalog\n"
             << "2) Add product\n"
             << "3) Update product price\n"
             << "4) Add stock\n"
             << "5) Deactivate product\n"
             << "6) View all orders\n"
             << "0) Back\n";
        int choice;
        if (!readInt("Choose: ", choice)) continue;

        if (choice == 0) return;

        try {
            if (choice == 1) {
                printProducts(catalog.getActiveProducts());
            } else if (choice == 2) {
                string name = readLine("Name: ");
                double price;
                int stock;
                if (!readDouble("Price: ", price) || !readInt("Stock: ", stock)) continue;
                Product& p = catalog.addProduct(name, price, stock);
                cout << "Product added with id " << p.getId() << ".\n";
            } else if (choice == 3) {
                int id;
                double price;
                if (!readInt("Product id: ", id) || !readDouble("New price: ", price)) continue;
                Product* p = catalog.findById(id);
                if (!p) { cout << "Product not found.\n"; continue; }
                p->setPrice(price);
                cout << "Price updated.\n";
            } else if (choice == 4) {
                int id, qty;
                if (!readInt("Product id: ", id) || !readInt("Quantity to add: ", qty)) continue;
                Product* p = catalog.findById(id);
                if (!p) { cout << "Product not found.\n"; continue; }
                p->addStock(qty);
                cout << "Stock updated. New stock: " << p->getStock() << "\n";
            } else if (choice == 5) {
                int id;
                if (!readInt("Product id: ", id)) continue;
                Product* p = catalog.findById(id);
                if (!p) { cout << "Product not found.\n"; continue; }
                p->deactivate();
                cout << "Product deactivated.\n";
            } else if (choice == 6) {
                printOrders(service);
            } else {
                cout << "Unknown option.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

// ---------- البرنامج الرئيسي ----------
static const string PRODUCTS_FILE = "data/products.txt";
static const string ORDERS_FILE = "data/orders.txt";

int main() {
    Catalog catalog;
    Cart cart;
    OrderService service;

    if (!catalog.loadFromFile(PRODUCTS_FILE) || catalog.isEmpty()) {
        catalog.addProduct("Laptop", 15000.0, 5);
        catalog.addProduct("Gaming Mouse", 250.0, 10);
        catalog.addProduct("Keyboard", 600.0, 7);
    } else {
        cout << "Loaded products from " << PRODUCTS_FILE << "\n";
    }

    if (service.loadFromFile(ORDERS_FILE)) {
        cout << "Loaded orders from " << ORDERS_FILE << "\n";
    }

    cout << "Welcome to the E-Commerce System\n";
    while (true) {
        cout << "\n=== Main Menu ===\n"
             << "1) Customer\n"
             << "2) Admin\n"
             << "0) Exit\n";
        int choice;
        if (!readInt("Choose: ", choice)) continue;

        if (choice == 0) {
            if (catalog.saveToFile(PRODUCTS_FILE)) cout << "Products saved.\n";
            else cout << "Warning: could not save products.\n";
            if (service.saveToFile(ORDERS_FILE)) cout << "Orders saved.\n";
            else cout << "Warning: could not save orders.\n";
            cout << "Goodbye!\n";
            return 0;
        } else if (choice == 1) {
            customerMenu(catalog, cart, service);
        } else if (choice == 2) {
            adminMenu(catalog, service);
        } else {
            cout << "Unknown option.\n";
        }
    }
}
