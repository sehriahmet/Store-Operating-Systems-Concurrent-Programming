# Concurrent Store Simulation

This repository contains the implementation of **Concurrent Store Simulation** for **CENG 334: Introduction to Operating Systems** (Spring 2024–2025) at METU. The goal is to simulate a store selling three item types (AAA, BBB, CCC) with concurrent customer and supplier threads, enforcing synchronization constraints without deadlock or busy-waiting.


## Assignment Overview

- **Store Details**: Maintains capacity (cap) and availability (avail) for item types AAA, BBB, CCC.
- **Customer Threads**: Issue `buy(countAAA, countBBB, countCCC)`; block if any count > avail.
- **Supplier Threads**: Issue `maysupply(itemType, n)` to reserve capacity, then `supply(itemType, n)` to restock; block if capacity would overflow.
- **Constraints**: Must satisfy 14 synchronization rules (e.g., no busy-waiting, deadlock-free, fairness not required).

Refer to [hw2.pdf](hw2.pdf) for full specification.
