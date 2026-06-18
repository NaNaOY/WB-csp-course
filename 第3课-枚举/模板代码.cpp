/**
 * 第3课：枚举（Enumeration）—— 模板代码
 * 
 * 包含内容：
 *   1. 简单枚举（数字统计）
 *   2. 嵌套枚举（三元组）
 *   3. 预处理 + 枚举（火柴棒）
 *   4. 全排列枚举（next_permutation）
 *   5. 组合枚举（递归 / 子集枚举）
 * 
 * 编译：g++ -std=c++17 -O2 模板代码.cpp -o 模板代码
 * 运行：./模板代码
 */

#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

// ============================================================
// 1. 简单枚举：分离数字的每一位
// ============================================================

// 统计整数 n 中数字 digit 出现的次数
int count_digit(int n, int digit) {
    if (n == 0) return (digit == 0) ? 1 : 0;  // 特判 0
    int cnt = 0;
    while (n > 0) {
        if (n % 10 == digit) cnt++;  // 取个位，判断
        n /= 10;                      // 去掉个位
    }
    return cnt;
}

// 统计区间 [L, R] 中数字 digit 出现的总次数
int count_in_range(int L, int R, int digit = 2) {
    int ans = 0;
    for (int i = L; i <= R; i++) {   // 枚举 [L, R] 每个数
        ans += count_digit(i, digit); // 累加该数中的 digit 个数
    }
    return ans;
}

// ============================================================
// 2. 嵌套枚举：三元组 a + b + c = target
// ============================================================

// 朴素版（三重循环）
void triple_naive(int target) {
    cout << "=== 三元组（朴素版·三重循环）===" << endl;
    int cnt = 0;
    for (int a = 1; a <= target; a++) {
        for (int b = 1; b <= target; b++) {
            for (int c = 1; c <= target; c++) {
                if (a + b + c == target) {
                    // printf("(%d, %d, %d)\n", a, b, c);
                    cnt++;
                }
            }
        }
    }
    cout << "共找到 " << cnt << " 组解" << endl;
}

// 优化版（缩小范围，不用枚举c）
void triple_optimized(int target) {
    cout << "=== 三元组（优化版·减范围）===" << endl;
    int cnt = 0;
    for (int a = 1; a <= target - 2; a++) {
        for (int b = 1; b <= target - a - 1; b++) {
            int c = target - a - b;  // 不用枚举 c！
            (void)c;  // 避免未使用警告
            // printf("(%d, %d, %d)\n", a, b, c);
            cnt++;
        }
    }
    cout << "共找到 " << cnt << " 组解" << endl;
}

// ============================================================
// 3. 预处理 + 枚举：火柴棒等式
// ============================================================

// 数字 0~9 各需要的火柴棒数
const int COST[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

// 预处理：计算 0~999 每个数需要多少火柴棒（查表法）
vector<int> pre_match(1000);

void build_match_table() {
    for (int i = 0; i <= 999; i++) {
        if (i == 0) pre_match[i] = COST[0];
        else {
            int sum = 0, x = i;
            while (x > 0) {
                sum += COST[x % 10];
                x /= 10;
            }
            pre_match[i] = sum;
        }
    }
}

// 解火柴棒等式：n 根火柴，拼 A + B = C
int matchsticks_equation(int n) {
    build_match_table();
    int ans = 0;
    for (int a = 0; a <= 999; a++) {
        for (int b = 0; b <= 999; b++) {
            int c = a + b;
            if (c > 999) continue;  // C 太大，跳过
            // 关键：总火柴数 == n（+4 是「+」和「=」）
            if (pre_match[a] + pre_match[b] + pre_match[c] + 4 == n) {
                ans++;
            }
        }
    }
    return ans;
}

// ============================================================
// 4. 全排列枚举：next_permutation
// ============================================================

// 生成并打印 1~n 的全排列
void print_permutations(int n) {
    vector<int> v(n);
    iota(v.begin(), v.end(), 1);  // 填充 1, 2, 3, ..., n
    
    cout << "=== 1~" << n << " 的全排列（前10个）===" << endl;
    int cnt = 0;
    do {
        for (int x : v) cout << x << " ";
        cout << endl;
        cnt++;
    } while (next_permutation(v.begin(), v.end()) && cnt < 10);
    cout << "...（共 " << cnt << " 个排列被打印）" << endl;
}

// ============================================================
// 5. 组合枚举：从 n 个数中选 k 个
// ============================================================

// 方法1：用二进制位表示选/不选（子集枚举）
void combination_bitmask(int n, int k) {
    cout << "=== 从 " << n << " 个中选 " << k << " 个（位掩码法）===" << endl;
    int cnt = 0;
    for (int mask = 0; mask < (1 << n); mask++) {
        // __builtin_popcount 统计 mask 的二进制表示中 1 的个数
        if (__builtin_popcount(mask) != k) continue;
        
        // 输出选中的元素（编号从 1 开始）
        cout << "{ ";
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) cout << (i + 1) << " ";
        }
        cout << "}" << endl;
        cnt++;
    }
    cout << "共 " << cnt << " 种组合 (C(" << n << "," << k << ") = "
         << cnt << ")" << endl;
}

// 方法2：递归方式生成组合（回溯）
void dfs_combine(int start, int n, int k, vector<int>& cur, int& cnt) {
    if (cur.size() == (size_t)k) {
        cout << "{ ";
        for (int x : cur) cout << x << " ";
        cout << "}" << endl;
        cnt++;
        return;
    }
    for (int i = start; i <= n; i++) {
        cur.push_back(i);
        dfs_combine(i + 1, n, k, cur, cnt);
        cur.pop_back();  // 回溯！
    }
}

// ============================================================
// 综合演示主函数
// ============================================================

int main() {
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║   第3课：枚举法 — 模板代码演示        ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    cout << endl;

    // 1. 简单枚举演示
    cout << "【1】简单枚举：统计 [2, 22] 中数字 2 的个数" << endl;
    int ans1 = count_in_range(2, 22, 2);
    cout << "  结果: " << ans1 << "  (期望: 6)" << endl;
    cout << endl;

    // 2. 嵌套枚举演示
    cout << "【2】嵌套枚举：a+b+c=10 的正整数解" << endl;
    triple_naive(10);
    triple_optimized(10);
    cout << endl;

    // 3. 火柴棒等式演示
    cout << "【3】火柴棒等式：n=18 时的解数" << endl;
    int ans3 = matchsticks_equation(18);
    cout << "  结果: " << ans3 << "  (期望: 9)" << endl;
    cout << endl;

    // 4. 全排列演示
    print_permutations(3);
    cout << endl;

    // 5. 组合枚举演示
    combination_bitmask(5, 3);
    cout << endl;
    
    cout << "【5-递归法】从 5 个中选 3 个（回溯法）" << endl;
    {
        vector<int> cur;
        int cnt = 0;
        dfs_combine(1, 5, 3, cur, cnt);
        cout << "共 " << cnt << " 种组合" << endl;
    }

    return 0;
}
