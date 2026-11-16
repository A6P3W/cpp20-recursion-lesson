# C++20 再帰関数と探索アルゴリズム — 競技プログラミング実践コース

競技プログラミング（AtCoder ABC A〜D問題レベル）で頻出する**再帰関数**の基礎から、**メモ化再帰・分割統治・バックトラッキング全探索・深さ優先探索（DFS）・木上のDFS**までを体系的に習得するハンズオン教材です。

すべてのサンプルコードおよび演習解答は、外部ライブラリに依存せず **`g++ -std=c++20 <file>.cpp -o <out>`** の1コマンドでビルド・実行できるように設計されています。

---

## 動作環境・前提条件

- **C++規格**: C++20 (`-std=c++20`)
- **動作確認済みコンパイラ**: `g++` (GCC 13.2.0)
- **前提知識**: C++の基本文法（変数、条件分岐、ループ、`std::vector`、関数の基本）

### 基本のビルド＆実行コマンド

```bash
g++ -std=c++20 -O2 -Wall -Wextra solution.cpp -o solution
./solution
```

> **Windows (PowerShell / コマンドプロンプト) の場合**:
> `./solution` または `.\solution.exe` で実行できます。

---

## カリキュラム一覧

| 章        | タイトル                                                            | 学べるアルゴリズム・技法                                                                                  | 対応する演習                                                                 |
| :-------- | :------------------------------------------------------------------ | :-------------------------------------------------------------------------------------------------------- | :--------------------------------------------------------------------------- |
| **第1章** | [再帰関数の基本とC++20での書き方](./01-recursion-basics.md)         | ベースケースと再帰ステップ、コールスタック、ユークリッドの互除法、C++20ジェネリックラムダ再帰             | [演習1: 桁和と最大公約数](./exercises/ex01-gcd-and-digit-sum/problem.md)     |
| **第2章** | [メモ化再帰と分割統治法](./02-memoization-and-divide.md)            | 重複部分問題、`std::vector` によるメモ化再帰（トップダウンDP）、繰り返し二乗法（二分累乗）                | [演習2: 壊れた階段の上り方](./exercises/ex02-memoized-climb/problem.md)      |
| **第3章** | [再帰による全探索・バックトラッキング](./03-backtracking-search.md) | 「選ぶ/選ばない」の2択全探索、状態遷移と復元（`push_back`/`pop_back`）、枝刈り（Pruning）                 | [演習3: 部分和問題と組み合わせ列挙](./exercises/ex03-subset-sum/problem.md)  |
| **第4章** | [深さ優先探索（DFS）— グリッドとグラフ](./04-dfs-grid-and-graph.md) | 2次元グリッド4方向DFS、隣接リストによるグラフ表現、連結成分のカウント、到達可能性判定                     | [演習4: 島の個数カウント](./exercises/ex04-grid-island-count/problem.md)     |
| **第5章** | [木上のDFSと部分木集計](./05-tree-dfs-and-subtree.md)               | 木構造の性質、親頂点 `parent` による逆流防止、行きがけ順（Pre-order）と帰りがけ順（Post-order）、部分木DP | [演習5: 木の部分木サイズ集計](./exercises/ex05-tree-subtree-size/problem.md) |

---

## 実践演習（Exercises）

各章の理解を定着させるための実践演習を [./exercises/README.md](./exercises/README.md) に用意しています。

- [演習一覧・進め方ガイド](./exercises/README.md)
  1. [ex01: 桁和とユークリッドの互除法](./exercises/ex01-gcd-and-digit-sum/problem.md)（[解答コード](./exercises/ex01-gcd-and-digit-sum/solution.cpp) / [解説](./exercises/ex01-gcd-and-digit-sum/solution_guide.md)）
  2. [ex02: 壊れた階段の上り方（メモ化再帰）](./exercises/ex02-memoized-climb/problem.md)（[解答コード](./exercises/ex02-memoized-climb/solution.cpp) / [解説](./exercises/ex02-memoized-climb/solution_guide.md)）
  3. [ex03: 部分和問題（バックトラッキング全探索）](./exercises/ex03-subset-sum/problem.md)（[解答コード](./exercises/ex03-subset-sum/solution.cpp) / [解説](./exercises/ex03-subset-sum/solution_guide.md)）
  4. [ex04: 島の個数カウント（グリッドDFS）](./exercises/ex04-grid-island-count/problem.md)（[解答コード](./exercises/ex04-grid-island-count/solution.cpp) / [解説](./exercises/ex04-grid-island-count/solution_guide.md)）
  5. [ex05: 木の部分木サイズ集計（木DFS）](./exercises/ex05-tree-subtree-size/problem.md)（[解答コード](./exercises/ex05-tree-subtree-size/solution.cpp) / [解説](./exercises/ex05-tree-subtree-size/solution_guide.md)）

---

## おすすめの学習ステップ

1. **各章のMarkdownを読む**: サンプルコードを手元でコンパイル・実行しながら、再帰呼び出しの流れを追います。
2. **章末に対応する演習問題を解く**: まずは `problem.md` だけを見て、自力で実装してみます。
3. **解答・解説と比較する**: `solution.cpp` と `solution_guide.md` を読み、計算量やバグを防ぐ書き方（Gotchas）を確認します。

---

[第1章: 再帰関数の基本とC++20での書き方へ進む →](./01-recursion-basics.md)
