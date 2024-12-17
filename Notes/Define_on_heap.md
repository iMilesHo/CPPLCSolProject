
- Define a value on heap, we need to manually manage the memory, including deallocating it with `delete[]` to avoid memory leaks.
- the return value of `new ...` is a pointer to the value or the first value of a array

```cpp
int* arr = new int[5];

// Initializing array elements
for (int i = 0; i < 5; i++) {
    arr[i] = i + 1;  // Setting values from 1 to 5
}

// Deallocating the array to free memory
delete[] arr;
```

### Points to Consider
- **Memory Management**: otherwise lead to memory leaks.
  
- **Initialization**: Unlike stack-allocated arrays, heap-allocated arrays do not automatically initialize their elements (unless they are of a class type with a default constructor). This means that the elements of an `int` array will have indeterminate values if not explicitly initialized.
- Both stack-allocated and heap-allocated arrays of primitive types do not automatically initialize their elements to zero or any other specific value.
- Explicit initialization is required to set array elements to a specific value upon declaration, whether the array is allocated on the stack or the heap.


```cpp
int* arr = new int[5]();  // Allocate and initialize all elements to zero

int* arr = new int[5] {1,2,3,4,5}; // or
```

- **Safety**: Consider using modern C++ structures like `std::vector` for dynamic arrays, which handle memory management automatically and provide bounds checking and other useful features.
