#include <algorithm>
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;

int br_rjesenja;
int broj_skakaca;
int ukupno_skakaca;


//provjeravamo mozemo li skakaca postaviti na mjesto ploca[x][y]
int provjeri(int **ploca, int N, int x, int y) {
    if ((x + 2) < N && (y - 1) >= 0 && ploca[x + 2][y - 1] == 1)
        return 0;
    if ((x - 2) >= 0 && (y - 1) >= 0 && ploca[x - 2][y - 1] == 1)
        return 0;
    if ((x + 2) < N && (y + 1) < N && ploca[x + 2][y + 1] == 1)
        return 0;
    if ((x - 2) >= 0 && (y + 1) < N && ploca[x - 2][y + 1] == 1)
        return 0;
    if ((x + 1) < N && (y + 2) < N && ploca[x + 1][y + 2] == 1)
        return 0;
    if ((x - 1) >= 0 && (y + 2) < N && ploca[x - 1][y + 2] == 1)
        return 0;
    if ((x + 1) < N && (y - 2) >= 0 && ploca[x + 1][y - 2] == 1)
        return 0;
    if ((x - 1) >= 0 && (y - 2) >= 0 && ploca[x - 1][y - 2] == 1)
        return 0;

    return 1;
}

void dfs_knights(int **ploca, int N, int col, int row) {
    int i, j;
    if (ukupno_skakaca == broj_skakaca) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++)
                if (ploca[i][j] == 1) cout << "k ";
                else cout << ". ";
            cout << endl;
        }
        br_rjesenja++;
        cout << endl;
        return;
    }

    for (i = col; i < N; i++) {
        for (j = row; j < N; j++)
            if (ploca[i][j] == 0) {
                if (provjeri(ploca, N, i, j)) {
                    ploca[i][j] = 1;
                    ukupno_skakaca++;
                    dfs_knights(ploca, N, i, j);
                    ukupno_skakaca--;
                    ploca[i][j] = 0;
                }
            } 
        row = 0;
    }
    
    return;
}


int main (void) {
    int** ploca;
    int N, i, j;
    cout << "Unesite dimenzije sahovske ploce > "; cin >> N;
    cout << "Unesite broj skakaca koje treba postaviti > "; cin >> broj_skakaca;
    
    ploca = new int*[N];
    for (i = 0; i < N; i++)
        ploca[i] = new int[N];
    
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            ploca[i][j] = 0;
    
    auto start = high_resolution_clock::now();
    dfs_knights(ploca, N, 0, 0);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Broj rjesenja: " << br_rjesenja << endl;
    cout << endl << "Vrijeme izvrsavanja: " << duration.count() << " sekundi" << endl;

    return 0;
}