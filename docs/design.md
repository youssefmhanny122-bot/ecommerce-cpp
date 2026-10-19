# Design

## Class diagram

~~~mermaid
classDiagram
    class Product {
        -int id_
        -string name_
        -double price_
        -int stock_
        -bool active_
        +setPrice(double)
        +addStock(int)
        +removeStock(int) bool
        +deactivate()
    }
    class CartItem {
        +Product* product
        +int quantity
    }
    class Cart {
        -vector~CartItem~ items_
        +addItem(Product, int) bool
        +updateQuantity(int, int) bool
        +removeItem(int) bool
        +getTotal() double
        +clear()
    }
    class Catalog {
        -vector~unique_ptr~Product~~ products_
        +addProduct(name, price, stock) Product
        +findById(int) Product*
        +searchByName(string) vector
        +saveToFile(path) bool
        +loadFromFile(path) bool
    }
    class OrderItem {
        +int productId
        +string name
        +double unitPrice
        +int quantity
    }
    class Order {
        -int id_
        -double total_
        -OrderStatus status_
        +getTotal() double
    }
    class OrderService {
        -vector~Order~ orders_
        +checkout(Cart) optional~int~
        +findOrder(int) Order*
        +saveToFile(path) bool
        +loadFromFile(path) bool
    }

    Catalog "1" *-- "many" Product : owns
    Cart "1" o-- "many" CartItem : contains
    CartItem --> Product : refers to
    Order "1" *-- "many" OrderItem : snapshot
    OrderService "1" *-- "many" Order : stores
    OrderService ..> Cart : reads and clears
    OrderService ..> Product : reduces stock
~~~

## Responsibilities

| Class | Responsibility |
|-------|----------------|
| Product | Holds product data and protects its own rules (no negative price or stock) |
| Cart | Holds the customer's selected items and checks them against current stock |
| Catalog | Owns all products; search, lookup, save and load |
| Order / OrderItem | Immutable record of a purchase (name and price at purchase time) |
| OrderService | Checkout logic: validate everything, then reduce stock, create the order, clear the cart |

## Data flow: checkout

1. Check that the cart is not empty.
2. Re-check every item: product active and quantity not above current stock.
3. If any check fails, return failure and change nothing.
4. Create the order snapshot and reduce stock.
5. Save the order, clear the cart, return the order id.

## Key decisions

- **Cart stores pointers, Catalog owns the products** so stock is always checked against the real, current value.
- **Catalog uses `unique_ptr`** so product addresses stay valid when the list grows.
- **Validate first, change later** in checkout, so a failure never leaves half-updated data.
- **Order snapshot** so later price changes never rewrite history.
