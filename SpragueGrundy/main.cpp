#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <string>

int Mex(const std::vector<int>& calculatedGame) {
    std::unordered_set<int> mexSet(calculatedGame.begin(), calculatedGame.end());
    for (auto i = 0; ; ++i) {
        if (!mexSet.count(i)) {
            return i;
        }
    }
}

std::set<int> ComputeAnswer(int n, const std::vector<int>& calculatedGame) {
    std::set<int> answer;
    if (calculatedGame[n] == 0) {
        return answer;
    }
    if (calculatedGame[n - 1] == 0) {
        answer.insert(1);
        answer.insert(n);
    }
    if (n >= 3 && calculatedGame[n - 2] == 0) {
        answer.insert(2);
        answer.insert(n - 1);
    }
    for (int i = 3; i <= n - 2; ++i) {
        if ((calculatedGame[i - 1] ^ calculatedGame[n - i]) == 0) {
            answer.insert(i);
        }
    }
    
    return answer;
}

std::set<int> GetWinningMoves(int n) {
    std::vector<int> SG(n + 1);
    SG[1] = 0;
    SG[2] = 1;
    SG[3] = 2;
    for (int i = 4; i <= n; ++i) {
        std::vector<int> currentValue;
        currentValue.push_back(SG[i - 1]);
        currentValue.push_back(SG[i - 2]);
        for (int j = 3; j <= i - 2; ++j) {
            currentValue.push_back(SG[j - 1] ^ SG[i - j]);
        }
        SG[i] = Mex(currentValue);
    }
    
    return ComputeAnswer(n, SG);
}


int main() {
    int n;
    std::cin >> n;
    std::set<int> answer = GetWinningMoves(n);
    if (answer.empty()) {
        std::cout << "Mueller" << std::endl;
    } else {
        std::cout << "Schtirlitz" << std::endl;
        for (auto element : answer) {
            std::cout << element << std::endl;
        }
    }
    return 0;
}
