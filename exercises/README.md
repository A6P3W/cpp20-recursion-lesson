# 実践演習（Exercises）ガイド

[← コース目次に戻る](../README.md)

このディレクトリには、全5章に対応する競技プログラミング形式の演習課題が収録されています。

---

## ディレクトリ構成

各演習ディレクトリ（`ex01`〜`ex05`）は以下の3ファイルで構成されています。

- `problem.md`: 問題文、制約、入力形式、出力形式、入出力例、ヒント
- `solution.cpp`: 単体で `g++` ビルド・実行できる完全な解答コード
- `solution_guide.md`: 解法の着眼点、ステップバイステップ解説、計算量、よくある間違い（Gotchas）

---

## ビルドと動作確認の方法

各演習ディレクトリに移動し、`g++` でコンパイルして実行してください。

```bash
# 例: ex01 をビルドして実行する場合
cd exercises/ex01-gcd-and-digit-sum
g++ -std=c++20 -O2 -Wall -Wextra solution.cpp -o solution
```

### 標準入力の渡し方

**Linux / macOS / Git Bash の場合:**

```bash
echo "12345 54321" | ./solution
```

**Windows PowerShell の場合:**

```powershell
"12345 54321" | .\solution.exe
```

---

## 演習一覧

| 演習     | テーマ                           | 難易度目安 | 問題                                              | 解答コード                                            | 解説                                                            |
| :------- | :------------------------------- | :--------- | :------------------------------------------------ | :---------------------------------------------------- | :-------------------------------------------------------------- |
| **ex01** | 基本再帰（桁和とGCD）            | ABC-B      | [problem.md](./ex01-gcd-and-digit-sum/problem.md) | [solution.cpp](./ex01-gcd-and-digit-sum/solution.cpp) | [solution_guide.md](./ex01-gcd-and-digit-sum/solution_guide.md) |
| **ex02** | メモ化再帰（階段の上り方）       | ABC-C      | [problem.md](./ex02-memoized-climb/problem.md)    | [solution.cpp](./ex02-memoized-climb/solution.cpp)    | [solution_guide.md](./ex02-memoized-climb/solution_guide.md)    |
| **ex03** | バックトラッキング（部分和列挙） | ABC-C      | [problem.md](./ex03-subset-sum/problem.md)        | [solution.cpp](./ex03-subset-sum/solution.cpp)        | [solution_guide.md](./ex03-subset-sum/solution_guide.md)        |
| **ex04** | グリッドDFS（島の個数カウント）  | ABC-C      | [problem.md](./ex04-grid-island-count/problem.md) | [solution.cpp](./ex04-grid-island-count/solution.cpp) | [solution_guide.md](./ex04-grid-island-count/solution_guide.md) |
| **ex05** | 木上のDFS（部分木サイズ集計）    | ABC-D      | [problem.md](./ex05-tree-subtree-size/problem.md) | [solution.cpp](./ex05-tree-subtree-size/solution.cpp) | [solution_guide.md](./ex05-tree-subtree-size/solution_guide.md) |

---

[← コース目次に戻る](../README.md) | [演習1: 桁和とユークリッドの互除法へ →](./ex01-gcd-and-digit-sum/problem.md)
