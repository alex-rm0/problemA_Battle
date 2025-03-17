#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <list>
#include <unordered_set>
#include <set>

using namespace std;

vector<int> v;

int nl, nc, turrets;
bool notPossible;


void insertBloker(int x, int y){
    blokersInCol[x].insert(y);
    blokersInLine[y].insert(x);
}
bool insertTurrets(int x, int y, char t){
    int n = t - '0';
    for (int i = 0; i < 4 ; i++){
        if (n <= 0) return true;
        auto segments = noGoLine.find();
        if (segments != noGoLine.end());
    }

    if (n > 0)  return false;
}
int main() {
    int n_test_cases;
    cin >> n_test_cases;
    for (int i = 0; i < n_test_cases; i++) {

        turrets = 0;
        cin >> nl;
        cin >> nc;

        list<vector<pair<int, int>>> noGoLine[nl]; //y 
        list<vector<pair<int, int>>> noGoCol[nc];  //x

        list<set <int>> blokersInLine[nl];
        list<set <int>> blokersInCol[nc];
        
        string line;

        //ler a grid e para cada objeto fazer pré processamento
        for (int y = 0; y < nl; y++) {
            getline(cin,line);
            for(int x; x < nc; x++ ){
                if (isdigit(line[x]) ){
                        //add tower/bloker
                        insertBloker(x, y);
                        //add n turrets around the tower
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
