# C++ Random Operations Complete Cheat Sheet

## Table of Contents

1. [Legacy C-Style Random Functions](#legacy-c-style-random-functions)
2. [Modern C++11+ Random Library](#modern-c11-random-library)
3. [Random Number Generators](#random-number-generators)
4. [Distributions](#distributions)
5. [Random Operations on Containers](#random-operations-on-containers)
6. [Performance Comparison](#performance-comparison)
7. [Best Practices](#best-practices)
8. [Common Use Cases](#common-use-cases)

---

## Legacy C-Style Random Functions

### 1. `rand()` and `srand()`

```cpp
#include <cstdlib>
#include <ctime>

// Seeding
srand(time(0));  // Seed with current time

// Generate random numbers
int random_int = rand();                    // 0 to RAND_MAX
int range_int = rand() % 100;              // 0 to 99
double random_double = (double)rand() / RAND_MAX;  // 0.0 to 1.0
```

**Pros:**

- Simple to use
- Available everywhere
- No includes needed for basic usage

**Cons:**

- Poor quality randomness
- Not thread-safe
- Limited to 32767 (RAND_MAX) on many systems
- Modulo bias when using `%` for ranges
- Global state makes testing difficult

---

## Modern C++11+ Random Library

### Basic Setup

```cpp
#include <random>

// Method 1: Simple setup
std::random_device rd;
std::mt19937 gen(rd());

// Method 2: Seeded for reproducibility
std::mt19937 gen(12345);

// Method 3: Global generator (for convenience)
std::mt19937& getGenerator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}
```

---

## Random Number Generators

### 1. `std::random_device`

```cpp
std::random_device rd;
unsigned int random_num = rd();
```

**Pros:**

- True hardware randomness (when available)
- Non-deterministic
- Good for seeding other generators

**Cons:**

- Slow (not for bulk generation)
- May fall back to pseudo-random on some systems
- Limited availability

### 2. `std::mt19937` (Mersenne Twister)

```cpp
std::mt19937 gen(seed);  // 32-bit
std::mt19937_64 gen64(seed);  // 64-bit
```

**Pros:**

- High quality randomness
- Fast generation
- Long period (2^19937 - 1)
- Industry standard

**Cons:**

- Large state (2.5KB)
- Predictable if you know the state
- Not cryptographically secure

### 3. `std::linear_congruential_engine`

```cpp
std::minstd_rand gen(seed);  // Minimal standard
```

**Pros:**

- Very fast
- Small memory footprint
- Simple implementation

**Cons:**

- Lower quality randomness
- Shorter period
- Predictable patterns

### 4. `std::ranlux24` / `std::ranlux48`

```cpp
std::ranlux24 gen(seed);
std::ranlux48 gen(seed);
```

**Pros:**

- High quality randomness
- Designed for scientific computing

**Cons:**

- Slower than Mersenne Twister
- More complex

---

## Distributions

### 1. Uniform Integer Distribution

```cpp
std::uniform_int_distribution<int> dist(1, 6);  // Dice roll
int dice = dist(gen);

// One-liner
int random_range = std::uniform_int_distribution<int>(min, max)(gen);
```

### 2. Uniform Real Distribution

```cpp
std::uniform_real_distribution<double> dist(0.0, 1.0);
double random_real = dist(gen);

// Float version
std::uniform_real_distribution<float> float_dist(0.0f, 1.0f);
```

### 3. Normal (Gaussian) Distribution

```cpp
std::normal_distribution<double> dist(mean, stddev);
double normal_value = dist(gen);

// Example: IQ scores (mean=100, stddev=15)
std::normal_distribution<double> iq_dist(100.0, 15.0);
```

### 4. Bernoulli Distribution (True/False)

```cpp
std::bernoulli_distribution dist(0.3);  // 30% chance of true
bool coin_flip = dist(gen);
```

### 5. Binomial Distribution

```cpp
std::binomial_distribution<int> dist(trials, probability);
int successes = dist(gen);

// Example: 10 coin flips
std::binomial_distribution<int> coin_flips(10, 0.5);
```

### 6. Poisson Distribution

```cpp
std::poisson_distribution<int> dist(lambda);
int events = dist(gen);

// Example: Average 3 events per hour
std::poisson_distribution<int> hourly_events(3.0);
```

### 7. Exponential Distribution

```cpp
std::exponential_distribution<double> dist(lambda);
double time_between = dist(gen);
```

---

## Random Operations on Containers

### 1. Shuffle Container

```cpp
#include <algorithm>
#include <vector>

std::vector<int> vec = {1, 2, 3, 4, 5};
std::mt19937 gen(std::random_device{}());

// Modern way
std::shuffle(vec.begin(), vec.end(), gen);

// Legacy way (DON'T USE)
std::random_shuffle(vec.begin(), vec.end());  // Deprecated in C++14
```

### 2. Random Element Selection

```cpp
#include <iterator>

template<typename Container>
auto randomElement(Container& container) {
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, container.size() - 1);
    auto it = container.begin();
    std::advance(it, dist(gen));
    return it;
}

// Usage
std::vector<int> vec = {10, 20, 30, 40, 50};
auto random_it = randomElement(vec);
int random_value = *random_it;
```

### 3. Random Sample

```cpp
#include <algorithm>

// Sample k elements from container
template<typename InputIt, typename OutputIt>
void randomSample(InputIt first, InputIt last, OutputIt out, size_t k) {
    std::mt19937 gen(std::random_device{}());
    std::sample(first, last, out, k, gen);
}

// Usage
std::vector<int> source = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::vector<int> sample(3);
randomSample(source.begin(), source.end(), sample.begin(), 3);
```

### 4. Random Partition

```cpp
#include <algorithm>

std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
std::mt19937 gen(std::random_device{}());

// Randomly partition around a pivot
auto predicate = [&gen](int) {
    std::bernoulli_distribution dist(0.5);
    return dist(gen);
};

auto partition_point = std::partition(vec.begin(), vec.end(), predicate);
```

---

## Performance Comparison

```cpp
#include <chrono>
#include <iostream>

void benchmarkGenerators() {
    const int iterations = 1000000;

    // rand() benchmark
    auto start = std::chrono::high_resolution_clock::now();
    srand(time(0));
    for(int i = 0; i < iterations; ++i) {
        volatile int x = rand();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto rand_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // mt19937 benchmark
    start = std::chrono::high_resolution_clock::now();
    std::mt19937 gen(12345);
    for(int i = 0; i < iterations; ++i) {
        volatile unsigned int x = gen();
    }
    end = std::chrono::high_resolution_clock::now();
    auto mt_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "rand(): " << rand_time.count() << "ms\n";
    std::cout << "mt19937: " << mt_time.count() << "ms\n";
}
```

**Typical Results:**

- `rand()`: Fast but poor quality
- `std::mt19937`: Slightly slower but much better quality
- `std::random_device`: Very slow, use only for seeding

---

## Best Practices

### 1. Thread Safety

```cpp
// Thread-local generators
thread_local std::mt19937 gen(std::random_device{}());

// Or use atomic operations for shared state
class ThreadSafeRandom {
    std::mutex mtx;
    std::mt19937 gen;
public:
    ThreadSafeRandom() : gen(std::random_device{}()) {}

    int get(int min, int max) {
        std::lock_guard<std::mutex> lock(mtx);
        std::uniform_int_distribution<int> dist(min, max);
        return dist(gen);
    }
};
```

### 2. Seeding Strategies

```cpp
// Good: Non-deterministic seeding
std::mt19937 gen(std::random_device{}());

// Good: Deterministic seeding for testing
std::mt19937 gen(12345);

// Better: Multiple entropy sources
std::mt19937 gen(std::random_device{}() ^
                 std::chrono::high_resolution_clock::now().time_since_epoch().count());

// Best: Proper seeding with seed_seq
std::random_device rd;
std::seed_seq seed{rd(), rd(), rd(), rd()};
std::mt19937 gen(seed);
```

### 3. Distribution Reuse

```cpp
// Good: Reuse distributions
std::mt19937 gen(std::random_device{}());
std::uniform_int_distribution<int> dist(1, 100);

for(int i = 0; i < 1000; ++i) {
    int value = dist(gen);  // Efficient
}

// Bad: Create new distribution each time
for(int i = 0; i < 1000; ++i) {
    int value = std::uniform_int_distribution<int>(1, 100)(gen);  // Inefficient
}
```

### 4. Avoiding Modulo Bias

```cpp
// Wrong: Modulo bias
int biased = rand() % 6;  // Not uniform if RAND_MAX % 6 != 5

// Correct: Use proper distribution
std::uniform_int_distribution<int> dist(0, 5);
int unbiased = dist(gen);
```

---

## Common Use Cases

### 1. Monte Carlo Simulation

```cpp
class MonteCarloPI {
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;

public:
    MonteCarloPI() : gen(std::random_device{}()), dist(-1.0, 1.0) {}

    double estimatePI(int samples) {
        int inside_circle = 0;
        for(int i = 0; i < samples; ++i) {
            double x = dist(gen);
            double y = dist(gen);
            if(x*x + y*y <= 1.0) {
                inside_circle++;
            }
        }
        return 4.0 * inside_circle / samples;
    }
};
```

### 2. Random String Generation

```cpp
std::string randomString(size_t length) {
    const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, chars.size() - 1);

    std::string result;
    result.reserve(length);
    for(size_t i = 0; i < length; ++i) {
        result += chars[dist(gen)];
    }
    return result;
}
```

### 3. Weighted Random Selection

```cpp
template<typename T>
class WeightedRandom {
    std::vector<T> items;
    std::vector<double> weights;
    std::mt19937 gen;

public:
    WeightedRandom() : gen(std::random_device{}()) {}

    void addItem(const T& item, double weight) {
        items.push_back(item);
        weights.push_back(weight);
    }

    T select() {
        std::discrete_distribution<size_t> dist(weights.begin(), weights.end());
        return items[dist(gen)];
    }
};

// Usage
WeightedRandom<std::string> wr;
wr.addItem("Common", 70);
wr.addItem("Rare", 25);
wr.addItem("Epic", 5);
std::string result = wr.select();
```

### 4. Random Graph Generation

```cpp
class RandomGraph {
    std::mt19937 gen;

public:
    RandomGraph() : gen(std::random_device{}()) {}

    std::vector<std::vector<int>> generateErdosRenyi(int n, double p) {
        std::vector<std::vector<int>> adj(n);
        std::bernoulli_distribution dist(p);

        for(int i = 0; i < n; ++i) {
            for(int j = i + 1; j < n; ++j) {
                if(dist(gen)) {
                    adj[i].push_back(j);
                    adj[j].push_back(i);
                }
            }
        }
        return adj;
    }
};
```

---

## Summary Recommendations

### For New Code (C++11+):

1. **Use `std::mt19937`** for general-purpose random number generation
2. **Use appropriate distributions** (`std::uniform_int_distribution`, etc.)
3. **Seed with `std::random_device`** for non-deterministic behavior
4. **Use `std::shuffle`** instead of `std::random_shuffle`
5. **Make generators thread-local** for multithreaded applications

### For Legacy Code:

1. **Avoid `rand()`** if possible
2. **If you must use `rand()`**, seed with `srand(time(0))`
3. **Be aware of modulo bias** when creating ranges

### For Performance-Critical Code:

1. **Reuse generators and distributions**
2. **Consider `std::minstd_rand`** for speed over quality
3. **Use thread-local storage** to avoid synchronization

### For Cryptographic Applications:

1. **Don't use standard generators** - they're not cryptographically secure
2. **Use OS-provided secure random functions** instead
3. **Consider libraries like OpenSSL** for cryptographic randomness
