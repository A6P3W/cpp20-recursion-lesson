# 第3章: 再帰による全探索・バックトラッキング

[← 第2章: メモ化再帰と分割統治法](./02-memoization-and-divide.md) | [目次に戻る](./README.md) | [第4章: 深さ優先探索（DFS）へ →](./04-dfs-grid-and-graph.md)

---

## 1. なぜ全探索に再帰を使うのか？

競技プログラミングのC問題レベルでは、「$N$ 個の要素から条件を満たす組み合わせをすべて調べる」という**全探索**が頻出します。

もし $N = 3$ 固定なら、3重の `for` ループで書けます。しかし、**$N$ が入力によって変わる（例: $1 \le N \le 20$）場合、`for` ループを $N$ 重にネストして書くことはできません。**

再帰関数を使えば、**深さ $N$ のループ（決定木）**を自在に構築できます。

---

## 2. 「選ぶ / 選ばない」の2択全探索（部分集合の列挙）

$N$ 個の整数 $A_0, A_1, \dots, A_{N-1}$ があり、それぞれを「選ぶ」か「選ばない」かの $2^N$ 通りをすべて調べるパターンです。

### 状態の設計

`dfs(index, current_sum)` のように、以下の情報を引数で持ち回ります。

- `index`: 今注目している要素の番号（$0$ から $N$ まで進む）
- `current_sum`: これまでに「選んだ」要素の合計値

```text
                       dfs(0, sum=0)
                     /               \
         [A0を選ばない]               [A0を選ぶ]
          dfs(1, 0)                  dfs(1, A0)
          /       \                  /        \
   [選ばない]    [選ぶ]        [選ばない]     [選ぶ]
  dfs(2, 0)   dfs(2, A1)     dfs(2, A0)   dfs(2, A0+A1)
```

- **ベースケース**: `index == N` に到達したら、すべての要素について「選ぶ/選ばない」の決定が終わっているので、条件（例: `current_sum == S` かどうか）を判定して `return` する。
- **再帰ステップ**:
  1. `A[index]` を**選ばない**場合：`dfs(index + 1, current_sum)`
  2. `A[index]` を**選ぶ**場合：`dfs(index + 1, current_sum + A[index])`

---

## 3. バックトラッキング（状態を進めて戻す）

「合計が $S$ になるかどうか（Yes/No）」だけでなく、**「実際にどの要素を選んだか（配列の中身）」**を列挙したい場合はどうすればよいでしょうか？

再帰呼び出しのたびに `std::vector<int>` をコピーして引数で渡すと、コピーのコスト（$O(N)$）が毎回かかってしまいます。
そこで、**1つの配列 `chosen` を共有し、「要素を追加して再帰し、帰ってきたら要素を取り除く」**という手法を使います。これを**バックトラッキング（Backtracking / 後戻り探索）**と呼びます。

### バックトラッキングの黄金パターン

```cpp
// A[index] を選ぶルートに入る前
chosen.push_back(A[index]);     // 1. 状態を進める（変更する）
self(self, index + 1);          // 2. 再帰呼び出しで深く潜る
chosen.pop_back();              // 3. 状態を元に戻す（復元する）
```

### コード例：`backtrack_combinations.cpp`

1 から $N$ までの整数の中から、ちょうど $K$ 個を選ぶ組み合わせ（$_N\mathrm{C}_K$ 通り）を辞書順にすべて出力するプログラムです。

```cpp
#include <iostream>
#include <vector>

int main() {
    int n = 4, k = 2;
    std::vector<int> chosen; // 現在選んでいる要素のリスト

    // start: 次に選ぶことができる最小の数値 (1 〜 n)
    auto dfs = [&](auto& self, int start) -> void {
        // ベースケース: k 個選び終わったら出力して戻る
        if (static_cast<int>(chosen.size()) == k) {
            std::cout << "{ ";
            for (int x : chosen) {
                std::cout << x << " ";
            }
            std::cout << "}\n";
            return;
        }

        // start から n までの数値を順番に試す
        for (int num = start; num <= n; ++num) {
            chosen.push_back(num);      // 1. num を選ぶ
            self(self, num + 1);        // 2. 次の数値 (num + 1 以降) を探しに行く
            chosen.pop_back();          // 3. バックトラック（num を外して元の状態に戻す）
        }
    };

    dfs(dfs, 1);
    return 0;
}
```

#### ビルド＆実行結果

```text
{ 1 2 }
{ 1 3 }
{ 1 4 }
{ 2 3 }
{ 2 4 }
{ 3 4 }
```

`chosen.pop_back()` があるおかげで、`{ 1 2 }` を出力して戻ってきた直後に `chosen` は `{ 1 }` に戻り、そのまま次のループで `3` を追加して `{ 1 3 }` を作ることができます。

---

## 4. 順列の全列挙（Visited配列を使ったバックトラッキング）

「$1 \sim N$ を並び替えた順列（$N!$ 通り）」を再帰で作る場合は、「どの数字をすでに使ったか」を記録する `std::vector<bool> used(n + 1, false)` を併用します。

```cpp
#include <iostream>
#include <vector>

int main() {
    int n = 3;
    std::vector<int> perm;
    std::vector<bool> used(n + 1, false);

    auto dfs = [&](auto& self) -> void {
        if (static_cast<int>(perm.size()) == n) {
            for (int i = 0; i < n; ++i) {
                std::cout << perm[i] << (i + 1 == n ? "\n" : " ");
            }
            return;
        }

        for (int num = 1; num <= n; ++num) {
            if (used[num]) continue; // すでに使用済みならスキップ

            // 1. 状態を進める
            used[num] = true;
            perm.push_back(num);

            // 2. 再帰
            self(self);

            // 3. 状態を元に戻す（バックトラック）
            perm.pop_back();
            used[num] = false;
        }
    };

    dfs(dfs);
    return 0;
}
```

> **補足**: 単純な順列列挙であれば `<algorithm>` の `std::next_permutation` でも書けますが、再帰による順列生成には**「途中で条件を満たさなくなったらその先の探索を打ち切る（枝刈り / Pruning）」**ができるという圧倒的なメリットがあります。

---

## 5. 枝刈り（Pruning）の基礎

たとえば「正の整数の集合から選んで合計をちょうど $S$ にする」問題で、探索の途中で **`current_sum > S`（すでに合計が $S$ を超えてしまった）** になったとします。

残りの要素もすべて正の整数であるなら、これ以上どんな要素を足しても合計が $S$ に戻ることは絶対にありません。
そこで、ベースケースの直前に以下の1行を入れるだけで、無駄な探索木をごっそり切り落とすことができます。

```cpp
if (current_sum > target_sum) {
    return; // 枝刈り: これ以上探索しても無駄なので引き返す
}
```

---

## 6. 章末チェックと演習課題

### この章のまとめ

- $N$ 個の要素に対する「選ぶ / 選ばない」は、深さ $N$ の2分岐再帰で $O(2^N)$ で全探索できる（目安：$N \le 20$ で約 $10^6$ 通り）。
- 共有の `vector` や `used` フラグを書き換えて再帰するときは、**再帰呼び出しの直後に必ず元の状態に戻す（バックトラック）**。
- 条件を満たす見込みがなくなった時点で `return` することを**枝刈り（Pruning）**と呼ぶ。

### ハンズオン演習へ進もう

バックトラッキングと枝刈りを使って、部分和をすべて列挙する問題に挑戦しましょう！

👉 **[演習3: 部分和問題と組み合わせ列挙 (ex03-subset-sum)](./exercises/ex03-subset-sum/problem.md)**

---

[← 第2章: メモ化再帰と分割統治法](./02-memoization-and-divide.md) | [目次に戻る](./README.md) | [第4章: 深さ優先探索（DFS）へ →](./04-dfs-grid-and-graph.md)
