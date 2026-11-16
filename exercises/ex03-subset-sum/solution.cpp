#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long s;
    if (!(std::cin >> n >> s)) {
        return 0;
    }

    std::vector<long long> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    std::vector<long long> chosen;
    std::vector<std::vector<long long>> answers;

    // index: 現在見ている要素の添字 (0 〜 n)
    // current_sum: 現在選んでいる要素の合計
    auto dfs = [&](auto& self, int index, long long current_sum) -> void {
        // 枝刈り (Pruning): すべて正の整数なので、すでに s を超えていたらこれ以上探索しない
        if (current_sum > s) {
            return;
        }

        // ベースケース: すべての要素について「選ぶ / 選ばない」を決定し終えた
        if (index == n) {
            if (current_sum == s) {
                answers.push_back(chosen);
            }
            return;
        }

        // 分岐1: a[index] を「選ぶ」
        chosen.push_back(a[index]);
        self(self, index + 1, current_sum + a[index]);
        chosen.pop_back(); // バックトラック（状態復元）

        // 分岐2: a[index] を「選ばない」
        self(self, index + 1, current_sum);
    };

    dfs(dfs, 0, 0LL);

    // 結果の出力
    std::cout << answers.size() << "\n";
    for (const auto& seq : answers) {
        for (size_t i = 0; i < seq.size(); ++i) {
            std::cout << seq[i] << (i + 1 == seq.size() ? "" : " ");
        }
        std::cout << "\n";
    }

    return 0;
}
