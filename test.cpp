#include <iostream>
#include <vector>
#include <string>
#include <cctype> // For isdigit

using namespace std;

// Returns true if the cell is considered a barrier (wall or outpost).
bool isBarrier(char cell) {
    return (cell == '#' || isdigit(cell));
}

// Label contiguous empty segments in each row.
// Outposts and walls act as segment boundaries.
void labelRowSegments(const vector<string>& board, vector<vector<int>>& rowLabels) {
    int n = board.size();
    int m = board[0].size();
    int label = 0;
    for (int i = 0; i < n; i++) {
        int j = 0;
        while (j < m) {
            // Start a new segment if we hit an empty cell.
            if (board[i][j] == '.') {
                label++;  // New label for this contiguous segment.
                int k = j;
                // Fill all contiguous empty cells until we hit a barrier.
                while (k < m && board[i][k] == '.') {
                    rowLabels[i][k] = label;
                    k++;
                }
                j = k; // Continue after the segment.
            } else {
                // Skip barriers.
                j++;
            }
        }
    }
}

// Label contiguous empty segments in each column.
// Outposts and walls act as segment boundaries.
void labelColumnSegments(const vector<string>& board, vector<vector<int>>& colLabels) {
    int n = board.size();
    int m = board[0].size();
    int label = 0;
    for (int j = 0; j < m; j++) {
        int i = 0;
        while (i < n) {
            // Start a new segment if we hit an empty cell.
            if (board[i][j] == '.') {
                label++;  // New label for this contiguous segment.
                int k = i;
                // Fill all contiguous empty cells until we hit a barrier.
                while (k < n && board[k][j] == '.') {
                    colLabels[k][j] = label;
                    k++;
                }
                i = k; // Continue after the segment.
            } else {
                // Skip barriers.
                i++;
            }
        }
    }
}

int main() {
    // Example board:
    //   '.' -> empty cell (where a rook can be placed)
    //   '#' -> wall
    //   '0'-'4' -> outpost (acts like a wall and requires an exact number of adjacent rooks)
    vector<string> board = {
        "..#..",
        "..#..",
        ".#2..",
        "#...."
    };

    int n = board.size();
    int m = board[0].size();

    // Create label grids initialized with 0 (meaning no segment assigned).
    vector<vector<int>> rowLabels(n, vector<int>(m, 0));
    vector<vector<int>> colLabels(n, vector<int>(m, 0));

    // Label row and column segments.
    labelRowSegments(board, rowLabels);
    labelColumnSegments(board, colLabels);

    // Output the row segmentation labels.
    cout << "Row Segments:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Print barriers as they are.
            if (isBarrier(board[i][j]))
                cout << board[i][j] << " ";
            else
                cout << rowLabels[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nColumn Segments:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // Print barriers as they are.
            if (isBarrier(board[i][j]))
                cout << board[i][j] << " ";
            else
                cout << colLabels[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
