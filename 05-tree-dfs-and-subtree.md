# 第5章: 木上のDFSと部分木集計（木DPの基礎）

[← 第4章: 深さ優先探索（DFS）](./04-dfs-grid-and-graph.md) | [目次に戻る](./README.md) | [実践演習一覧へ →](./exercises/README.md)

---

## 1. 競技プログラミングにおける「木（Tree）」とは？

グラフの中でも、**「閉路（サイクル）を持たない連結なグラフ」**を**木（Tree）**と呼びます。
AtCoderのC問題・D問題では、木構造を扱う問題が極めて高い頻度で出題されます。

### 木の3大性質

1. 頂点数が $N$ のとき、辺の本数は必ず **$N - 1$ 本** である。
2. 任意の2頂点間を結ぶ単純パス（同じ頂点を2度通らない経路）が**ただ1つ**存在する。
3. 閉路がないため、根（Root）からスタートして**「今来た道（親頂点）へ引き返さない」**というルールさえ守れば、**`visited` 配列なしで**すべての頂点をちょうど1回ずつ訪問できる。

---

## 2. 木DFSの鉄板テンプレート：`dfs(u, parent)`

一般のグラフ（第4章）では `visited` 配列を使って無限ループを防ぎましたが、木では関数の引数に **`parent`（直前にいた親頂点の番号）** を渡し、`next_v == parent` のときだけスキップするのが定石です。
（根の親は存在しないため、最初は `parent = -1` として呼び出します。）

```cpp
// u: 現在いる頂点, parent: 直前にいた親頂点（根の場合は -1）
auto dfs = [&](auto& self, int u, int parent) -> void {
    // [行きがけ順 (Pre-order) の処理はここに書く]

    for (int next_v : tree[u]) {
        if (next_v == parent) {
            continue; // 親への逆流を防ぐ！
        }
        self(self, next_v, u); // 子の親は u 自身

        // [子 next_v から帰ってきた直後の集計処理はここに書く]
    }

    // [帰りがけ順 (Post-order) の確定処理はここに書く]
};
```

---

## 3. 「行きがけ順」と「帰りがけ順」の使い分け

木上のDFSでは、処理をどのタイミングで書くかによって計算できるものが変わります。

### 3.1 行きがけ順（Pre-order）：親から子へ情報を配る

**「根からの深さ（距離） `depth[u]`」**を求める場合、親の深さ `depth[u]` がわかっていれば、子の深さは `depth[next_v] = depth[u] + 1` で即座に決まります。
このように**上から下へ情報を伝播させる処理**は、`self(self, next_v, u)` を呼ぶ**前（行きがけ）**に書きます。

### 3.2 帰りがけ順（Post-order）：子から親へ情報を集める（木DP）

**「頂点 `u` を根とする部分木（Subtree）に含まれる頂点の総数 `subtree_size[u]`」**を求める場合を考えましょう。

```text
          0 (size = 1 + 3 + 1 = 5)
         / \
(size=3) 1   2 (size = 1)
       / \
      3   4
  (size=1) (size=1)
```

頂点 $1$ の部分木サイズ（$3$）を知るためには、先にその子である頂点 $3$（サイズ $1$）と頂点 $4$（サイズ $1$）の計算が終わっている必要があります。
つまり、**「まず子へ再帰して計算を完了させ、帰ってきた結果に自分自身の分（$+1$）を足し合わせる」**という**下から上へ集める処理（帰りがけ順）**を使います。これが**木DP（Tree Dynamic Programming）**の基礎です。

---

## 4. コード例：`tree_depth_and_subtree.cpp`

頂点 $0$ を根とする木において、各頂点の「根からの深さ `depth[u]`」と「部分木のサイズ `subtree_size[u]`」を1回のDFSで同時に計算する完全なプログラムです。

```cpp
#include <iostream>
#include <vector>

int main() {
    int n = 5;
    // 辺のリスト (0-1, 0-2, 1-3, 1-4)
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 2}, {1, 3}, {1, 4}
    };

    std::vector<std::vector<int>> tree(n);
    for (auto [u, v] : edges) {
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    std::vector<int> depth(n, 0);
    std::vector<int> subtree_size(n, 0);

    auto dfs = [&](auto& self, int u, int parent) -> void {
        // 自分自身（頂点 u）をカウントして 1 で初期化
        subtree_size[u] = 1;

        for (int next_v : tree[u]) {
            if (next_v == parent) continue; // 親には戻らない

            // 【行きがけ】子の深さは「親の深さ + 1」
            depth[next_v] = depth[u] + 1;

            // 子へ潜る
            self(self, next_v, u);

            // 【帰りがけ】子 next_v の部分木サイズが決まって帰ってきたので合算する
            subtree_size[u] += subtree_size[next_v];
        }
    };

    // 根を 0 として探索開始（根の親は -1）
    dfs(dfs, 0, -1);

    for (int i = 0; i < n; ++i) {
        std::cout << "Vertex " << i
                  << ": depth = " << depth[i]
                  << ", subtree_size = " << subtree_size[i] << "\n";
    }

    return 0;
}
```

#### ビルド＆実行結果

```text
Vertex 0: depth = 0, subtree_size = 5
Vertex 1: depth = 1, subtree_size = 3
Vertex 2: depth = 1, subtree_size = 1
Vertex 3: depth = 2, subtree_size = 1
Vertex 4: depth = 2, subtree_size = 1
```

---

## 5. 章末チェックと演習課題

### この章のまとめ

- 木のDFSは `dfs(u, parent)` の形で書き、`if (next_v == parent) continue;` で逆流を防ぐ。
- **根から葉へ配る値（深さ・距離など）**は**行きがけ（再帰呼び出しの前）**に計算する。
- **葉から根へ集計する値（部分木のサイズ・部分木の重み総和など）**は**帰りがけ（再帰呼び出しの後）**に合算する。
- 頂点数 $N$ の木DFSの計算量は **$O(N)$** であり、$N = 2 \times 10^5$ でも一瞬で計算が終わるため、大量のクエリ問題にも事前計算として対応できる。

### ハンズオン演習へ進もう

コースの総仕上げとして、木上のDFSで部分木情報を事前計算し、クエリに高速に答える問題に挑戦しましょう！

👉 **[演習5: 木の部分木サイズ集計 (ex05-tree-subtree-size)](./exercises/ex05-tree-subtree-size/problem.md)**

---

[← 第4章: 深さ優先探索（DFS）](./04-dfs-grid-and-graph.md) | [目次に戻る](./README.md) | [実践演習一覧へ →](./exercises/README.md)
