# 第4章: 深さ優先探索（DFS）— グリッドとグラフ

[← 第3章: バックトラッキング全探索](./03-backtracking-search.md) | [目次に戻る](./README.md) | [第5章: 木上のDFSと部分木集計へ →](./05-tree-dfs-and-subtree.md)

---

## 1. 深さ優先探索（DFS: Depth-First Search）とは？

**深さ優先探索（DFS）**とは、迷路やグラフの頂点を探索するときに、**「行けるところまでとにかく奥（深さ方向）へ進み、行き止まりになったら一歩手前に戻って別ルートを試す」**アルゴリズムです。

この「奥へ進み、行き止まりで一歩手前に戻る」という動きは、再帰関数のコールスタックの動きそのものです。

### バックトラッキング（第3章）とDFS（第4章）の決定的な違い

- **第3章（バックトラッキング）**: すべてのルートを別々に試したいので、帰りがけに `used[v] = false` と**訪問フラグを戻しました**。
- **第4章（到達判定・連結成分のDFS）**: 「一度でも調べた頂点（マス）には二度と入る必要がない」ため、`visited[v] = true` にしたら**最後まで `true` のままにします（戻さない）**。これにより、サイクル（閉路）があっても無限ループにならず、頂点数 $V$・辺数 $E$ に対して **$O(V + E)$** という高速な計算量で全頂点を探索できます。

---

## 2. グラフ表現の基本：隣接リストとDFS

競技プログラミングで $N$ 頂点 $M$ 辺のグラフを扱うときは、`std::vector<std::vector<int>> graph(N)` という**隣接リスト（Adjacency List）**を使います。
`graph[u]` には「頂点 `u` と直接辺でつながっている頂点のリスト」が入ります。

### コード例：`graph_dfs.cpp`（連結成分の探索）

頂点 $0$ からスタートして、辺をたどって到達できるすべての頂点を調べるプログラムです。

```cpp
#include <iostream>
#include <vector>

int main() {
    int n = 6; // 頂点数 (0 〜 5)
    std::vector<std::pair<int, int>> edges = {
        {0, 1}, {0, 2}, {1, 3}, {2, 3}, // 0-1-2-3 のグループ（閉路あり）
        {4, 5}                          // 4-5 の独立したグループ
    };

    // 1. 隣接リストの構築（無向グラフなので双方向に張る）
    std::vector<std::vector<int>> graph(n);
    for (auto [u, v] : edges) {
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    // 2. 訪問済みフラグ配列
    std::vector<bool> visited(n, false);

    // 3. DFS関数
    auto dfs = [&](auto& self, int u) -> void {
        visited[u] = true; // 頂点 u に到着したら即座に訪問済みにする
        std::cout << "Visited vertex: " << u << "\n";

        // u に隣接するすべての頂点 next_v を調べる
        for (int next_v : graph[u]) {
            if (visited[next_v]) {
                continue; // すでに訪問済みならスキップ（無限ループ防止）
            }
            self(self, next_v);
        }
    };

    // 頂点 0 から探索開始
    dfs(dfs, 0);

    // どの頂点に到達できたか確認
    for (int i = 0; i < n; ++i) {
        std::cout << "Vertex " << i << ": "
                  << (visited[i] ? "Reachable" : "Unreachable") << "\n";
    }

    return 0;
}
```

#### ポイント：いつ `visited[u] = true` にするか？

関数の**先頭（頂点に入った瞬間）**で `visited[u] = true` にするのが最も安全でバグのない書き方です。

---

## 3. 2次元グリッド上のDFS（迷路・島の数え上げ）

競技プログラミングで非常によく出るのが、$H$ 行 $W$ 列のマス目（グリッド）が与えられ、上下左右の隣接マスへ移動する問題です。
グリッドも「各マスが頂点、上下左右の隣接関係が辺」であるグラフの一種と見なせます。

### 4方向移動の方向ベクトル `dr`, `dc`

上下左右の4方向を `if` 文で4回書くとバグの温床になります。以下のように移動量配列 `dr`, `dc` を定義して `for (int dir = 0; dir < 4; ++dir)` で回すのが競プロの定石です。

```cpp
// 上・下・左・右 の4方向への移動量
constexpr int dr[4] = {-1, 1, 0, 0};
constexpr int dc[4] = {0, 0, -1, 1};
```

> **8方向（斜め移動あり）の場合**は、長さ8の配列にするか、`for (int dr = -1; dr <= 1; ++dr) for (int dc = -1; dc <= 1; ++dc)` で `(dr == 0 && dc == 0)` を除外すれば簡単に書けます。

### グリッドDFSの境界チェック（範囲外アクセスの防止）

隣のマス `(nr, nc)` に進む前に、必ず以下の3つを順番に確認します。

1. **グリッドの範囲内か？**: `0 <= nr && nr < H && 0 <= nc && nc < W`
2. **通行可能なマスか？**: `grid[nr][nc] != '#'`（壁ではないか）
3. **未訪問か？**: `!visited[nr][nc]`

> **絶対注意**: 必ず **1（範囲内チェック）を最初** に行ってください。範囲チェックより先に `grid[nr][nc]` にアクセスすると、負の添字や範囲外アクセスで未定義動作（Segmentation Fault / RE）になります。

### コード例：`grid_maze_dfs.cpp`

スタート `S` からゴール `G` まで壁 `#` を避けて到達できるかを判定する完全なコードです。

```cpp
#include <iostream>
#include <string>
#include <vector>

int main() {
    int h = 4, w = 5;
    std::vector<std::string> grid = {
        "S.#..",
        "..#..",
        ".#..#",
        "...#G"
    };

    int sr = 0, sc = 0, gr = 3, gc = 4;

    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
    constexpr int dr[4] = {-1, 1, 0, 0};
    constexpr int dc[4] = {0, 0, -1, 1};

    auto dfs = [&](auto& self, int r, int c) -> void {
        visited[r][c] = true;

        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr[dir];
            int nc = c + dc[dir];

            // 1. グリッドの範囲外ならスキップ
            if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
            // 2. 壁 '#' ならスキップ
            if (grid[nr][nc] == '#') continue;
            // 3. 訪問済みならスキップ
            if (visited[nr][nc]) continue;

            self(self, nr, nc);
        }
    };

    dfs(dfs, sr, sc);

    if (visited[gr][gc]) {
        std::cout << "Goal is reachable!\n";
    } else {
        std::cout << "Goal is NOT reachable.\n";
    }

    return 0;
}
```

---

## 4. 連結成分（塊）の個数を数えるアルゴリズム

「海に浮かぶ島の数を数えよ」「つながっているグループの数を求めよ」という問題は、**外側のループとDFSを組み合わせる**ことで簡単に解けます。

### アルゴリズムの流れ

1. カウンタ `count = 0` を用意する。
2. すべての頂点（またはすべての陸地マス）を順番に調べる。
3. もし「まだ未訪問の陸地マス」を見つけたら：
   - 新しい島を見つけたので **`count++`** する。
   - そのマスを始点として **DFS を1回実行** し、その島とつながっているすべての陸地マスを `visited = true` に塗りつぶす。
4. 全マスを見終わったときの `count` が答え！

---

## 5. 章末チェックと演習課題

### この章のまとめ

- グラフやグリッドの到達判定・塊の塗りつぶしには、`visited` を戻さない **DFS** を使う（計算量 $O(V + E)$ または $O(HW)$）。
- グリッドの移動は方向配列 `dr`, `dc` を使い、**必ず配列の範囲チェックを最初に行う**。
- 連結成分（島）の個数は、「未訪問を見つけたらカウントを $+1$ してDFSで塊ごと訪問済みにする」で求められる。

### ハンズオン演習へ進もう

グリッドDFSの最重要典型問題である「島の個数カウント」を実装してみましょう！

👉 **[演習4: 島の個数カウント (ex04-grid-island-count)](./exercises/ex04-grid-island-count/problem.md)**

---

[← 第3章: バックトラッキング全探索](./03-backtracking-search.md) | [目次に戻る](./README.md) | [第5章: 木上のDFSと部分木集計へ →](./05-tree-dfs-and-subtree.md)
