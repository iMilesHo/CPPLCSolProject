// 堆排序
#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class HeapSort {
public:
    // 主要的堆排序函数
    static void heapSort(vector<int>& arr) {
        int n = arr.size();
        
        // 构建最大堆 (从最后一个非叶子节点开始)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapify(arr, n, i);
        }
        
        // 一个个从堆中取出元素
        for (int i = n - 1; i > 0; i--) {
            // 将当前根（最大元素）移到末尾
            swap(arr[0], arr[i]);
            
            // 在减小的堆上调用heapify
            heapify(arr, i, 0);
        }
    }
    
    // 堆排序函数 - 使用迭代版本的heapify
    static void heapSortIterative(vector<int>& arr) {
        int n = arr.size();
        
        // 构建最大堆 (从最后一个非叶子节点开始)
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyIterative(arr, n, i);
        }
        
        // 一个个从堆中取出元素
        for (int i = n - 1; i > 0; i--) {
            // 将当前根（最大元素）移到末尾
            swap(arr[0], arr[i]);
            
            // 在减小的堆上调用heapify
            heapifyIterative(arr, i, 0);
        }
    }

private:
    // 调整堆的函数 - 递归版本
    static void heapify(vector<int>& arr, int n, int i) {
        int largest = i;    // 初始化largest为根
        int left = 2 * i + 1;   // 左子节点
        int right = 2 * i + 2;  // 右子节点
        
        // 如果左子节点比根大
        if (left < n && arr[left] > arr[largest]) {
            largest = left;
        }
        
        // 如果右子节点比当前最大的大
        if (right < n && arr[right] > arr[largest]) {
            largest = right;
        }
        
        // 如果largest不是根
        if (largest != i) {
            swap(arr[i], arr[largest]);
            
            // 递归调整受影响的子树
            heapify(arr, n, largest);
        }
    }


    
    // 调整堆的函数 - 迭代版本
    static void heapifyIterative(vector<int>& arr, int n, int i) {
        while (true) {
            int largest = i;        // 初始化largest为根
            int left = 2 * i + 1;   // 左子节点
            int right = 2 * i + 2;  // 右子节点
            
            // 如果左子节点比根大
            if (left < n && arr[left] > arr[largest]) {
                largest = left;
            }
            
            // 如果右子节点比当前最大的大
            if (right < n && arr[right] > arr[largest]) {
                largest = right;
            }
            
            // 如果largest就是根，说明堆性质已满足，跳出循环
            if (largest == i) {
                break;
            }
            
            // 交换并继续向下调整
            swap(arr[i], arr[largest]);
            i = largest;  // 移动到被交换的位置继续调整
        }
    }
};

// Google Test 测试用例
class HeapSortTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 测试前的准备工作
    }
    
    void TearDown() override {
        // 测试后的清理工作
    }
};

// 测试基本功能
TEST_F(HeapSortTest, BasicSorting) {
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    vector<int> expected = {11, 12, 22, 25, 34, 64, 90};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试空数组
TEST_F(HeapSortTest, EmptyArray) {
    vector<int> arr = {};
    vector<int> expected = {};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试单个元素
TEST_F(HeapSortTest, SingleElement) {
    vector<int> arr = {42};
    vector<int> expected = {42};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试已排序数组
TEST_F(HeapSortTest, AlreadySorted) {
    vector<int> arr = {1, 2, 3, 4, 5};
    vector<int> expected = {1, 2, 3, 4, 5};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试反向排序数组
TEST_F(HeapSortTest, ReverseSorted) {
    vector<int> arr = {5, 4, 3, 2, 1};
    vector<int> expected = {1, 2, 3, 4, 5};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试重复元素
TEST_F(HeapSortTest, DuplicateElements) {
    vector<int> arr = {3, 7, 3, 4, 7, 4, 4, 3};
    vector<int> expected = {3, 3, 3, 4, 4, 4, 7, 7};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试负数
TEST_F(HeapSortTest, NegativeNumbers) {
    vector<int> arr = {-5, -1, -8, -3, -9, -2};
    vector<int> expected = {-9, -8, -5, -3, -2, -1};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试混合正负数
TEST_F(HeapSortTest, MixedNumbers) {
    vector<int> arr = {-3, 5, -1, 0, 8, -7, 2};
    vector<int> expected = {-7, -3, -1, 0, 2, 5, 8};
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试大数组性能（可选）
TEST_F(HeapSortTest, LargeArray) {
    vector<int> arr;
    vector<int> expected;
    
    // 生成1000个随机数
    for (int i = 1000; i > 0; i--) {
        arr.push_back(i);
        expected.push_back(i);
    }
    
    sort(expected.begin(), expected.end()); // 使用标准库排序作为期望结果
    
    HeapSort::heapSort(arr);
    
    EXPECT_EQ(arr, expected);
}

// =================== 迭代版本 heapify 的测试 ===================

// 测试迭代版本基本功能
TEST_F(HeapSortTest, IterativeBasicSorting) {
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    vector<int> expected = {11, 12, 22, 25, 34, 64, 90};
    
    HeapSort::heapSortIterative(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试迭代版本空数组
TEST_F(HeapSortTest, IterativeEmptyArray) {
    vector<int> arr = {};
    vector<int> expected = {};
    
    HeapSort::heapSortIterative(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试迭代版本单个元素
TEST_F(HeapSortTest, IterativeSingleElement) {
    vector<int> arr = {42};
    vector<int> expected = {42};
    
    HeapSort::heapSortIterative(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试迭代版本重复元素
TEST_F(HeapSortTest, IterativeDuplicateElements) {
    vector<int> arr = {3, 7, 3, 4, 7, 4, 4, 3};
    vector<int> expected = {3, 3, 3, 4, 4, 4, 7, 7};
    
    HeapSort::heapSortIterative(arr);
    
    EXPECT_EQ(arr, expected);
}

// 测试迭代版本混合正负数
TEST_F(HeapSortTest, IterativeMixedNumbers) {
    vector<int> arr = {-3, 5, -1, 0, 8, -7, 2};
    vector<int> expected = {-7, -3, -1, 0, 2, 5, 8};
    
    HeapSort::heapSortIterative(arr);
    
    EXPECT_EQ(arr, expected);
}

// 比较递归版本和迭代版本结果是否一致
TEST_F(HeapSortTest, RecursiveVsIterativeConsistency) {
    vector<int> arr1 = {64, 34, 25, 12, 22, 11, 90, 88, 76, 50, 43};
    vector<int> arr2 = arr1;  // 复制相同的数组
    
    HeapSort::heapSort(arr1);         // 递归版本
    HeapSort::heapSortIterative(arr2); // 迭代版本
    
    EXPECT_EQ(arr1, arr2);  // 两个版本的结果应该相同
}

// 演示函数
void demonstrateHeapSort() {
    cout << "\n=== 堆排序演示 ===" << endl;
    
    vector<int> arr = {64, 34, 25, 12, 22, 11, 90};
    cout << "原数组: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
    
    vector<int> arr1 = arr, arr2 = arr;
    
    // 递归版本
    HeapSort::heapSort(arr1);
    cout << "递归版本排序结果: ";
    for (int num : arr1) {
        cout << num << " ";
    }
    cout << endl;
    
    // 迭代版本
    HeapSort::heapSortIterative(arr2);
    cout << "迭代版本排序结果: ";
    for (int num : arr2) {
        cout << num << " ";
    }
    cout << endl;
    
    // 验证两个版本结果一致
    cout << "两个版本结果" << (arr1 == arr2 ? "一致" : "不一致") << endl;
}

int main(int argc, char **argv) {
    // 运行演示
    demonstrateHeapSort();
    
    // 运行Google Test
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}