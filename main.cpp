#include <algorithm>
#include <queue>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

const int N = 26;

std::vector<int64_t> stepOfTwo;

void fillV() {
    stepOfTwo.resize(65);
    stepOfTwo[0] = 1;
    for (int i = 1; i < 65; i++) {
        stepOfTwo[i] = stepOfTwo[i - 1] * 2;
    }
}


class Game {
private:
    std::unordered_map<std::string , int> mp;
    std::unordered_map<std::string, std::string> pred;
    std::string ConstructEdge(std::string position, int64_t hit) {
        position[1 + hit] = '0';
        position[1 + N + hit] = '0';
        int64_t level = 0;
        for (size_t i = 2 * N + 1; i < position.size(); i++) {
            level = level * 10 + (position[i] - '0');
        }
        if (level % N == 0) {
            for (size_t i = 1; i <= N; i++) {
                if (position[i] == '1') {
                    position[N + i] = '1';
                }
            }
        }
        std::string used, unused;
        used.resize(N, '0');
        unused.resize(N, '0');
        bool b = false;
        for (size_t i = 0; i < N; i++) {
            if (position[1 + i] == '1') {
                if (i > 0) {
                    used[i - 1] = '1';
                    b = true;
                }
                if (i < N - 1) {
                    used[i + 1] = '1';
                    b = true;
                }
            }
            if (position[1 + N + i] == '1') {
                if (i > 1) {
                    used[i - 2] = '1';
                    b = true;
                }
                if (i < N - 2) {
                    used[i + 2] = '1';
                    b = true;
                }
                if (i > 0) {
                    unused[i - 1] = '1';
                    b = true;
                }
                if (i < N - 1) {
                    unused[i + 1] = '1';
                    b = true;
                }
            }
        }
        std::string newused(N, '0');
        //std::string ans = used + unused;
        std::string ans = newused + unused; //Мне было лень полностью убирать часть с +2, поэтому я просто её вырезал))
        if (b) {
            ans = '1' + ans;
        } else {
            ans = '0' + ans;
        }
        ans += std::to_string((level + 1) % N);
        return ans;
    }
    std::vector<std::string> ConstructNode(std::string positions) {
        std::vector<std::string> ans;
        for (int64_t i = 0; i < N; i++) {
            if(positions[1 + i] == '1' || positions[1 + i + N] == '1') {
                ans.emplace_back(ConstructEdge(positions, i));
                if (mp.find(ans.back()) == mp.end()) {
                    mp[ans.back()] = i;
                }
                //mp[ans.back()] = i;
            }
        }
        return ans;
    }
    void printPos(std::string s) {
        for (int i = 1; i <= N; i++) {
            std::cout << s[i] << "";
        }
        std::cout << "--";
        for (int i = N + 1; i <= 2 * N; i++) {
            std::cout << s[i] << "";
        }
        std::cout << "-- Время: ";
        for (int i = 2 * N + 1; i < s.size(); i++) {
            std::cout << s[i];
        }
        std::cout << " -- Шахта для удара " << mp[s] <<  '\n';
    }
public:
    Game() {
        fillV();
    }
    void BFS() {
        std::queue<std::string> q;
        std::string start, s1, s2;
        std::unordered_map<std::string, int> ishere;
        s1.resize(N, '0');
        s2.resize(N, '1');
        start = '1' + s1 + s2;
        start += '0';
        q.push(start);
        pred[start] = "END";
        while (!q.empty()) {
            //printPos(q.front());
            std::vector<std::string> sons = ConstructNode(q.front());
            for (auto &son : sons) {
                if (son[0] == '0') {
                    pred[son] = q.front();
                    auto el  = son;
                    std::vector<std::string> way;
                    while (el != start) {
                        way.push_back(el);
                        el = pred[el];
                    }
                    std::reverse(way.begin(), way.end());
                    for (auto &el : way) {
                        printPos(el);
                    }
                    std::cout << "Length of way: " << way.size() << '\n';
                    std::cout << "Bush win!!!\n";
                    return;
                }
                if (ishere[son] == 0) {
                    ishere[son] = 1;
                    q.push(son);
                    pred[son] = q.front();
                }
            }
            q.pop();
        }
        std::cout << "Usama win!!!\n";
        return;
    }
};

int main() {
    Game g;
    //auto a = g.ConstructNode("10100002");
    g.BFS();
    return 0;
}