/**
 * 例题：P1601 A+B Problem（高精）
 * 来源：洛谷 P1601
 * 难度：入门（红题）
 * 
 * 题目描述：
 *   高精度加法，求两个非负整数之和。
 *   每个数不超过 10^500 位。
 * 
 * 解题思路：
 *   - 用 string 读入两个大数
 *   - 转为倒序 vector<int>（个位在 index 0）
 *   - 模拟竖式加法：从低位到高位逐位相加，处理进位
 *   - 倒序输出结果
 * 
 * 时间复杂度：O(max(lenA, lenB))
 * 空间复杂度：O(max(lenA, lenB))
 */

#include <bits/stdc++.h>
using namespace std;

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

int main() {
    // 读入两个大数
    string a, b;
    cin >> a >> b;
    
    // 转为倒序 vector
    vector<int> A, B;
    for (int i = a.size() - 1; i >= 0; i--) A.push_back(a[i] - '0');
    for (int i = b.size() - 1; i >= 0; i--) B.push_back(b[i] - '0');
    
    // 加法
    vector<int> C = add(A, B);
    
    // 倒序输出
    for (int i = C.size() - 1; i >= 0; i--)
        cout << C[i];
    cout << endl;
    
    return 0;
}

/*
 测试样例：
 
 样例1：
 输入：
 12345678901234567890
 98765432109876543210
 输出：
 111111111011111111100
 
 样例2：
 输入：
 99999999999999999999
 1
 输出：
 100000000000000000000
*/
