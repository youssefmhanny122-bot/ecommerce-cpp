#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "Cart.h"
#include "Catalog.h"
#include "OrderService.h"

static int passed = 0;
static int failed = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (cond) {                                                          \
            ++passed;                                                        \
        } else {                                                             \
            ++failed;                                                        \
            std::cout << "FAIL (line " << __LINE__ << "): " << #cond << "\n"; \
        }                                                                    \
    } while (0)

template <typename F>
static bool throwsInvalidArgument(F f) {
    try {
        f();
    } catch (const std::invalid_argument&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

static void testProduct() {
    CHECK(throwsInvalidArgument([] { Product p(1, "A", -1, 1); }));
    CHECK(throwsInvalidArgument([] { Product p(1, "A", 1, -1); }));
    CHECK(throwsInvalidArgument([] { Product p(1, "", 1, 1); }));

    Product p(1, "Laptop", 100.0, 5);
    CHECK(p.removeStock(3));
    CHECK(p.getStock() == 2);
    CHECK(!p.removeStock(10));      // أكبر من المخزون
    CHECK(p.getStock() == 2);       // ما اتغيّر
    CHECK(!p.removeStock(0));
    CHECK(!p.removeStock(-1));
    CHECK(p.removeStock(2));        // حالة حدية: كل المخزون
    CHECK(p.getStock() == 0);
    CHECK(throwsInvalidArgument([&] { p.addStock(0); }));
    CHECK(throwsInvalidArgument([&] { p.setPrice(-5); }));
    CHECK(p.getPrice() == 100.0);
}

static void testCart() {
    Product laptop(1, "Laptop", 1000.0, 5);
    Product mouse(2, "Mouse", 50.0, 10);
    Cart cart;

    CHECK(cart.isEmpty());
    CHECK(cart.addItem(laptop, 2));
    CHECK(!cart.addItem(laptop, 4));   // 2 + 4 أكبر من 5
    CHECK(cart.addItem(laptop, 3));    // 2 + 3 تساوي 5 بالضبط
    CHECK(cart.getItems().size() == 1);
    CHECK(cart.getItems()[0].quantity == 5);
    CHECK(!cart.addItem(laptop, 1));
    CHECK(!cart.addItem(mouse, 0));
    CHECK(!cart.addItem(mouse, -2));
    CHECK(cart.addItem(mouse, 4));
    CHECK(cart.getTotal() == 5 * 1000.0 + 4 * 50.0);

    CHECK(!cart.updateQuantity(99, 1));
    CHECK(!cart.updateQuantity(2, 0));
    CHECK(!cart.updateQuantity(2, 11));
    CHECK(cart.updateQuantity(2, 1));

    CHECK(!cart.removeItem(99));
    CHECK(cart.removeItem(1));
    CHECK(cart.removeItem(2));
    CHECK(cart.isEmpty());

    Product old(3, "Old", 10.0, 5);
    old.deactivate();
    CHECK(!cart.addItem(old, 1));      // منتج موقوف
}

static void testCatalog() {
    Catalog c;
    CHECK(c.addProduct("Laptop", 15000.0, 5).getId() == 1);
    CHECK(throwsInvalidArgument([&] { c.addProduct("Bad", -1.0, 1); }));
    CHECK(c.addProduct("Gaming Mouse", 250.0, 10).getId() == 2);  // الـ id ما اتحرق

    CHECK(c.searchByName("MOUSE").size() == 1);   // بدون حساسية لحجم الحروف
    CHECK(c.searchByName("xyz").empty());
    CHECK(c.findById(99) == nullptr);

    c.findById(2)->deactivate();
    CHECK(c.searchByName("mouse").empty());
    CHECK(c.getActiveProducts().size() == 1);
}

static void testPersistence() {
    const std::string path = "test_products.tmp";

    Catalog a;
    a.addProduct("Laptop", 15000.5, 5);
    a.addProduct("Mouse", 250.0, 10);
    a.addProduct("Keyboard", 600.0, 7);
    a.addProduct("Pricey", 1234567.89, 1);
    a.findById(3)->deactivate();
    CHECK(a.saveToFile(path));

    Catalog b;
    CHECK(b.loadFromFile(path));
    CHECK(b.getActiveProducts().size() == 3);
    CHECK(b.findById(1)->getPrice() == 15000.5);
    CHECK(b.findById(4)->getPrice() == 1234567.89);
    CHECK(b.findById(3) != nullptr && !b.findById(3)->isActive());
    CHECK(b.addProduct("New", 1.0, 1).getId() == 5);

    Catalog missing;
    CHECK(!missing.loadFromFile("does_not_exist.tmp"));

    {
        std::ofstream out(path);
        out << "1|Good|10|5|1\n"
            << "garbage line\n"
            << "2|NegPrice|-5|1|1\n"
            << "3|Ok|20|2|1\n";
    }
    Catalog d;
    CHECK(d.loadFromFile(path));
    CHECK(d.getActiveProducts().size() == 2);   // السطرين التالفين اتجاهلوا

    std::remove(path.c_str());
}

static void testCheckout() {
    Product laptop(1, "Laptop", 15000.0, 5);
    Product mouse(2, "Mouse", 250.0, 10);
    Cart cart;
    OrderService service;

    CHECK(!service.checkout(cart).has_value());   // سلة فاضية

    cart.addItem(laptop, 2);
    cart.addItem(mouse, 1);
    auto id1 = service.checkout(cart);
    CHECK(id1.has_value() && id1.value_or(-1) == 1);
    CHECK(laptop.getStock() == 3);
    CHECK(mouse.getStock() == 9);
    CHECK(cart.isEmpty());

    const Order* order = service.findOrder(id1.value_or(-1));
    CHECK(order != nullptr && order->getTotal() == 30250.0);

    // الـ snapshot: تغيير السعر بعدين ما يأثر على الطلب القديم
    laptop.setPrice(99999.0);
    CHECK(service.findOrder(1)->getTotal() == 30250.0);
    CHECK(service.findOrder(1)->getItems()[0].unitPrice == 15000.0);
    CHECK(service.findOrder(99) == nullptr);

    // فشل بسبب تغيّر المخزون: ما يتغيّر أي شيء (لا خصم جزئي)
    cart.addItem(laptop, 3);
    cart.addItem(mouse, 2);
    mouse.removeStock(8);                         // مخزون الماوس بقى 1
    CHECK(!service.checkout(cart).has_value());
    CHECK(laptop.getStock() == 3);
    CHECK(mouse.getStock() == 1);
    CHECK(!cart.isEmpty());
    CHECK(service.getOrders().size() == 1);

    // نصلح السلة والـ checkout يعدّي
    CHECK(cart.updateQuantity(2, 1));
    auto id2 = service.checkout(cart);
    CHECK(id2.has_value() && id2.value_or(-1) == 2);
    CHECK(laptop.getStock() == 0);
    CHECK(mouse.getStock() == 0);
}

static void testOrderPersistence() {
    const std::string path = "test_orders.tmp";

    Product laptop(1, "Laptop", 15000.5, 10);
    Product mouse(2, "Mouse", 250.0, 10);
    Cart cart;
    OrderService a;
    cart.addItem(laptop, 2);
    cart.addItem(mouse, 3);
    a.checkout(cart);
    cart.addItem(mouse, 1);
    a.checkout(cart);
    CHECK(a.saveToFile(path));

    OrderService b;
    CHECK(b.loadFromFile(path));
    CHECK(b.getOrders().size() == 2);
    CHECK(b.findOrder(1) != nullptr);
    CHECK(b.findOrder(1)->getItems().size() == 2);
    CHECK(b.findOrder(1)->getTotal() == 2 * 15000.5 + 3 * 250.0);
    CHECK(b.findOrder(2)->getTotal() == 250.0);

    // الطلب الجديد بعد التحميل ياخد رقم 3 مش 1
    Cart cart2;
    cart2.addItem(mouse, 1);
    auto id = b.checkout(cart2);
    CHECK(id.has_value() && id.value_or(-1) == 3);

    OrderService missing;
    CHECK(!missing.loadFromFile("does_not_exist.tmp"));

    {
        std::ofstream out(path);
        out << "1|C|1|Laptop|100|2\n"
            << "garbage\n"
            << "2|C|1|Bad|-5|1\n"
            << "3|C|1|Bad|5|0\n";
    }
    OrderService d;
    CHECK(d.loadFromFile(path));
    CHECK(d.getOrders().size() == 1);

    std::remove(path.c_str());
}

int main() {
    testProduct();
    testCart();
    testCatalog();
    testPersistence();
    testCheckout();
    testOrderPersistence();

    std::cout << "Passed: " << passed << "  Failed: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}
