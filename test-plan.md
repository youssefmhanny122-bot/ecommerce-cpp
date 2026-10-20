# Test Plan

## Test environment
- Operating system: Ubuntu 24 (Linux). **Re-run on your own machine and update this line and the results if they differ.**
- Compiler: g++ with `-std=c++17 -Wall -Wextra`
- Build instructions: `./build.sh` (app) and `./run_tests.sh` (tests)
- Data setup: automated tests use their own temporary files; manual tests start from an empty `data/` folder (the program then seeds Laptop, Gaming Mouse, Keyboard)

## How testing is done
- **Automated (TC-01 to TC-22):** `tests/test_all.cpp`, one `CHECK` per assertion. Result of the last run: **Passed: 81, Failed: 0**.
- **Manual (TC-M1 to TC-M8):** scripted console input piped into `./app`. These cover the menus in `main.cpp`, which have no automated tests.

## Automated test cases

| Test ID | Requirement | Preconditions | Input / Action | Expected result | Actual result | Status |
|---|---|---|---|---|---|---|
| TC-01 | FR-08 | none | Create `Product` with negative price, negative stock, empty name | `invalid_argument` thrown each time | as expected | Pass |
| TC-02 | NFR-02 | Product stock 5 | `removeStock` 3; 10 (too many); 0; -1; 2 (all remaining) | Succeeds for 3 and 2; rejects 10, 0, -1; stock unchanged on rejection; ends at 0 | as expected | Pass |
| TC-03 | FR-09, FR-10 | Product exists | `addStock(0)`; `setPrice(-5)` | `invalid_argument`; price unchanged | as expected | Pass |
| TC-04 | FR-03 | Stock 5 | Add 2, then 4 (total 6), then 3 (total 5), then 1; add 0 and -2; add inactive product | 2 ok; 4 rejected; 3 ok and merged into one line of 5; 1 rejected; 0, -2 and inactive rejected | as expected | Pass |
| TC-05 | FR-05 | Cart with 5 laptops, 4 mice | `getTotal` | 5 x 1000 + 4 x 50 | as expected | Pass |
| TC-06 | FR-04 | Cart with a mouse line | Update unknown id; quantity 0; quantity above stock; valid quantity | First three rejected; last accepted | as expected | Pass |
| TC-07 | FR-04 | Cart with two lines | Remove unknown id; remove each line | Unknown rejected; lines removed; cart ends empty | as expected | Pass |
| TC-08 | FR-08 | Empty catalog | Add valid; add with negative price; add another | Ids 1 and 2 (the failed add does not consume an id) | as expected | Pass |
| TC-09 | FR-02 | Catalog with Gaming Mouse | Search `MOUSE`; `xyz`; `findById(99)` | One result; empty; `nullptr` | as expected | Pass |
| TC-10 | FR-01, FR-02, FR-11 | Mouse deactivated | Search `mouse`; list active products | Search empty; one active product | as expected | Pass |
| TC-11 | FR-13 | 4 products, one inactive, prices like 15000.5 and 1234567.89 | Save then load into a new `Catalog` | Same prices, inactive flag kept, next new id is 5 | as expected | Pass |
| TC-12 | FR-13 | File does not exist | `loadFromFile` | Returns false, no crash | as expected | Pass |
| TC-13 | FR-13 | File with a garbage line and a negative price | Load | Bad lines skipped; 2 good products loaded | as expected | Pass |
| TC-14 | FR-06 | Empty cart | `checkout` | No order | as expected | Pass |
| TC-15 | FR-07 | Laptop 5, mouse 10 | Add 2 laptops and 1 mouse, checkout | Order id 1; stock 3 and 9; cart empty; total 30250 | as expected | Pass |
| TC-16 | FR-07 | Order 1 exists | Change laptop price afterwards | Order total and unit price unchanged | as expected | Pass |
| TC-17 | FR-06 | Cart holds more mice than remaining stock | Checkout | No order; laptop and mouse stock unchanged; cart kept; still 1 order | as expected | Pass |
| TC-18 | FR-07 | Fixed cart from TC-17 | Update quantity, checkout again | Order id 2; stock correct | as expected | Pass |
| TC-19 | FR-13 | Two orders created | Save then load | 2 orders, items and totals match | as expected | Pass |
| TC-20 | FR-07 | Orders loaded | New checkout | Gets id 3, not 1 | as expected | Pass |
| TC-21 | FR-13 | Orders file missing | Load | Returns false | as expected | Pass |
| TC-22 | FR-13 | Order file with garbage, negative price, zero quantity | Load | Only the one valid order is loaded | as expected | Pass |

## Manual test cases (run through `./app`)

| Test ID | Requirement | Preconditions | Input / Action | Expected result | Actual result | Status |
|---|---|---|---|---|---|---|
| TC-M1 | NFR-01 | Start app | Type `abc`, then `9` at the main menu | "Invalid number." then "Unknown option."; menu again | as expected | Pass |
| TC-M2 | FR-03, NFR-01 | Default data | Add product 99; quantity 999 of product 1; quantity -2; quantity `x` | "Product not found."; "Could not add..." twice; "Invalid number." | as expected | Pass |
| TC-M3 | FR-03, FR-04, FR-05 | Default data | Add laptop 2 then 1 more; add mouse 3; view; update mouse to 1; remove laptop | One laptop line x3 (45000.00); total 45750.00; after update 45250.00; after remove 250.00 | as expected | Pass |
| TC-M4 | FR-07 | Default data | Add 4 mice, checkout, exit | Order #1 Confirmed, total 1000.00; mouse stock 6; files written | as expected | Pass |
| TC-M5 | FR-12, FR-13 | After TC-M4 | Restart, view orders and catalog | Order #1 and stock 6 still present | as expected | Pass |
| TC-M6 | FR-06, FR-11 | Laptop in cart | Admin deactivates laptop, customer checks out | "Checkout failed..."; "No orders yet." | as expected | Pass |
| TC-M7 | FR-08, NFR-01 | Start app | Admin adds product with price -5; stock -1; price `abc` | "Price cannot be negative"; "Stock cannot be negative"; "Invalid number." | as expected | Pass |
| TC-M8 | FR-01, FR-13 | `data/` empty | Start app, browse | 3 default products, no crash | as expected | Pass |

## Minimum scenario coverage

- [x] Normal catalog and search flow (TC-09, TC-10, TC-M8)
- [x] Product not found (TC-09, TC-M2)
- [ ] Duplicate product ID: **not testable through the UI** (ids are auto-assigned). Duplicate ids inside a data file are **not checked** (see D-06)
- [x] Invalid price or stock (TC-01, TC-M7)
- [x] Add/update/remove cart item (TC-04, TC-06, TC-07, TC-M3)
- [x] Zero, negative, non-numeric, and over-stock quantity (TC-04, TC-M2)
- [x] Empty cart checkout (TC-14)
- [x] Successful checkout and unique order (TC-15, TC-18, TC-20, TC-M4)
- [x] Failed checkout with no partial stock/order change (TC-17, TC-M6)
- [x] Total calculation consistency (TC-05, TC-15, TC-19)
- [x] Persistence across restart (TC-11, TC-19, TC-M5)
- [x] Missing or malformed data file (TC-12, TC-13, TC-21, TC-22) *(but see D-03 for what happens afterwards)*
- [ ] Regression test after fixing a defect: **add one per defect below after you fix it**

## Not yet tested (honest list)
- Checkout with an **inactive product** in the cart has only a manual test (TC-M6); there is no automated test.
- Empty catalog search/list is not covered by an automated test.
- Duplicate ids and a missing or empty field in a data file are not tested.
- Very large quantities (integer overflow in `addStock` or in the cart) are not tested.

## Defects found

| Defect ID | Steps to reproduce | Root cause | Fix | Regression test | Status |
|---|---|---|---|---|---|
| D-01 | Admin adds a product, then input ends (Ctrl+D) without choosing Exit | `readLine` calls `exit(0)` on end of input; nothing is saved. Verified: no data file was written | Throw an `EndOfInput` exception, catch it in `main`, save, then exit | Piped-input test that expects the file to exist | **Open** |
| D-02 | Admin adds a product named `A\|B`. Verified: file line `4\|A\|B\|10\|5\|1`; after restart the product is gone | `\|` is the field separator and names are not checked | Reject `\|` in the `Product` constructor | `throwsInvalidArgument(Product(1,"A\|B",1,1))` | **Open** |
| D-03 | Put a malformed line in `data/products.txt`, start and exit. Verified: the bad line is deleted from the file | Loading skips bad lines silently and Exit overwrites the file | Count skipped lines, print a warning, copy the file to `.bak` before the first save | Load a file with a bad line and check the warning/backup | **Open** |
| D-04 | Checkout fails for any reason | `checkout` returns only `nullopt`, so the user always sees the same generic message | Return a reason (empty cart, or the name of the unavailable product) | Test that the reason names the product | **Open** |
| D-05 | Admin adds a product with price 0 | Only negative prices are rejected | Decide: reject 0, or document that free products are allowed (decision log) | Test for the chosen rule | **Open (decision needed)** |
| D-06 | File contains two lines with the same product id | `loadFromFile` does not check for duplicates | Skip or report duplicates on load | Load a file with duplicate ids | **Open** |
