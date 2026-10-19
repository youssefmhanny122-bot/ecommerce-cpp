# Requirements

## Functional requirements

| ID | Requirement | Acceptance criteria |
|----|-------------|---------------------|
| F1 | Customer can browse active products | Only active products are listed with id, name, price, stock |
| F2 | Customer can search products by name | Case-insensitive partial match; deactivated products are excluded |
| F3 | Customer can add a product to the cart | Quantity must be positive; total quantity in cart must not exceed stock; inactive products are rejected |
| F4 | Customer can update or remove cart items | Unknown ids and invalid quantities are rejected; stock limit still applies |
| F5 | System calculates the cart total | Total equals the sum of price x quantity for all items |
| F6 | Customer can check out | Empty cart is rejected; availability is re-validated; stock is reduced only if the whole checkout succeeds; cart is emptied after success |
| F7 | Orders keep a snapshot | Product name and unit price are stored at purchase time; later price changes do not affect old orders |
| F8 | Customer can view orders | Order id, status, items and total are shown |
| F9 | Admin can add products | Name must not be empty; price and stock must not be negative |
| F10 | Admin can update price and add stock | Price must not be negative; added quantity must be positive |
| F11 | Admin can deactivate a product | Deactivated product disappears from browsing and search |
| F12 | Admin can view all orders | All created orders are listed |
| F13 | Products are saved to and loaded from a file | Data survives restarts; corrupted lines are skipped without crashing |

## Non-functional requirements

- Console application written in C++17.
- Invalid user input (letters instead of numbers, unknown ids) never crashes the program.
- Cart, order and inventory data stay consistent: a failed checkout changes nothing.
- The project builds with `./build.sh` and its tests run with `./run_tests.sh`.

## Out of scope

Graphical interface, real payments, real authentication, network services, and a real database.
