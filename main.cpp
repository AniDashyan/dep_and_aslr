#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>

// address randomization demo
void sample_function() {
    std::cout << "Sample function executed successfully!" << std::endl;
}

void demo_aslr() {    
    int stack_var = 42;
    std::cout << "Stack variable address: " << std::hex << &stack_var << std::endl;
    
    std::cout << "Function address: " << std::hex << (void*)sample_function << std::endl;
    
    void* heap_ptr = malloc(100);
    std::cout << "Heap allocation address: " << std::hex << heap_ptr << std::endl;
    
    if (heap_ptr) {
        free(heap_ptr);
    }
    
    std::cout << std::dec; // Reset to decimal output
}

// DEP by attempting heap execution demo
void demo_dep() {
    std::cout << "\n=== DEP Demo ===" << std::endl;
    
    // Allocate memory on heap
    const size_t code_size = 64;
    unsigned char* heap_buffer = (unsigned char*)malloc(code_size);
    
    if (!heap_buffer) {
        std::cout << "Memory allocation failed!" << std::endl;
        return;
    }
    
    std::cout << "Allocated heap buffer at: 0x" << std::hex << (void*)heap_buffer << std::dec << std::endl;
    
    // Simple machine code that returns (x86-64: ret instruction = 0xC3)
    unsigned char simple_code[] = {
        0x48, 0x31, 0xC0,  // xor rax, rax (clear return value)
        0xC3               // ret (return)
    };
    
    // Copy code to heap
    memcpy(heap_buffer, simple_code, sizeof(simple_code));
    
    std::cout << "Copied simple return code to heap buffer" << std::endl;
    std::cout << "Code bytes: ";
    for (size_t i = 0; i < sizeof(simple_code); i++) {
        std::cout << "0x" << std::hex << (int)simple_code[i] << " ";
    }
    std::cout << std::dec << std::endl;
    
    // Create function pointer to heap memory
    typedef void (*FuncPtr)();
    FuncPtr heap_func = (FuncPtr)heap_buffer;
    
    std::cout << "\nAttempting to execute code from heap..." << std::endl;
    std::cout << "NOTE: This will likely fail on systems with DEP enabled!" << std::endl;
    
    // WARNING: This will crash on DEP-enabled systems
    // The crash demos DEP working correctly
    try {
        std::cout << "Calling heap function..." << std::endl;
        heap_func(); // This line will likely cause a segmentation fault with DEP
        std::cout << "Heap execution succeeded! (DEP might be disabled)" << std::endl;
    } catch (...) {
        std::cout << "Exception caught during heap execution" << std::endl;
    }
    
    free(heap_buffer);
}

// Function to show memory protection information
void show_memory_info() {
    std::cout << "\n=== Memory Layout Information ===" << std::endl;
    
    // Different memory regions
    static int static_var = 123;
    int stack_var = 456;
    void* heap_var = malloc(sizeof(int));
    
    std::cout << "Static variable (data segment): " << std::hex << &static_var << std::endl;
    std::cout << "Stack variable: " << std::hex << &stack_var << std::endl;
    std::cout << "Heap variable: " << std::hex << heap_var << std::endl;
    std::cout << "Code segment (this function): " << std::hex << (void*)show_memory_info << std::endl;
    
    if (heap_var) {
        free(heap_var);
    }
    
    std::cout << std::dec; // Reset to decimal
}

int main() {
    std::cout << "Security Defense Exploration Program" << std::endl;
    std::cout << "====================================" << std::endl;
    
    std::cout << "\nRun this program multiple times to observe ASLR effects" << std::endl;
    std::cout << "Addresses should change between runs on ASLR-enabled systems" << std::endl;
    
    std::cout << "\n=== ASLR Demo ===" << std::endl;
    demo_aslr();
    
    // Show memory layout
    show_memory_info();
    
    // demo DEP (this part might crash the program)
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "WARNING: attempts to execute" << std::endl;
    std::cout << "code from heap memory. On DEP-enabled systems," << std::endl;
    std::cout << "this will cause a segmentation fault/access violation." << std::endl;
    std::cout << "DEP works correctly" << std::endl;
    
    char choice;
    std::cout << "\nProceed with DEP demonstration? (y/n): ";
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        demo_dep();
    } else {
        std::cout << "Skipping DEP demo" << std::endl;
    }
    
    std::cout << "\nProgram completed successfully!" << std::endl;
    return 0;
}