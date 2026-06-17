/**
 * 高精度计算 —— 核心模板代码
 * 
 * 包含四个核心运算：
 *   1. 高精度加法（add）
 *   2. 高精度减法（sub）—— 需要 cmp 判断大小
 *   3. 高精度乘法（mul_lo 高×低、mul_hi 高×高）
 *   4. 高精度除法（div_lo 高÷低）
 * 
 * 统一约定：
 *   - 所有大数用 vector<int> 存储，下标 0 存个位（倒序存储）
 *   - 输入用 string 读取，转换为倒序 vector<int>
 *   - 输出时倒序遍历 vector
 * 
 * 编译：g++ -std=c++17 模板代码.cpp -o 模板代码
 * 运行：./模板代码
 */

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ==================== 工具函数 ====================

/**
 * 输入一个大数，返回倒序存储的 vector<int>
 * 例：输入 "12345" → 返回 [5, 4, 3, 2, 1]
 */
vector<int> read_bigint() {
    string s;
    cin >> s;
    vector<int> A;
    for (int i = s.size() - 1; i >= 0; i--)
        A.push_back(s[i] - '0');   // '5' - '0' = 5
    return A;
}

/**
 * 输出一个大数（倒序存储 → 正序输出）
 */
void print_bigint(vector<int>& A) {
    for (int i = A.size() - 1; i >= 0; i--)
        cout << A[i];
    cout << endl;
}

/**
 * 判断 A >= B（两个都是倒序存储）
 * 注意：比较时从高位（数组末尾）开始
 */
bool cmp(vector<int>& A, vector<int>& B) {
    if (A.size() != B.size())
        return A.size() > B.size();          // 位数多的更大
    for (int i = A.size() - 1; i >= 0; i--)  // 从高位逐位比较
        if (A[i] != B[i])
            return A[i] > B[i];
    return true;                             // 完全相等
}

// ==================== 1. 高精度加法 ====================

/**
 * C = A + B
 * 时间复杂度：O(max(lenA, lenB))
 */
vector<int> add(vector<int>& A, vector<int>& B) {
    vector<int> C;
    int t = 0;                               // t：当前位的"总和"
                                              // t % 10 = 当前位结果
                                              // t / 10 = 进位值
    for (int i = 0; i < A.size() || i < B.size(); i++) {
        if (i < A.size()) t += A[i];         // 加上A的当前位
        if (i < B.size()) t += B[i];         // 加上B的当前位
        C.push_back(t % 10);                 // 当前位结果
        t /= 10;                             // 进位
    }
    if (t) C.push_back(t);                   // 最高位进位
    return C;
}

// ==================== 2. 高精度减法 ====================

/**
 * C = A - B（前提：A >= B）
 * 时间复杂度：O(lenA)
 */
vector<int> sub(vector<int>& A, vector<int>& B) {
    vector<int> C;
    int t = 0;                               // t：借位标记（0或1）
    for (int i = 0; i < A.size(); i++) {
        t = A[i] - t;                        // 先扣掉上一轮借位
        if (i < B.size()) t -= B[i];         // 再减去B的当前位
        C.push_back((t + 10) % 10);          // (t+10)%10 同时处理够减和不够减
        if (t < 0) t = 1;                    // 不够减 → 向高位借1
        else t = 0;                          // 够减 → 无需借位
    }
    // 去除前导零（如 100-99=001 → 1）
    while (C.size() > 1 && C.back() == 0)
        C.pop_back();
    return C;
}

// ==================== 3. 高精度乘法 ====================

/**
 * 3.1 高精度 × 低精度：C = A * b
 * 时间复杂度：O(lenA)
 */
vector<int> mul_lo(vector<int>& A, int b) {
    vector<int> C;
    int t = 0;                               // 进位
    for (int i = 0; i < A.size() || t; i++) {
        if (i < A.size()) t += A[i] * b;     // 当前位×b + 进位
        C.push_back(t % 10);                 // 当前位结果
        t /= 10;                             // 剩余进位
    }
    // 去除前导零（防止 b=0 的情况，输出 "0" 而非空）
    while (C.size() > 1 && C.back() == 0)
        C.pop_back();
    return C;
}

/**
 * 3.2 高精度 × 高精度：C = A * B
 * 思路：二重循环，C[i+j] += A[i] * B[j]，最后统一处理进位
 * 时间复杂度：O(lenA × lenB)
 */
vector<int> mul_hi(vector<int>& A, vector<int>& B) {
    vector<int> C(A.size() + B.size());      // 最多 lenA+lenB 位
    
    // 逐位相乘，累加到对应位置
    for (int i = 0; i < A.size(); i++)
        for (int j = 0; j < B.size(); j++)
            C[i + j] += A[i] * B[j];
    
    // 统一处理进位
    int t = 0;
    for (int i = 0; i < C.size(); i++) {
        t += C[i];
        C[i] = t % 10;
        t /= 10;
    }
    
    // 去除前导零
    while (C.size() > 1 && C.back() == 0)
        C.pop_back();
    return C;
}

// ==================== 4. 高精度除法（÷ 低精度） ====================

/**
 * C = A / b，余数通过引用参数 r 返回
 * 注意：除法从高位到低位处理（和加/减/乘方向相反！）
 * 时间复杂度：O(lenA)
 */
vector<int> div_lo(vector<int>& A, int b, int& r) {
    vector<int> C;
    r = 0;
    // 从高位到低位逐位处理
    for (int i = A.size() - 1; i >= 0; i--) {
        r = r * 10 + A[i];                   // 把当前位"降下来"
        C.push_back(r / b);                  // 当前位的商
        r %= b;                              // 新的余数
    }
    // C 现在是正序（高位在前），需要反转成倒序
    reverse(C.begin(), C.end());
    // 去除前导零
    while (C.size() > 1 && C.back() == 0)
        C.pop_back();
    return C;
}

// ==================== 综合演示 ====================

int main() {
    // ---------- 演示1：加法 ----------
    cout << "=== 高精度加法演示 ===" << endl;
    cout << "输入两个大数 A 和 B：" << endl;
    vector<int> A = read_bigint();
    vector<int> B = read_bigint();
    vector<int> C = add(A, B);
    cout << "A + B = ";
    print_bigint(C);
    
    // ---------- 演示2：减法 ----------
    cout << "\n=== 高精度减法演示 ===" << endl;
    cout << "输入两个大数 A 和 B：" << endl;
    A = read_bigint();
    B = read_bigint();
    cout << "A - B = ";
    if (cmp(A, B)) {
        C = sub(A, B);
        print_bigint(C);
    } else {
        cout << "-";
        C = sub(B, A);
        print_bigint(C);
    }
    
    // ---------- 演示3：乘法 ----------
    cout << "\n=== 高精度乘法演示 ===" << endl;
    cout << "输入大数 A 和低精度整数 b：" << endl;
    A = read_bigint();
    int b;
    cin >> b;
    C = mul_lo(A, b);
    cout << "A × " << b << " = ";
    print_bigint(C);
    
    // ---------- 演示4：除法 ----------
    cout << "\n=== 高精度除法演示 ===" << endl;
    cout << "输入大数 A 和低精度整数 b：" << endl;
    A = read_bigint();
    cin >> b;
    int r;
    C = div_lo(A, b, r);
    cout << "A ÷ " << b << " = ";
    print_bigint(C);
    cout << "余数 = " << r << endl;
    
    // ---------- 演示5：阶乘 ----------
    cout << "\n=== 阶乘演示：计算 n! ===" << endl;
    cout << "输入 n（建议 ≤ 100）：" << endl;
    int n;
    cin >> n;
    vector<int> fact = {1};
    for (int i = 2; i <= n; i++) {
        fact = mul_lo(fact, i);
    }
    cout << n << "! = ";
    print_bigint(fact);
    
    return 0;
}

/*
 测试样例：

 [加法]
 输入：189 277
 输出：466

 [减法]
 输入：500 277
 输出：223

 [乘法]
 输入：12345 28
 输出：345660

 [除法]
 输入：8123 25
 输出：324 ... 余数 23

 [阶乘]
 输入：10
 输出：3628800
*/
