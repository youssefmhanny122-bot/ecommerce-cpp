# Final Project Report: E-Commerce Console System (C++17)

## 1. Project Overview
The **E-Commerce Console System** is a robust command-line application built using **C++17**. The system is designed to simulate a complete e-commerce workflow, providing distinct roles for Customers and Administrators, supported by persistent file storage.

## 2. Core Features & Architecture
- **OOP Architecture & Pillars:** Designed using core Object-Oriented Programming principles including Encapsulation (data hiding and validation), Inheritance, and Polymorphism.
- **Admin Capabilities:** Manage the product catalog, update stock levels, and add new inventory items.
- **Customer Capabilities:** Browse products, search items, manage a shopping cart (add, update, remove items), execute checkout, and view past order history.
- **Data Persistence:** Automatically saves product inventory and order transactions to local files (`products.txt`, `orders.txt`) to maintain state between sessions.

## 3. Testing & Quality Assurance
- Comprehensive unit testing was implemented to validate system behavior under both success and failure conditions.
- **Test Summary:** Total of **81 automated tests** executed successfully with **0 failures** (`Passed: 81, Failed: 0`).
- Core validations include preventing negative pricing, handling stock shortages during checkout, and ensuring cart data integrity.

## 4. How to Build and Run
1. Give execution permissions to the scripts:
   ```bash
   chmod +x build.sh run_tests.sh
