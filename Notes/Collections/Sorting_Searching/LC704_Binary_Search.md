## 704. Binary Search

```cpp
int Solution::search(std::vector<int>& nums, int target)
{
    int left = 0;
    int right = static_cast<int>(nums.size()) - 1;
    
    while (left <= right)
    {
        int mid = (right + left) / 2;
        if (nums[mid] == target)
        {
            return mid;
        }
        else if (nums[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return -1;
}
```

## Notes:
- Integer Overflow Safety (when calculating mid):
int mid = left + (right - left) / 2;
- discuss Robustness and Edge Cases
    - works fine for my method
- Higher-Level Alternatives
```cpp
auto it = std::lower_bound(nums.begin(), nums.end(), target);
if (it != nums.end() && *it == target) {
    return static_cast<int>(it - nums.begin());
} else {
    return -1;
}
```
    - This isn’t necessary, but it shows us know the standard library and can choose between manual and library-driven approaches as needed.
- When both right and left are integers, the expression (right - left) / 2 performs integer arithmetic. That means:
	- First, right - left is evaluated as an int, producing another integer result.
	- Then, dividing by 2 also uses integer division. Any fractional part resulting from the division is discarded (truncated toward zero).


- Example of error
```cpp
#include <iostream>
#include <vector>

int main() {
    std::size_t left = 5;  // unsigned
    int right = 3;         // signed
    
    // We intend to compute a midpoint:
    // int mid = left + (right - left) / 2;  <-- intended logic
    
    // But since 'right' is signed and 'left' is unsigned,
    // 'right - left' first converts 'right' to an unsigned type,
    // effectively causing underflow.
    
    auto diff = right - left; 
    // Here, 'right - left' is computed as an unsigned operation since left is size_t (unsigned).
    // right (3) is promoted to an unsigned number, and subtracting a larger unsigned (5)
    // leads to wrap-around, resulting in a very large number instead of a negative one.

    std::cout << "right - left = " << diff << "\n";
    // Instead of getting a negative number like -2, we get a huge unsigned value
    // (on a 64-bit system, typically something close to std::numeric_limits<std::size_t>::max()).

    return 0;
}
```

**What happens here:**

- On a 64-bit platform, `std::size_t` is typically `unsigned long long` (64-bit).
- Converting the signed int `3` to an unsigned 64-bit value still gives `3ULL`.
- Now, `3ULL - 5ULL` doesn’t result in `-2`; instead, since unsigned arithmetic cannot represent negative values, it wraps around:
  - `3ULL - 5ULL` = `(0x0000000000000003ULL) - (0x0000000000000005ULL)`
  - This results in something like `0xFFFFFFFFFFFFFFFEULL` (a very large number).

**Effect on your calculation:**

If you were using this difference to calculate a midpoint, you’d get a nonsensical index (often extremely large), leading to out-of-bounds array accesses or other undefined behavior.

By ensuring both `right` and `left` are signed (e.g., both are `int`), negative differences remain negative, and you can handle them logically (e.g., `right < left` meaning the search range is invalid), rather than causing subtle overflow bugs.


Overall: my logic is sound and my solution is good. Emphasizing safety (overflow), clear code style, and knowledge of complexity and standard library facilities will help us appear more seasoned in interviews for a mid-level SDE role.

##  Topics
- binary search