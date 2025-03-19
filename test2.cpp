#include <iostream>
#include <vector>
#include <string>
#include <cctype>   // for isdigit
using namespace std;

// Global board dimensions and data
int n, m;
vector<string> board;

// These will store the segment labels for empty cells.
// A label of 0 means no segment was assigned.
vector<vector<int>> rowLabels, colLabels;
int rowSegmentCount = 0, colSegmentCount = 0;

// A candidate is a coordinate (i,j) where board[i][j] == '.'
vector<pair<int,int>> candidates;

// The solution grid (initially a copy of board) where we place rooks ('R').
vector<vector<char>> solution;

// For enforcing non-attacking rule:
// When a rook is placed in an empty cell, mark its row segment and column segment as "used".
vector<bool> rowUsed, colUsed;

// For outpost constraint: track how many rooks are adjacent to each cell (only matters for outpost cells).
vector<vector<int>> outpostCount;

// -------------------- Helper Functions --------------------

// Returns true if the cell is considered a barrier (wall '#' or outpost digit)
bool isBarrier(char cell) {
    return (cell == '#' || isdigit(cell));
}

// -------------------- Segmentation Functions --------------------

// Label contiguous empty segments in each row. Walls and outposts break segments.
void labelRowSegments() {
    rowLabels.assign(n, vector<int>(m, 0));
    for (int i = 0; i < n; i++) {
        int j = 0;
        while (j < m) {
            if (board[i][j] == '.') {
                rowSegmentCount++;
                int k = j;
                while (k < m && board[i][k] == '.') {
                    rowLabels[i][k] = rowSegmentCount;
                    k++;
                }
                j = k;
            } else {
                j++;
            }
        }
    }
}

// Label contiguous empty segments in each column.
void labelColumnSegments() {
    colLabels.assign(n, vector<int>(m, 0));
    for (int j = 0; j < m; j++) {
        int i = 0;
        while (i < n) {
            if (board[i][j] == '.') {
                colSegmentCount++;
                int k = i;
                while (k < n && board[k][j] == '.') {
                    colLabels[k][j] = colSegmentCount;
                    k++;
                }
                i = k;
            } else {
                i++;
            }
        }
    }
}

// -------------------- Coverage and Outpost Check --------------------

// Checks if an empty cell (i,j) is covered by a rook in its row or column (stopped by barriers).
bool isCovered(int i, int j) {
    // Look left in row
    for (int k = j - 1; k >= 0; k--) {
        if (isBarrier(board[i][k])) break;
        if (solution[i][k] == 'R') return true;
    }
    // Look right in row
    for (int k = j + 1; k < m; k++) {
        if (isBarrier(board[i][k])) break;
        if (solution[i][k] == 'R') return true;
    }
    // Look up in column
    for (int k = i - 1; k >= 0; k--) {
        if (isBarrier(board[k][j])) break;
        if (solution[k][j] == 'R') return true;
    }
    // Look down in column
    for (int k = i + 1; k < n; k++) {
        if (isBarrier(board[k][j])) break;
        if (solution[k][j] == 'R') return true;
    }
    return false;
}

// For an outpost cell at (i,j), count the rooks placed in its four adjacent cells.
int countAdjacentRooks(int i, int j) {
    int cnt = 0;
    int di[4] = {-1, 1, 0, 0};
    int dj[4] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        int ni = i + di[d], nj = j + dj[d];
        if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
            if (solution[ni][nj] == 'R')
                cnt++;
        }
    }
    return cnt;
}

// Check that all outpost constraints are satisfied (exact count).
bool checkOutposts() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (isdigit(board[i][j])) {
                int required = board[i][j] - '0';
                int count = countAdjacentRooks(i, j);
                if (count != required)
                    return false;
            }
        }
    }
    return true;
}

// Check that every empty cell is covered by at least one rook.
bool checkCoverage() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (board[i][j] == '.') {
                if (!isCovered(i, j))
                    return false;
            }
        }
    }
    return true;
}

// -------------------- Backtracking Solver --------------------

// Recursive function that tries to decide on rook placements on candidate empty cells.
bool solve(int index) {
    // Base case: processed all candidate positions.
    if (index == candidates.size()) {
        if (checkOutposts() && checkCoverage())
            return true;
        return false;
    }

    // Get current candidate cell.
    int i = candidates[index].first;
    int j = candidates[index].second;

    // Option 1: Do not place a rook at this candidate.
    if (solve(index + 1))
        return true;

    // Option 2: Try placing a rook if the non-attacking condition holds.
    int rseg = rowLabels[i][j];
    int cseg = colLabels[i][j];
    if (!rowUsed[rseg] && !colUsed[cseg]) {
        // Before placing the rook, record changes for outpost counts.
        vector<pair<int,int>> changedOutposts;
        int di[4] = {-1, 1, 0, 0};
        int dj[4] = {0, 0, -1, 1};
        bool valid = true;
        for (int d = 0; d < 4; d++) {
            int ni = i + di[d], nj = j + dj[d];
            if (ni >= 0 && ni < n && nj >= 0 && nj < m && isdigit(board[ni][nj])) {
                outpostCount[ni][nj]++;
                changedOutposts.push_back({ni, nj});
                int required = board[ni][nj] - '0';
                if (outpostCount[ni][nj] > required) {
                    valid = false;
                    break;
                }
            }
        }
        if (!valid) {
            // Revert outpost count changes before skipping rook placement.
            for (auto &p : changedOutposts) {
                outpostCount[p.first][p.second]--;
            }
            // Skip placing the rook here.
            // (Using a label or simply continue to the next branch.)
        } else {
            // Place the rook.
            solution[i][j] = 'R';
            rowUsed[rseg] = true;
            colUsed[cseg] = true;

            if (solve(index + 1))
                return true;

            // Backtrack: remove the rook.
            solution[i][j] = '.';
            rowUsed[rseg] = false;
            colUsed[cseg] = false;
        }
        // Revert outpost count changes in any case.
        for (auto &p : changedOutposts) {
            outpostCount[p.first][p.second]--;
        }
    }
    return false;
}

// -------------------- Main --------------------
int main() {
    // Sample board:
    //   '.' -> empty cell (rook may be placed)
    //   '#' -> wall
    //   '0'-'4' -> outpost (acts like a wall; requires that exactly that many rooks be adjacent)
    board = {
        "..#..",
        "..#..",
        ".#2..",
        "#...."
    };

    n = board.size();
    m = board[0].size();

    // Initialize segmentation arrays.
    labelRowSegments();
    labelColumnSegments();

    // Build candidate list: all empty cells ('.') where a rook can potentially be placed.
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (board[i][j] == '.')
                candidates.push_back({i, j});

    // Initialize the solution grid from board.
    solution.resize(n, vector<char>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            solution[i][j] = board[i][j];

    // Initialize usage arrays for row and column segments.
    rowUsed.assign(rowSegmentCount + 1, false);
    colUsed.assign(colSegmentCount + 1, false);

    // Initialize outpost counts.
    outpostCount.assign(n, vector<int>(m, 0));

    // Start the backtracking search.
    if (solve(0)) {
        cout << "Solution found:\n";
        for (const auto &row : solution) {
            for (char c : row)
                cout << c;
            cout << endl;
        }
    } else {
        cout << "No solution found.\n";
    }

    return 0;
}
