
# وثيقة مخططات UML ومراجعة التصميم (UML & Design Review) - Milestone 2
مشروع التجارة الإلكترونية بلغة C++

---

## 1. مخطط الكلاسات (Class Diagram - Mermaid Format)

يفتح هذا المخطط العلاقات البنيوية بين الكلاسات، والتوريث، والتركيب داخل المشروع:

```mermaid
classDiagram
    class User {
        #int userId
        #string username
        #string password
        #string role
        +getUserId() int
        +getUsername() string
        +virtual displayMenu()*
    }

    class Customer {
        -ShoppingCart cart
        +displayMenu()
        +getCart() ShoppingCart&
    }

    class Admin {
        +displayMenu()
        +addNewProduct()
        +updateProduct()
    }

    class Product {
        -int productId
        -string name
        -string description
        -double price
        -int stockQuantity
        -int categoryId
        +getPrice() double
        +setPrice(double p)
        +getStockQuantity() int
        +setStockQuantity(int q)
    }

    class Category {
        -int categoryId
        -string categoryName
    }

    class OrderItem {
        -int productId
        -string productNameSnapshot
        -double unitPriceSnapshot
        -int quantity
        +getTotalPrice() double
    }

    class Order {
        -int orderId
        -string customerName
        -string orderDate
        -double totalAmount
        -vector~OrderItem~ items
        +calculateTotal() double
    }

    class ShoppingCart {
        -vector~OrderItem~ cartItems
        +addItem(Product p, int qty)
        +removeItem(int productId)
        +clearCart()
        +checkout() Order
    }

    User <|-- Customer : Inheritance
    User <|-- Admin : Inheritance
    Category "1" -- "many" Product : Contains
    ShoppingCart "1" *-- "many" OrderItem : Composition
    Order "1" *-- "many" OrderItem : Composition
