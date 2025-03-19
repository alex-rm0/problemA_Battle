#include <iostream>
#include <vector>
#include <string>
using namespace std;


int n, m, turrets;
vector<string> board;
vector<string> solution;
vector<vector<tuple<int, int, bool>>> noGoLine; //y 
vector<vector<tuple<int, int, bool>>> noGoCol;  //x

vector<vector<int>> outpostCount; 
bool solved = false;



void insertBloker(int x, int y, vector<vector<tuple<int, int, bool>>> &noGoLine, vector<vector<tuple<int, int, bool>>> &noGoCol){
    vector <tuple<int, int, bool>> &col = noGoCol[x];
    vector <tuple<int, int, bool>> &line = noGoLine[y];
    if (col.empty()) {
        col.push_back(make_tuple(0, y, false));
        col.push_back(make_tuple(y, n, false));
    }else{
        int i= 0;
        int part = y/ m-1/col.size();
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
    if (line.empty()) {
        line.push_back(make_tuple(0, x, false));
        line.push_back(make_tuple(x, m, false));
    }else{
        int i= 0;
        int part = x/ n-1/line.size();
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

bool canPlaceTurret(int x, int y) {
    vector <tuple<int, int, bool>> &col = noGoCol[x];
    vector <tuple<int, int, bool>> &line = noGoLine[y];
    if (col.empty()) {
        if (noGoLine.empty()) {
            col.push_back(make_tuple(0, n, true));
            line.push_back(make_tuple(0, m, true));
            return true;
        }else{
            int i= 0;
            int part = x/ n-1/line.size();
            while (i < line.size()) {//i = (i+1)%line.size() cicles forward 
                if(x > get<0>(line[i]) && x < get<1>(line[i])){
                    col.push_back(make_tuple(0, n, true));
                    get<2>(line[i]) = true;
                    
                }else if(x == get<0>(line[i]) || x == get<1>(line[i])) return false;
                else if(x > get<1>(line[i])) i = (i+1)%line.size();
                else i = (i-1)%line.size();
            }
        }


    }else{
        int i= 0;
        int part = y/ m-1/col.size();
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
    

    }
    return true;
}

void updateOutpostCount(int i, int j, int delta) {
    int di[4] = {-1, 1, 0, 0};
    int dj[4] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        int ni = i + di[d], nj = j + dj[d];
        if (ni >= 0 && ni < n && nj >= 0 && nj < m) {
            if (isdigit(board[ni][nj])) {
                outpostCount[ni][nj] += delta;
            }
        }
    }
}

bool validLocalOutposts() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (isdigit(board[i][j])) {
                int required = board[i][j] - '0';
                if (outpostCount[i][j] > required)
                    return false;
            }
        }
    }
    return true;
}

bool checkAllOutposts() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (isdigit(board[i][j])) {
                int required = board[i][j] - '0';
                if (outpostCount[i][j] != required)
                    return false;
            }
        }
    }
    return true;
}

bool isCovered(int i, int j) {
    //left
    for (int k = j - 1; k >= 0; k--) {
        if (board[i][k] == '#' || isdigit(board[i][k]))
            break;
        if (solution[i][k] == 'R')
            return true;
    }
    //right
    for (int k = j + 1; k < m; k++) {
        if (board[i][k] == '#' || isdigit(board[i][k]))
            break;
        if (solution[i][k] == 'R')
            return true;
    }
    //up
    for (int k = i - 1; k >= 0; k--) {
        if (board[k][j] == '#' || isdigit(board[k][j]))
            break;
        if (solution[k][j] == 'R')
            return true;
    }
    //down
    for (int k = i + 1; k < n; k++) {
        if (board[k][j] == '#' || isdigit(board[k][j]))
            break;
        if (solution[k][j] == 'R')
            return true;
    }
    return false;
}

bool checkCoverage() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (board[i][j] == '.' && solution[i][j] != 'R') {
                if (!isCovered(i, j))
                    return false;
            }
        }
    }
    return true;
}

void backtrack(int idx) {
    if (solved) return;
    if (idx == n * m) {
        //check that every outpost is satisfied and every empty cell is covered
        if (checkAllOutposts() && checkCoverage())
            solved = true;
        return;
    }
    
    int i = idx / m;
    int j = idx % m;
    
    if (board[i][j] != '.') {
        backtrack(idx + 1);
        return;
    }
    
    //skip option
    backtrack(idx + 1);
    if (solved) return;
    
    //try to place a Turret
    if (canPlaceTurret(i, j)) {
        solution[i][j] = 'R';
        turrets++;
        updateOutpostCount(i, j, 1);
        
        //only continue if outpost are still valid.
        if (validLocalOutposts()) {
            backtrack(idx + 1);
        }
        
        if (solved) return;
        
        //backtrack remove the Turret.
        updateOutpostCount(i, j, -1);
        solution[i][j] = '.';
        turrets--;
    }
}



int main() {
    string line;
    int n_test_cases;
    cin >> n_test_cases;
    for (int i = 0; i < n_test_cases; i++) {

        turrets = 0;
        cin >> n;
        cin >> m;
        getline(cin,line);

        noGoLine.resize(n);
        noGoCol.resize(m);

        for (int y = 0; y < n; y++) {
            getline(cin,line);
            board.push_back(line);
            for(int x; x < m; x++ ){
                if (isdigit(line[x]) ){
                    //add tower/bloker
                    insertBloker(x, y, noGoLine, noGoCol);
                }
                if (line[x] =='#') insertBloker(x, y, noGoLine, noGoCol);
            }
        }
        solution = board;

        outpostCount.assign(n, vector<int>(m, 0));

        backtrack(0);

        if (solved) {
            cout << turrets<<"\n";
            /*for (const auto &row : solution) {
                for (char c : row)
                    cout << c;
                cout << endl;
            }*/
        } else {
            cout <<"noxus will rise!\n";
        }
        
        solution.clear();
        board.clear();
        outpostCount.clear();
        solved = false;
        noGoLine.clear();
        noGoCol.clear();
    }
    return 0;
}
