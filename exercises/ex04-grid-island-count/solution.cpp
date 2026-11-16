#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int h, w;
    if (!(std::cin >> h >> w)) {
        return 0;
    }

    std::vector<std::string> grid(h);
    for (int r = 0; r < h; ++r) {
        std::cin >> grid[r];
    }

    std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));
    constexpr int dr[4] = {-1, 1, 0, 0};
    constexpr int dc[4] = {0, 0, -1, 1};

    // マス (r, c) を含む連結な陸地をすべて訪問済みにし、その面積（マス数）を返すDFS
    auto dfs = [&](auto& self, int r, int c) -> int {
        visited[r][c] = true;
        int area = 1; // 現在のマス自身の分 (1マス)

        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr[dir];
            int nc = c + dc[dir];

            // 1. 範囲外チェック
            if (nr < 0 || nr >= h || nc < 0 || nc >= w) continue;
            // 2. 海 '.' ならスキップ
            if (grid[nr][nc] != '#') continue;
            // 3. 訪問済みならスキップ
            if (visited[nr][nc]) continue;

            // 隣接する未訪問の陸地へ潜り、その先の面積を合算する
            area += self(self, nr, nc);
        }

        return area;
    };

    int island_count = 0;
    int max_area = 0;

    for (int r = 0; r < h; ++r) {
        for (int c = 0; c < w; ++c) {
            if (grid[r][c] == '#' && !visited[r][c]) {
                ++island_count;
                int current_area = dfs(dfs, r, c);
                max_area = std::max(max_area, current_area);
            }
        }
    }

    std::cout << island_count << " " << max_area << "\n";

    return 0;
}
