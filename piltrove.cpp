#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <queue>


using namespace std;

vector<int> v;

int nl, nc, turrets;
bool notPossible;


void insertBloker(int x, int y, vector<vector<tuple<int, int, bool>>> &noGoLine, vector<vector<tuple<int, int, bool>>> &noGoCol){
    vector <tuple<int, int, bool>> &col = noGoCol[x];
    vector <tuple<int, int, bool>> &line = noGoLine[y];
    if (col.empty()) {
        col.push_back(make_tuple(0, y, false));
        col.push_back(make_tuple(y, nl, false));
    }else{
        int i= 0;
        int part = y/ nc-1/col.size();
        while (i<col.size()) {//i = (i+1)%col.size()) cicles forward 
            if(y > get<0>(col[i]) && y < get<1>(col[i])){
                col.insert(col.begin() + i+1 , make_tuple(y,get<1>(col[i]), false));
                get<1>(col[i]) = y;
                break;
            }else if(y == get<0>(col[i]) || y == get<1>(col[i])) break;
            else if(y > get<1>(col[i])) i = (i+1)%col.size(); //forward
            else i = (i-1)%col.size(); //back
        }
    }
    if (noGoLine.empty()) {
        line.push_back(make_tuple(0, x, false));
        line.push_back(make_tuple(x, nc, false));
    }else{
        int i= 0;
        int part = x/ nl-1/line.size();
        while (i < line.size()) {//i = (i+1)%line.size() cicles forward 
            if(x > get<0>(line[i]) && x < get<1>(line[i])){
                line.insert(line.begin() + i+1 , make_tuple(x, get<1>(line[i]), false));
                get<1>(line[i]) = x;
                break;
            }else if(x == get<0>(line[i]) || x == get<1>(line[i])) break;
            else if(x > get<1>(line[i])) i = (i+1)%line.size();
            else i = (i-1)%line.size();
        }
    }
}

bool isBarrier(char cell) {
    return (cell == '#' || isdigit(cell));
}

// Label contiguous empty segments in each line.
// Outposts and walls act as segment boundaries.
void labelLineSegments(const vector<string>& board, vector<vector<int>>& lineLabels) {
    int label = 0;
    for (int i = 0; i < nl; i++) {
        int j = 0;
        while (j < nc) {
            // Start a new segment if we hit an empty cell.
            if (board[i][j] == '.') {
                label++;  // New label for this contiguous segment.
                int k = j;
                // Fill all contiguous empty cells until we hit a barrier.
                while (k < nc && board[i][k] == '.') {
                    lineLabels[i][k] = label;
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
    int label = 0;
    for (int j = 0; j < nc; j++) {
        int i = 0;
        while (i < nl) {
            // Start a new segment if we hit an empty cell.
            if (board[i][j] == '.') {
                label++;  // New label for this contiguous segment.
                int k = i;
                // Fill all contiguous empty cells until we hit a barrier.
                while (k < nl && board[k][j] == '.') {
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
    int n_test_cases;
    cin >> n_test_cases;
    for (int i = 0; i < n_test_cases; i++) {

        turrets = 0;
        cin >> nl;
        cin >> nc;

        //vector<vector<tuple<int, int, bool>>> noGoLine; //y 
        //vector<vector<tuple<int, int, bool>>> noGoCol;  //x
        //noGoLine.resize(nl);
        //noGoCol.resize(nc);
        string line;
        //queue<tuple<int, int , char>> startingTurrets;
        vector<string> board;

        //ler a grid e para cada objeto fazer pré processamento
        for (int y = 0; y < nl; y++) {
            getline(cin,line);
            board.push_back(line);
            /*for(int x; x < nc; x++ ){
                /*if (isdigit(line[x]) ){
                    //add tower/bloker
                    insertBloker(x, y, noGoLine, noGoCol);
                    //add n turrets around the tower to the queue
                    startingTurrets.push(make_tuple(x,y, line[x]));
                }
                if (line[x] =='#') insertBloker(x, y, noGoLine, noGoCol);
            }
            /*while (!startingTurrets.empty()){
                auto ts = startingTurrets.front();
                if((notPossible = !insertTurrets(get<0>(ts), get<1>(ts), get<2>(ts), noGoLine, noGoCol))) {
                    cout << "noxus will rise!\n";
                    break;
                }
                
            }
            
            if (notPossible) continue;*/

            vector<vector<int>> rowLabels(nl, vector<int>(nc, 0));
            vector<vector<int>> colLabels(nl, vector<int>(nc, 0));
            labelRowSegments(board, rowLabels);
            labelColumnSegments(board, colLabels);
        }


    }

    //debugging



    return 0;
}
