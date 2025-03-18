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

bool insertTurrets(int x, int y, char t, vector<vector<tuple<int, int, bool>>> &noGoLine, vector<vector<tuple<int, int, bool>>> &noGoCol){
    int n = t - '0';
    for (int i = 0; i < 4 ; i++){
        if (4-i<n) return false;
        if (n<=0) break;
        //auto segments = noGoLine.find(); to be changed
        //if (segments != noGoLine.end());
    }

    turrets += t - '0';  
    return true;
}
int main() {
    int n_test_cases;
    cin >> n_test_cases;
    for (int i = 0; i < n_test_cases; i++) {

        turrets = 0;
        cin >> nl;
        cin >> nc;

        vector<vector<tuple<int, int, bool>>> noGoLine; //y 
        vector<vector<tuple<int, int, bool>>> noGoCol;  //x
        noGoLine.resize(nl);
        noGoCol.resize(nc);
        string line;
        queue<tuple<int, int , char>> startingTurrets;

        //ler a grid e para cada objeto fazer pré processamento
        for (int y = 0; y < nl; y++) {
            getline(cin,line);
            for(int x; x < nc; x++ ){
                if (isdigit(line[x]) ){
                    //add tower/bloker
                    insertBloker(x, y, noGoLine, noGoCol);
                    //add n turrets around the tower to the queue
                    startingTurrets.push(make_tuple(x,y, line[x]));
                }
                if (line[x] =='#') insertBloker(x, y, noGoLine, noGoCol);
            }
            while (!startingTurrets.empty()){
                auto ts = startingTurrets.front();
                if((notPossible = !insertTurrets(get<0>(ts), get<1>(ts), get<2>(ts), noGoLine, noGoCol))) {
                    cout << "noxus will rise!\n";
                    break;
                }
                
            }
            
            if (notPossible) continue;
        }
    }

    //debugging



    return 0;
}
