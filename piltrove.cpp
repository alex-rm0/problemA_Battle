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
    if (noGoCol[x].empty()) {
        noGoCol[x].push_back(make_tuple(0, y, false));
        noGoCol[x].push_back(make_tuple(y, nl, false));
    }else{}
    if (noGoLine[y].empty()) {
        noGoLine[y].push_back(make_tuple(0, x, false));
        noGoLine[y].push_back(make_tuple(x, nc, false));
    }else{}
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

        vector<vector<tuple<int, int, bool>>> noGoLine[nl]; //y 
        vector<vector<tuple<int, int, bool>>> noGoCol[nc];  //x
        
        string line;
        queue<tuple<int, int , char>> startingTurrets;

        //ler a grid e para cada objeto fazer pré processamento
        for (int y = 0; y < nl; y++) {
            getline(cin,line);
            for(int x; x < nc; x++ ){
                if (isdigit(line[x]) ){
                        //add tower/bloker
                        insertBloker(x, y, noGoLine, noGoCol);
                        //add n turrets around the tower
                        startingTurrets.push(make_tuple(x,y, line[x]));
                        if((notPossible = !insertTurrets(x, y, line[x]))) {
                            cout << "noxus will rise!\n";
                            break;
                        }
                    }
            }
            if (notPossible) continue;
        }
    }

    //debugging



    return 0;
}
