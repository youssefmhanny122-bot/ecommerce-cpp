# E-Commerce System (C++ Console)

A console-based e-commerce simulation: product catalog, search, shopping cart, checkout, orders, and admin inventory management.
Graduation project for the EX Code Computer Science Diploma.

## Features

- Customer: browse, search, add/update/remove cart items, checkout, view orders
- Admin: add products, change price, add stock, deactivate products, view orders
- Validation: no negative prices or stock, no overselling, safe handling of invalid input
- Products and orders are saved to `data/` and loaded on startup

## Project structure

    include/   header files (Product, Cart, Catalog, Order, OrderService)
    src/       implementation files and main.cpp (console menus)
    tests/     automated tests (test_all.cpp)
    docs/      requirements and design notes
    data/      saved products (created at runtime, not tracked by Git)

## Build and run

Requires a C++17 compiler (g++ or clang++).

    ./build.sh
    ./app

## Run the tests

    ./run_tests.sh

## Design decisions

- **Product** validates its own data in the constructor, so an invalid product can never exist.
- **Cart** stores pointers to products owned by the Catalog, so it always checks the current stock.
- **Catalog** owns products through `unique_ptr`, which keeps their addresses stable when the list grows.
- **OrderService::checkout** validates everything first and only then reduces stock, so a failed checkout changes nothing.
- **Order** stores a snapshot (name and unit price) so later price changes do not alter past orders.

## Known limitations

- There is no real authentication: role selection is simulated at startup.
- Product names must not contain the `|` character (it is the file separator).
