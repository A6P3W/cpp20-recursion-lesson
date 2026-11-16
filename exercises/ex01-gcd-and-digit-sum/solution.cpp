#include <iostream>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long a, b;
    if (!(std::cin >> a >> b)) {
        return 0;
    }

    // 1. 各桁の和を求める再帰ラムダ関数
    auto digit_sum = [](auto& self, long long n) -> long long {
        // ベースケース: 1桁になったらそのまま返す
        if (n < 10) {
            return n;
        }
        // 再帰ステップ: 一の位 + 残りの桁の和
        return (n % 10) + self(self, n / 10);
    };

    // 2. ユークリッドの互除法で最大公約数を求める再帰ラムダ関数
    auto my_gcd = [](auto& self, long long x, long long y) -> long long {
        // ベースケース: 余りが 0 になったら x が最大公約数
        if (y == 0) {
            return x;
        }
        // 再帰ステップ: gcd(x, y) = gcd(y, x % y)
        return self(self, y, x % y);
    };

    long long sum_a = digit_sum(digit_sum, a);
    long long sum_b = digit_sum(digit_sum, b);
    long long g = my_gcd(my_gcd, sum_a, sum_b);

    std::cout << sum_a << " " << sum_b << " " << g << "\n";

    return 0;
}
