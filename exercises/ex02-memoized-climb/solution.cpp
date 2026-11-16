#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, m;
    if (!(std::cin >> n >> m)) {
        return 0;
    }

    // 壊れている段を記録する配列
    std::vector<bool> broken(n + 1, false);
    for (int i = 0; i < m; ++i) {
        int a;
        std::cin >> a;
        broken[a] = true;
    }

    constexpr long long MOD = 1'000'000'007LL;

    // メモ化配列（未計算は -1 で初期化）
    std::vector<long long> memo(n + 1, -1);

    // 0段目から k段目に到達する方法の総数を返すメモ化再帰関数
    auto ways = [&](auto& self, int k) -> long long {
        // 1. 範囲外または壊れた段なら到達不可能なので 0 通り
        if (k < 0 || broken[k]) {
            return 0LL;
        }
        // 2. スタート地点 (0段目) は 1 通り
        if (k == 0) {
            return 1LL;
        }
        // 3. すでに計算済みならメモの値を返す
        if (memo[k] != -1) {
            return memo[k];
        }

        // 4. (k - 1)段目から1段上る方法 + (k - 2)段目から2段上る方法
        long long res = (self(self, k - 1) + self(self, k - 2)) % MOD;
        return memo[k] = res;
    };

    std::cout << ways(ways, n) << "\n";

    return 0;
}
