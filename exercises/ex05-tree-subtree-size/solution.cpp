#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, q;
    if (!(std::cin >> n >> q)) {
        return 0;
    }

    // 1-indexed (頂点 1 〜 n) で隣接リストを構築
    std::vector<std::vector<int>> tree(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        std::cin >> u >> v;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    std::vector<int> depth(n + 1, 0);
    std::vector<int> subtree_size(n + 1, 0);

    // 頂点 u を探索する木DFS（parent は直前の親頂点）
    auto dfs = [&](auto& self, int u, int parent) -> void {
        subtree_size[u] = 1; // 自分自身をカウント

        for (int next_v : tree[u]) {
            if (next_v == parent) {
                continue; // 親への逆流を防ぐ
            }

            // 行きがけ順 (Pre-order): 親の深さ + 1 を子に設定
            depth[next_v] = depth[u] + 1;

            // 子頂点へ再帰
            self(self, next_v, u);

            // 帰りがけ順 (Post-order): 子の部分木サイズを親に合算
            subtree_size[u] += subtree_size[next_v];
        }
    };

    // 根である頂点 1 から 1回だけ DFS を実行して全頂点の値を前計算する (O(N))
    dfs(dfs, 1, -1);

    // 各クエリに O(1) で回答する
    for (int j = 0; j < q; ++j) {
        int x;
        std::cin >> x;
        std::cout << depth[x] << " " << subtree_size[x] << "\n";
    }

    return 0;
}
