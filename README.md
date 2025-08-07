# DEP and ASLR Exploration

## Overview

This project demonstrates two key memory protection mechanisms used in modern operating systems:
**DEP (Data Execution Prevention)** and **ASLR (Address Space Layout Randomization)**.

The program includes:

* A demonstration of **ASLR** by printing the addresses of stack, heap, static, and function pointers.
* A simulation of **DEP** by attempting to execute dynamically generated code from the heap—an operation that should fail on systems with DEP enabled.
* Additional memory layout visualization to aid understanding of typical memory segment separation (stack, heap, code, static).

> ⚠️ **Warning**: The DEP demo may crash the program due to protection mechanisms. Only run the DEP section in controlled or sandboxed environments.

---

## Build & Run
```bash
git clone https://github.com/AniDashyan/dep_and_aslr
cd dep_and_aslr
cmake -S . -B build
cmake --build build
```

#### Quick Run Options
* Run main binary:

  ```bash
  ./build/main
  ```

* Run protected build:

  ```bash
  ./build/main_protected
  ```

* Automatically test ASLR (run twice with different DEP choices):

  ```bash
  cmake --build build --target test_aslr
  ```

* Automatically run:

  ```bash
  cmake --build build --target run
  ```

---

## Example Output

### ASLR (Address Randomization)

Running with `cmake --build build --target test_aslr`:
```
Security Defense Exploration Program
====================================

Run this program multiple times to observe ASLR effects
Addresses should change between runs on ASLR-enabled systems

=== ASLR Demo ===
Stack variable address: 0xe4631ffc7c
Function address: 0x7ff7f04b1460
Heap allocation address: 0x297aa49b6b0

=== Memory Layout Information ===
Static variable (data segment): 0x7ff7f04b4000
Stack variable: 0xe4631ffc7c
Heap variable: 0x297aa4ad2e0
Code segment (this function): 0x7ff7f04b18dd

==================================================
WARNING: attempts to execute
code from heap memory. On DEP-enabled systems,
this will cause a segmentation fault/access violation.
DEP works correctly

Proceed with DEP demonstration? (y/n): Skipping DEP demo

Program completed successfully!
Security Defense Exploration Program
====================================

Run this program multiple times to observe ASLR effects
Addresses should change between runs on ASLR-enabled systems

=== ASLR Demo ===
Stack variable address: 0x25e73ff75c
Function address: 0x7ff7f04b1460
Heap allocation address: 0x138d9fded60

=== Memory Layout Information ===
Static variable (data segment): 0x7ff7f04b4000
Stack variable: 0x25e73ff75c
Heap variable: 0x138d9fe3b00
Code segment (this function): 0x7ff7f04b18dd

==================================================
WARNING: attempts to execute
code from heap memory. On DEP-enabled systems,
this will cause a segmentation fault/access violation.
DEP works correctly

Proceed with DEP demonstration? (y/n): Skipping DEP demo

Program completed successfully!
```

>  If addresses are different ASLR is active.

---

### DEP (Heap Execution)

**With DEP Enabled:**

```
Security Defense Exploration Program
====================================

Run this program multiple times to observe ASLR effects
Addresses should change between runs on ASLR-enabled systems

=== ASLR Demo ===
Stack variable address: 0xd8c07ffd8c
Function address: 0x7ff7f04b1460
Heap allocation address: 0x2b4d53e4d00

=== Memory Layout Information ===
Static variable (data segment): 0x7ff7f04b4000
Stack variable: 0xd8c07ffd8c
Heap variable: 0x2b4d53d97f0
Code segment (this function): 0x7ff7f04b18dd

==================================================
WARNING: attempts to execute
code from heap memory. On DEP-enabled systems,
this will cause a segmentation fault/access violation.
DEP works correctly

Proceed with DEP demonstration? (y/n): y

=== DEP Demo ===
Allocated heap buffer at: 0x0x2b4d53e1720
Copied simple return code to heap buffer
Code bytes: 0x48 0x31 0xc0 0xc3

Attempting to execute code from heap...
NOTE: This will likely fail on systems with DEP enabled!
Calling heap function...
Segmentation fault (core dumped)
```

**With DEP Disabled (in a sandbox VM):**

```
Calling heap function...
Heap execution succeeded! (DEP might be disabled)
```

> ⚠️ Do **not** disable DEP on production machines. Test in VMs or sandboxes only.

---

## How Does It Work?

1. **ASLR Demo (`demo_aslr`)**

   * Allocates a heap buffer.
   * Declares a stack variable.
   * Prints the addresses of code, stack, heap, and static memory.
   * On ASLR-enabled systems, these addresses will shift between runs.

2. **DEP Demo (`demo_dep`)**

   * Allocates executable-style code in heap memory (`xor rax, rax; ret`).
   * Copies the machine instructions to heap.
   * Tries to execute this buffer using a function pointer.
   * If DEP is enabled, execution fails—illustrating the prevention of code execution from data segments.

3. **Memory Info (`show_memory_info`)**

   * Displays the addresses of various memory segments.
   * Helps visualize memory layout and security segmentation.

4. **Platform Variants (CMake)**

   * Adds variants to test with/without stack protection, execstack flag, or DEP/ASLR flags on Windows.
