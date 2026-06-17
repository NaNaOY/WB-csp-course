/**
 * 例题：P1009 阶乘之和
 * 来源：洛谷 P1009
 * 难度：普及-（橙题）
 * 
 * 题目描述：
 *   用高精度计算 S = 1! + 2! + 3! + ... + n!（n ≤ 50）
 *   输出 S 的值。
 * 
 * 解题思路：
 *   本题综合了高精度乘法和加法。
 *   - fact 变量存储当前阶乘值 i!
 *   - sum 变量存储阶乘之和
 *   - 每轮迭代：
 *     1. fact = fact × i（高精度 × 低精度）
 *     2. sum = sum + fact（高精度 + 高精度）
 *   
 *   为什么必须用高精度？
 *   20! ≈ 2.43 × 10^18，恰好超过 long long 上限
 *   50! ≈ 3.04 × 10^64，需要约 65 位数字
 * 
 * 时间复杂度：O(n² log n)，n ≤ 50 完全足够
 */

#include <bits/stdc++.h>
using namespace std;

// 高精度加法
vector<int> add(vector<int>& A, vector<int>& B) {
    vector<int> C;
    int t = 0;
    for (int i = 0; i < A.size() || i < B.size(); i++) {
        if (i < A.size()) t += A[i];
        if (i < B.size()) t += B[i];
        C.push_back(t % 10);
        t /= 10;
    }
    if (t) C.push_back(t);
    return C;
}

// 高精度 × 低精度
vector<int> mul_lo(vector<int>& A, int b) {
    vector<int> C;
    int t = 0;
    for (int i = 0; i < A.size() || t; i++) {
        if (i < A.size()) t += A[i] * b;
        C.push_back(t % 10);
        t /= 10;
    }
    while (C.size() > 1 && C.back() == 0)
        C.pop_back();
    return C;
}

int main() {
    int n;
    cin >> n;
    
    vector<int> fact = {1};   // fact = 1! = 1
    vector<int> sum  = {1};   // sum  = 1
                               // (题目要求从1!开始累加)
    
    for (int i = 2; i <= n; i++) {
        fact = mul_lo(fact, i);    // fact = fact × i → 得到 i!
        sum  = add(sum, fact);     // sum = sum + i!
    }
    
    // 倒序输出
    for (int i = sum.size() - 1; i >= 0; i--)
        cout << sum[i];
    cout << endl;
    
    return 0;
}

/*
 测试样例：
 
 样例1：
 输入：3
 输出：9
 解释：1! + 2! + 3! = 1 + 2 + 6 = 9
 
 样例2：
 输入：10
 输出：4037913
 解释：1! + 2! + ... + 10! = 4037913
 
 样例3：
 输入：50
 输出：31035053229546199656252032972759319953190362094566672920420940313
 （65位数字）
*/
