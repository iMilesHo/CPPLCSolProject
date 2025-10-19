# Count Elements Greater Than Previous Average

https://www.hackerrank.com/contests/software-engineer-prep-kit/challenges/count-elements-greater-than-previous-average/problem

## My solution

### Wrong Answer

```c++

int countResponseTimeRegressions(vector<int> responseTimes) {
    int n = responseTimes.size();
    if(n <= 1) {
        return 0;
    }
    int previousSum = responseTimes[0];
    int count = 0;

    for(int i = 1; i < n; ++i){
        if (previousSum < responseTimes[i]*(long long)i) {
            count++;
        }
        previousSum += responseTimes[i];
    }
    return count;
}

```

### Accepted

```C++
#include <bits/stdc++.h>

using namespace std;

/*
 * Complete the 'countResponseTimeRegressions' function below.
 *
 * The function is expected to return an INTEGER.
 * The function accepts INTEGER_ARRAY responseTimes as parameter.
 */
int countResponseTimeRegressions(vector<int> responseTimes) {
    int n = responseTimes.size();
    if(n <= 1) {
        return 0;
    }
    double previousSum = responseTimes[0];
    int count = 0;

    for(int i = 1; i < n; ++i){
        double avrg = previousSum / i;
        if (responseTimes[i] > avrg) {
            count++;
        }
        previousSum += responseTimes[i];
    }
    return count;
}
```

## Notes

`#include <bits/stdc++.h>` is supported in HackerRank's C++ environment and includes most standard libraries, making it convenient for competitive programming.
