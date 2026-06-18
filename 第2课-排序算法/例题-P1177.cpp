/**
 * 例题：【模板】快速排序
 * 来源：洛谷 P1177
 * 难度：⭐⭐（基础）
 * 
 * 题目：给定 N 个整数，从小到大排序后输出。
 *       N ≤ 10^5，|a[i]| ≤ 10^9
 * 
 * 解题思路：
 *   直接使用 C++ STL 的 sort() 函数。
 *   sort() 底层是快速排序 + 插入排序 + 堆排序的混合实现，
 *   时间复杂度 O(N log N)，完全满足本题要求。
 * 
 * 注意：
 *   - N=10^5 时需要关闭同步流加速输入输出
 *   - 如果要求稳定排序（保持相等元素的原顺序），用 stable_sort()
 * 
 * 测试样例：
 *   输入：
 *     5
 *     4 2 4 5 1
 *   输出：
 *     1 2 4 4 5
 */

#include <bits/stdc++.h>
using namespace std;

int main() {
    // 关闭同步流，加速输入输出（N=10^5 时很重要！）
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    // STL sort：从小到大
    sort(a.begin(), a.end());

    // 输出结果
    for (int i = 0; i < n; i++) {
        cout << a[i] << (i == n - 1 ? '\n' : ' ');
    }

    return 0;
}
