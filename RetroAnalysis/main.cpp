#include <iostream>
#include <utility>
#include <vector>
#include <cassert>
#include <string>
#include <algorithm>

enum class CellType : char {
    EMPTY = '0',
    WALL = '1',
    ESCAPEE_CELL = '2',
    TERMINATOR_CELL = '3',
    EXIT = '4'
};

enum PlayerTurn {
    TERMINATOR = 0,
    ESCAPEE = 1
};

enum GameState {
    WIN = 1,
    LOSE = -1,
    TIE = 0
};

class Coordinate {
public:
    Coordinate() : x(-1), y(-1) {};
    explicit Coordinate(int x, int y) : x(x), y(y) {};
    
    bool IsCorrect(PlayerTurn) const;
    int ComputeState() const;
    
    int GetX() const;
    int GetY() const;

    constexpr static int GetXSize() {
        return XSize;
    }
    constexpr static int GetYSize() {
        return YSize;
    }
    
private:
    int x;
    int y;
    
    constexpr static int XSize = 8;
    constexpr static int YSize = 8;
};

int Coordinate::ComputeState() const {
    return (y * XSize + x);
}

int Coordinate::GetX() const {
    return x;
}

int Coordinate::GetY() const {
    return y;
}

bool Coordinate::IsCorrect(PlayerTurn turn) const {
    return (0 <= x && x < XSize && 0 <= y && y < YSize + (turn == PlayerTurn::ESCAPEE) ? 1 : 0);
}

Coordinate operator+(const Coordinate& lhs, const Coordinate& rhs) {
    return Coordinate(lhs.GetX() + rhs.GetX(), lhs.GetY()+ rhs.GetY());
}

Coordinate operator-(const Coordinate& lhs, const Coordinate& rhs) {
    return Coordinate(lhs.GetX() - rhs.GetX(), lhs.GetY() - rhs.GetY());
}

bool operator==(const Coordinate& lhs, const Coordinate& rhs) {
    return lhs.GetX() == rhs.GetX() && lhs.GetY() == rhs.GetY();
}


class Vertex {
public:
    explicit Vertex(const Coordinate& terminator,
                    const Coordinate& escapee,
                    int turn) :
                    terminator(terminator),
                    escapee(escapee),
                    turn(turn) {}
    
    Coordinate terminator;
    Coordinate escapee;
    int turn;
};

using vertexArray3D = std::vector< std::vector< std::vector< Vertex>>>;

class GameGraph {
public:
    explicit GameGraph(std::vector<std::vector<CellType>> field);
    
    int DefineWinner();
    
    vertexArray3D& edges(int turn);
    std::vector<std::vector<GameState>>& gameStatus(int turn);
    std::vector<std::vector<bool>>& used(int turn);
    std::vector<std::vector<int>>& degree(int turn);
    
private:
    vertexArray3D edgesTerminator;
    vertexArray3D edgesEscapee;
    
    int escapeeNum;
    int terminatorNum;
    
    std::vector<std::vector<GameState>> gameStatusTerminator;
    std::vector<std::vector<GameState>> gameStatusEscapee;
    
    std::vector<std::vector<bool>> usedTerminator;
    std::vector<std::vector<bool>> usedEscapee;
    
    std::vector<std::vector<int>> degreeTerminator;
    std::vector<std::vector<int>> degreeEscapee;
    
    Coordinate terminatorStart;
    Coordinate escapeeStart;
    std::vector<std::vector<CellType>> field;
    
    
    std::pair<bool, bool> CheckWin(Coordinate coord, std::vector<bool>& been) const;
    
    bool CanBeShot(const Coordinate& terminator, const Coordinate& escapee) const;
    bool CanBeShotX(const Coordinate& terminator, const Coordinate& escapee) const;
    bool CanBeShotY(const Coordinate& terminator, const Coordinate& escapee) const;
    bool CanBeShotMainDiagonal(const Coordinate& terminator, const Coordinate& escapee) const;
    bool CanBeShotSideDiagonal(const Coordinate& terminator, const Coordinate& escapee) const;
    
    void InitializeGameStatus();
    void InitializeVectors();
    void DefineStartPositions(int, int, const Coordinate&, const Coordinate&);
    void ComputeEdges();
    
    void DFS(int terminatorIndex, int escapeeIndex, int turn);
};

std::pair<bool, bool> GameGraph::CheckWin(Coordinate coord, std::vector<bool>& been) const {
    
    if (field[coord.GetY()][coord.GetX()] == (CellType::WALL)) {
        return {false, false};
    }
    
    if (terminatorStart == coord) {
        return {true, false};
    }
    
    std::pair<bool, bool> result = {false, false};
    
    if (coord.GetY() == Coordinate::GetYSize()) {
        result.second = true;
    }
    
    been[coord.ComputeState()] = true;
    
    for (int d_x = -1; d_x <= 1; d_x++) {
        for (int d_y = -1; d_y <= 1; d_y++) {
            if (d_x == 0 && d_y == 0) {
                continue;
            }
            
            Coordinate new_coord = coord + Coordinate(d_x, d_y);
            
            if (new_coord.IsCorrect(ESCAPEE) && been[new_coord.ComputeState()] == false) {
                std::pair<bool, bool> curr_result = CheckWin(new_coord, been);
                
                result.first |= curr_result.first;
                result.second |= curr_result.second;
            }
        }
    }
    
    return result;
}

bool GameGraph::CanBeShot(const Coordinate& terminator, const Coordinate& escapee) const {
    if (field[escapee.GetY()][escapee.GetX()] == (CellType::EXIT)) {
        return false;
    } else if (terminator.GetX() == escapee.GetX()) {
        return CanBeShotX(terminator, escapee);
    } else if (terminator.GetY() == escapee.GetY()) {
        return CanBeShotY(terminator, escapee);
    } else if (terminator.GetX() - terminator.GetY() == escapee.GetX() - escapee.GetY()) {
        return CanBeShotMainDiagonal(terminator, escapee);
    } else if (terminator.GetX() + terminator.GetY() == escapee.GetX() + escapee.GetY()) {
        return CanBeShotSideDiagonal(terminator, escapee);
    }
    return false;
}


bool GameGraph::CanBeShotX(const Coordinate& terminator, const Coordinate& escapee) const {
    int x = terminator.GetX();
    int max_y = std::max(terminator.GetY(), escapee.GetY());
    for (int y = std::min(terminator.GetY(), escapee.GetY()); y <= max_y; ++y) {
        if (field[y][x] == (CellType::WALL)) {
            return false;
        }
    }
    return true;
}


bool GameGraph::CanBeShotY(const Coordinate& terminator, const Coordinate& escapee) const {
    int y = terminator.GetY();
    int max_x = std::max(terminator.GetX(), escapee.GetX());
    for (int x = std::min(terminator.GetX(), escapee.GetX()); x <= max_x; ++x) {
        if (field[y][x] == (CellType::WALL)) {
            return false;
        }
    }
    return true;
}


bool GameGraph::CanBeShotMainDiagonal(const Coordinate& terminator, const Coordinate& escapee) const {
    int diff = terminator.GetX() - terminator.GetY();
    int max_x = std::max(terminator.GetX(), escapee.GetX());
    for (int x = std::min(terminator.GetX(), escapee.GetX()); x <= max_x; ++x) {
        if (field[x - diff][x] == (CellType::WALL)) {
            return false;
        }
    }
    return true;
}


bool GameGraph::CanBeShotSideDiagonal(const Coordinate& terminator, const Coordinate& escapee) const {
    int sum = terminator.GetX() + terminator.GetY();
    int max_x = std::max(terminator.GetX(), escapee.GetX());
    for (int x = std::min(terminator.GetX(), escapee.GetX()); x <= max_x; ++x) {
        if (field[sum - x][x] == (CellType::WALL)) {
            return false;
        }
    }
    return true;
}

vertexArray3D& GameGraph::edges(int turn) {
    return (turn == 0) ? edgesTerminator : edgesEscapee;
}
std::vector<std::vector<GameState>>& GameGraph::gameStatus(int turn) {
    return (turn == 0) ? gameStatusTerminator : gameStatusEscapee;
}
std::vector<std::vector<bool>>& GameGraph::used(int turn) {
    return (turn == 0) ? usedTerminator : usedEscapee;
}
std::vector<std::vector<int>>& GameGraph::degree(int turn) {
    return (turn == 0) ? degreeTerminator : degreeEscapee;
}

void GameGraph::DefineStartPositions(int terminatorIndex, int escapeeIndex, const Coordinate& terminatorCoord, const Coordinate& escapeeCoord) {
    for (int turn = TERMINATOR; turn <= ESCAPEE; ++turn) {

        if (gameStatus(turn)[terminatorIndex][escapeeIndex] != TIE) {
            continue;
        }

        for (int d_x = -1; d_x <= 1; d_x++) {
            for (int d_y = -1; d_y <= 1; d_y++) {

                if (!d_x && !d_y) {
                    continue;
                }

                Coordinate player_coord = (turn == ESCAPEE ? escapeeCoord : terminatorCoord) + Coordinate(d_x, d_y);
                if (player_coord.IsCorrect(turn == ESCAPEE ? ESCAPEE : TERMINATOR) && field[player_coord.GetY()][player_coord.GetX()] !=
                    (CellType::WALL)) {
                    
                    if (turn == ESCAPEE) {
                        edgesTerminator[terminatorIndex][player_coord.ComputeState()].emplace_back(terminatorCoord, escapeeCoord, turn);
                    } else {
                        edgesEscapee[player_coord.ComputeState()][escapeeIndex].emplace_back(terminatorCoord, escapeeCoord, turn);
                    }
                    ++degree(turn)[terminatorIndex][escapeeIndex];
                }
            }
        }

    }
}

void GameGraph::InitializeGameStatus() {
    for (int terminatorIndex = 0; terminatorIndex < terminatorNum; ++terminatorIndex) {

        Coordinate terminatorCoord(terminatorIndex % Coordinate::GetXSize(), terminatorIndex / Coordinate::GetXSize());
        if (field[terminatorCoord.GetY()][terminatorCoord.GetX()] ==
            (CellType::WALL)) {
            continue;
        }
        if (terminatorStart.GetX() == -1 && field[terminatorCoord.GetY()][terminatorCoord.GetX()]
            == (CellType::TERMINATOR_CELL)) {
            terminatorStart = terminatorCoord;
        }

        for (int escapeeIndex = 0; escapeeIndex < escapeeNum; ++escapeeIndex) {

            Coordinate escapeeCoord(escapeeIndex % Coordinate::GetXSize(), escapeeIndex / Coordinate::GetXSize());
            if (field[escapeeCoord.GetY()][escapeeCoord.GetX()] ==
                (CellType::WALL)) {
                continue;
            }
            if (escapeeStart.GetX() == -1 && field[escapeeCoord.GetY()][escapeeCoord.GetX()] ==
                (CellType::ESCAPEE_CELL)) {
                escapeeStart = escapeeCoord;
            }

            if (CanBeShot(terminatorCoord, escapeeCoord)) {
                gameStatusTerminator[terminatorIndex][escapeeIndex] = WIN;
                gameStatusEscapee[terminatorIndex][escapeeIndex] = LOSE;
            } else if (field[escapeeCoord.GetY()][escapeeCoord.GetX()] ==
                       (CellType::EXIT)) {
                gameStatusTerminator[terminatorIndex][escapeeIndex] = LOSE;
                gameStatusEscapee[terminatorIndex][escapeeIndex] = WIN;
            }
            
            DefineStartPositions(terminatorIndex, escapeeIndex, terminatorCoord, escapeeCoord);

        }
        
    }

}

void GameGraph::InitializeVectors() {
    for (int i = TERMINATOR; i <= ESCAPEE; ++i) {
        edges(i).resize(terminatorNum);
        gameStatus(i).resize(terminatorNum);
        used(i).resize(terminatorNum);
        degree(i).resize(terminatorNum);
        for (int j = 0; j < terminatorNum; ++j) {
            edges(i)[j].resize(escapeeNum);
            gameStatus(i)[j] = std::vector<GameState>(escapeeNum, TIE);
            used(i)[j] = std::vector<bool>(escapeeNum, false);
            degree(i)[j].resize(escapeeNum);
        }
    }
    
}

void GameGraph::ComputeEdges() {
    for (int terminatorIndex = 0; terminatorIndex < terminatorNum; ++terminatorIndex) {
        for (int escapeeIndex = 0; escapeeIndex < escapeeNum; ++escapeeIndex) {
            for (int turn = TERMINATOR; turn <= ESCAPEE; ++turn) {
                if (!used(turn)[terminatorIndex][escapeeIndex] && gameStatus(turn)[terminatorIndex][escapeeIndex] != TIE) {
                    DFS(terminatorIndex, escapeeIndex, turn);
                    if (gameStatus(ESCAPEE)[terminatorStart.ComputeState()][escapeeStart.ComputeState()] != TIE) {
                        return;
                    }
                }
            }
        }
    }
}


GameGraph::GameGraph(std::vector<std::vector<CellType>> field) : field(std::move(field)) {

    terminatorNum = Coordinate::GetXSize() * Coordinate::GetYSize();
    escapeeNum = Coordinate::GetXSize() * (Coordinate::GetYSize() + 1);

    this->field.emplace_back(Coordinate::GetXSize(), CellType::EXIT);

    InitializeVectors();

    InitializeGameStatus();
    
    if (gameStatus(ESCAPEE)[terminatorStart.ComputeState()][escapeeStart.ComputeState()] != TIE) {
        return;
    }

    ComputeEdges();
    
}


void GameGraph::DFS(int terminatorIndex, int escapeeIndex, int turn) {

    used(turn)[terminatorIndex][escapeeIndex] = true;
    for (const auto& i : edges(turn)[terminatorIndex][escapeeIndex]) {
        if (!used(i.turn)[i.terminator.ComputeState()][i.escapee.ComputeState()]) {

            if (gameStatus(turn)[terminatorIndex][escapeeIndex] == LOSE) {
                gameStatus(i.turn)[i.terminator.ComputeState()][i.escapee.ComputeState()] = WIN;
            } else if (--degree(i.turn)[i.terminator.ComputeState()][i.escapee.ComputeState()] <= 0) {
                gameStatus(i.turn)[i.terminator.ComputeState()][i.escapee.ComputeState()] = LOSE;
            } else {
                continue;
            }

            DFS(i.terminator.ComputeState(), i.escapee.ComputeState(), i.turn);
        }
    }

}

int GameGraph::DefineWinner() {
    std::vector<bool> been(Coordinate::GetXSize() * (Coordinate::GetYSize() + 1), false);
    
    std::pair<bool, bool> isEscapeeWin = CheckWin(escapeeStart, been);
    if (isEscapeeWin.second == false) {
        return -1;
    }
    if (!isEscapeeWin.first) {
        return 1;
    }
    
    return gameStatus(ESCAPEE)[terminatorStart.ComputeState()][escapeeStart.ComputeState()];
}

CellType DefineCellType(char cell) {
    CellType answer;
    switch (cell) {
        case '0':
            answer = CellType::EMPTY;
            break;
        case '1':
            answer = CellType::WALL;
            break;
        case '2':
            answer = CellType::ESCAPEE_CELL;
            break;
        case '3':
            answer = CellType::TERMINATOR_CELL;
            break;
        default:
            answer = CellType::EXIT;
            break;
    }
    return answer;
}

int main() {

    std::vector<std::vector<CellType>> field;
    field.resize(Coordinate::GetYSize());
    for (int i = 0; i < Coordinate::GetYSize(); ++i) {
        char cell;
        field[i].resize(Coordinate::GetXSize());
        for (int j  = 0; j < Coordinate::GetXSize(); ++j) {
            std::cin >> cell;
            field[i][j] = (DefineCellType(cell));
            }
    }
    
    GameGraph graph(field);
    std::cout << graph.DefineWinner() << std::endl;
    return 0;
}
