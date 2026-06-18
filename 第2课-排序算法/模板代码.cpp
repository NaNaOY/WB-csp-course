/**
 * 排序算法模板合集
 * 
 * 包含：冒泡排序 / 选择排序 / 插入排序 / 计数排序 / 归并排序 / STL sort 演示
 * 所有函数均可独立运行，main 函数提供完整演示
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

// ==================== 1. 冒泡排序 ====================
// 时间复杂度：O(n²)  空间复杂度：O(1)  稳定：是
void bubbleSort(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;           // 优化：检测是否已有序
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {      // 前面的比后面大 → 交换
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;            // 本轮无交换，已有序
    }
}

// ==================== 2. 选择排序 ====================
// 时间复杂度：O(n²)  空间复杂度：O(1)  稳定：否
void selectionSort(vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        int minPos = i;                 // 假设 i 位置是最小的
        for (int j = i + 1; j < n; j++) {
            if (a[j] < a[minPos]) {
                minPos = j;             // 找到更小的，更新位置
            }
        }
        if (minPos != i) {
            swap(a[i], a[minPos]);      // 把最小值换到前面
        }
    }
}

// ==================== 3. 插入排序 ====================
// 时间复杂度：O(n²)（基本有序时接近 O(n)）  空间复杂度：O(1)  稳定：是
void insertionSort(vector<int>& a) {
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int key = a[i];                 // 当前要插入的"牌"
        int j = i - 1;
        // 从后往前找位置，同时把大的往后挪
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];            // 往后挪一位（覆盖，不交换！）
            j--;
        }
        a[j + 1] = key;                 // 放到正确位置
    }
}

// ==================== 4. 计数排序 ====================
// 时间复杂度：O(n + maxVal)  空间复杂度：O(maxVal)  稳定：是
// 注意：仅适用于非负整数且数据范围不大的情况！
void countingSort(vector<int>& a, int maxVal) {
    int n = a.size();
    vector<int> cnt(maxVal + 1, 0);     // 计数数组，cnt[x] = x出现的次数
    
    // 第1步：统计
    for (int i = 0; i < n; i++) {
        cnt[a[i]]++;
    }
    
    // 第2步：按顺序填回
    int idx = 0;
    for (int val = 0; val <= maxVal; val++) {
        while (cnt[val] > 0) {
            a[idx++] = val;
            cnt[val]--;
        }
    }
}

// ==================== 5. 归并排序 ====================
// 时间复杂度：O(n log n)  空间复杂度：O(n)  稳定：是

// 合并两个有序子数组 [l, mid] 和 [mid+1, r]
void merge(vector<int>& a, int l, int mid, int r) {
    vector<int> tmp(r - l + 1);         // 临时数组
    int i = l, j = mid + 1, k = 0;
    
    // 两路归并：每次取较小的
    while (i <= mid && j <= r) {
        if (a[i] <= a[j])               // 取等号：保证稳定性！
            tmp[k++] = a[i++];
        else
            tmp[k++] = a[j++];
    }
    
    // 把剩余的直接抄过去
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r)   tmp[k++] = a[j++];
    
    // 拷回原数组
    for (int t = 0; t < k; t++) {
        a[l + t] = tmp[t];
    }
}

void mergeSort(vector<int>& a, int l, int r) {
    if (l >= r) return;                 // 终止条件：只剩一个元素
    
    int mid = l + (r - l) / 2;          // 防溢出写法
    mergeSort(a, l, mid);               // 递归排序左半
    mergeSort(a, mid + 1, r);           // 递归排序右半
    merge(a, l, mid, r);                // 合并
}

// ==================== 6. 归并排序求逆序对 ====================
// 在 merge 过程中统计逆序对数量
ll mergeAndCount(vector<int>& a, int l, int mid, int r) {
    vector<int> tmp(r - l + 1);
    int i = l, j = mid + 1, k = 0;
    ll inv = 0;
    
    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            tmp[k++] = a[j++];
            inv += (mid - i + 1);       // a[i..mid] 都比 a[j] 大
        }
    }
    
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r)   tmp[k++] = a[j++];
    
    for (int t = 0; t < k; t++) {
        a[l + t] = tmp[t];
    }
    return inv;
}

ll countInversions(vector<int>& a, int l, int r) {
    if (l >= r) return 0;
    int mid = l + (r - l) / 2;
    ll leftInv  = countInversions(a, l, mid);       // 左半的逆序对
    ll rightInv = countInversions(a, mid + 1, r);   // 右半的逆序对
    ll crossInv = mergeAndCount(a, l, mid, r);       // 跨左右的逆序对
    return leftInv + rightInv + crossInv;
}

// ==================== 演示 main 函数 ====================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    // ---- 演示数据 ----
    vector<int> original = {5, 2, 8, 1, 9, 3, 7, 4, 6, 5};
    
    auto printArr = [](const vector<int>& a, const string& label) {
        cout << label << ": ";
        for (int x : a) cout << x << " ";
        cout << endl;
    };
    
    printArr(original, "原始数组");
    cout << string(50, '-') << endl;
    
    // 1. 冒泡排序
    {
        vector<int> a = original;
        bubbleSort(a);
        printArr(a, "冒泡排序");
    }
    
    // 2. 选择排序
    {
        vector<int> a = original;
        selectionSort(a);
        printArr(a, "选择排序");
    }
    
    // 3. 插入排序
    {
        vector<int> a = original;
        insertionSort(a);
        printArr(a, "插入排序");
    }
    
    // 4. 计数排序（仅演示，数据范围 0~9）
    {
        vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
        countingSort(a, 9);
        printArr(a, "计数排序");
    }
    
    // 5. 归并排序
    {
        vector<int> a = original;
        mergeSort(a, 0, a.size() - 1);
        printArr(a, "归并排序");
    }
    
    // 6. 逆序对统计
    {
        vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6}; // 原始乱序
        vector<int> b = a;  // countInversions 会修改数组
        ll inv = countInversions(b, 0, b.size() - 1);
        printArr(a, "逆序对-原数组");
        cout << "逆序对数量: " << inv << endl;
    }
    
    cout << string(50, '-') << endl;
    
    // 7. STL sort 演示：自定义排序规则
    {
        cout << "\n=== STL sort 自定义排序演示 ===" << endl;
        
        // 7.1 降序排列
        vector<int> a = original;
        sort(a.begin(), a.end(), greater<int>());
        printArr(a, "降序排列");
        
        // 7.2 按绝对值排序
        vector<int> b = {-5, 2, -8, 1, 9, -3, 7, -4, 6};
        sort(b.begin(), b.end(), [](int x, int y) {
            return abs(x) < abs(y);
        });
        printArr(b, "按绝对值升序");
        
        // 7.3 结构体排序
        struct Student {
            string name;
            int score;
        };
        vector<Student> stu = {
            {"小明", 90}, {"小红", 95}, {"小刚", 90}, {"小丽", 88}
        };
        // 分数降序，同分名字升序
        sort(stu.begin(), stu.end(), [](const Student& a, const Student& b) {
            if (a.score != b.score) return a.score > b.score;
            return a.name < b.name;
        });
        cout << "按分数排名:" << endl;
        for (auto& s : stu) {
            cout << "  " << s.name << " " << s.score << endl;
        }
    }
    
    return 0;
}
