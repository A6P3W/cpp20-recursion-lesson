# 第2章: メモ化再帰と分割統治法

[← 第1章: 再帰関数の基本](./01-recursion-basics.md) | [目次に戻る](./README.md) | [第3章: バックトラッキング全探索へ →](./03-backtracking-search.md)

---

## 1. 素朴な再帰の落とし穴：計算量の爆発

フィボナッチ数列 $F_0 = 0, F_1 = 1, F_n = F_{n-1} + F_{n-2} \ (n \ge 2)$ を、第1章の方法で素朴に再帰実装するとどうなるでしょうか？

```cpp
long long fib_naive(int n) {
    if (n <= 1) return n;
    return fib_naive(n - 1) + fib_naive(n - 2);
}
```

この関数で `fib_naive(5)` を呼ぶと、呼び出しの木（再帰木）は次のようになります。

```text
                  fib(5)
                 /      \
            fib(4)      fib(3)
            /    \       /   \
        fib(3)  fib(2) fib(2) fib(1)
        /   \
    fib(2) fib(1)
```

`fib(3)` が2回、`fib(2)` が3回と、**まったく同じ引数の計算が何度も繰り返されている**ことがわかります。
$N = 45$ になると呼び出し回数は約20億回に達し、競技プログラミングの実行時間制限（通常 2.0 秒）を確実に超過（**TLE: Time Limit Exceeded**）します。計算量は $O(2^N)$ です。

---

## 2. メモ化再帰（Memoization）による $O(N)$ 化

「一度計算した `fib(k)` の結果を配列 `memo[k]` に保存しておき、2回目以降は再計算せずに保存した値をすぐ返す」という手法を**メモ化再帰（Memoized Recursion）**と呼びます。

これは動的計画法（DP: Dynamic Programming）をトップダウン（大きい問題から小さい問題へ）に解く手法そのものです。

### メモ化再帰の鉄板テンプレート

1. 未計算を表す特別な値（例: `-1`）で初期化した配列 `memo` を用意する。
2. ベースケースを判定する。
3. **`memo[n]` がすでに計算済み（`-1` ではない）なら、即座に `memo[n]` を返す。**
4. 計算した結果を `memo[n]` に代入してから返す。

### コード例：`memo_fib.cpp`

```cpp
#include <iostream>
#include <vector>

int main() {
    int n = 50;

    // memo[i]: fib(i) の計算結果を保存する配列（未計算は -1 で初期化）
    std::vector<long long> memo(n + 1, -1);

    auto fib = [&](auto& self, int k) -> long long {
        // 1. ベースケース
        if (k <= 1) {
            return k;
        }
        // 2. メモのチェック: すでに計算済みならそのまま返す
        if (memo[k] != -1) {
            return memo[k];
        }
        // 3. 計算結果をメモに記録しながら返す
        return memo[k] = self(self, k - 1) + self(self, k - 2);
    };

    std::cout << "F(" << n << ") = " << fib(fib, n) << "\n";
    return 0;
}
```

#### ビルド＆実行

```bash
g++ -std=c++20 -O2 memo_fib.cpp -o memo_fib
./memo_fib
# 出力: F(50) = 12586269025
```

### なぜ高速になるのか？

各 $k \ (0 \le k \le N)$ について、`self(self, k - 1) + self(self, k - 2)` の足し算が実行されるのは**最初の1回だけ**です。2回目以降は `if (memo[k] != -1)` で $O(1)$ で返るため、全体の計算量は $O(2^N)$ から **$O(N)$** へと劇的に改善します。

---

## 3. 分割統治法（Divide and Conquer）

再帰のもう一つの重要なパターンが、**「問題の規模を半分（$N / 2$）に分割して解く」分割統治法**です。

### 代表例：繰り返し二乗法（二分累乗 / Binary Exponentiation）

競技プログラミングでは「$a^n \bmod M$ を求めよ（ただし $n \le 10^{18}$）」という問題が頻出します。
`for` ループで $a$ を $n$ 回掛けると $O(n)$ かかり、$n = 10^{18}$ では絶対に終わりません。

しかし、指数法則を使うと $a^n$ を半分サイズの $a^{\lfloor n/2 \rfloor}$ から $O(1)$ で計算できます。

- **ベースケース**: $n = 0$ のとき、$a^0 = 1$
- **再帰ステップ**:
  まず $t = a^{\lfloor n/2 \rfloor} \bmod M$ を1回だけ再帰で求める。
  - $n$ が偶数のとき: $a^n = t \times t \pmod M$
  - $n$ が奇数のとき: $a^n = t \times t \times a \pmod M$

### コード例：`mod_pow.cpp`

```cpp
#include <iostream>

// a^n mod m を O(log n) で求める再帰関数
long long mod_pow(long long a, long long n, long long m) {
    // ベースケース: a^0 = 1
    if (n == 0) {
        return 1LL % m;
    }

    // 問題の規模を半分 (n / 2) に分割して1回だけ再帰呼び出し
    long long half = mod_pow(a, n / 2, m);
    long long result = (half * half) % m;

    // n が奇数なら、あと1回 a を掛ける
    if (n % 2 == 1) {
        result = (result * (a % m)) % m;
    }
    return result;
}

int main() {
    long long a = 2;
    long long n = 1000000000000000000LL; // 10^18
    long long mod = 998244353LL;

    std::cout << a << "^" << n << " mod " << mod << " = "
              << mod_pow(a, n, mod) << "\n";
    return 0;
}
```

#### 注意点：`mod_pow(a, n / 2, m) * mod_pow(a, n / 2, m)` と書いてはいけない！

もし変数 `half` に受けずに `mod_pow(a, n / 2, m) * mod_pow(a, n / 2, m)` と2回呼び出してしまうと、1ステップで呼び出しが2倍に増えるため、せっかく半分にしたのに計算量が $O(2^{\log_2 n}) = O(n)$ に戻ってしまいます。**「1回だけ呼んで変数 `half` に保存し、それを2乗する」**のが分割統治の鉄則です。

---

## 4. 章末チェックと演習課題

### この章のまとめ

- 同じ引数（状態）が何度も呼び出される再帰は、**メモ化配列 `memo`** を使うことで劇的に高速化できる。
- メモ配列の初期値には、答えとして絶対に登場しない値（`-1` など）を選ぶ。
- $N$ を $N/2$ に縮小する再帰は $O(\log N)$ で動作し、$N = 10^{18}$ のような巨大な入力にも対応できる。

### ハンズオン演習へ進もう

メモ化再帰と余り（MOD）計算を組み合わせた典型問題に挑戦しましょう！

👉 **[演習2: 壊れた階段の上り方 (ex02-memoized-climb)](./exercises/ex02-memoized-climb/problem.md)**

---

[← 第1章: 再帰関数の基本](./01-recursion-basics.md) | [目次に戻る](./README.md) | [第3章: バックトラッキング全探索へ →](./03-backtracking-search.md)
