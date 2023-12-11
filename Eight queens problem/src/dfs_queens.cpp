#include <iostream>

using namespace std;

int br_rjesenja;
int br_kraljica;

int aps(int a) {
    if (a < 0)
        return -a;
    return a;
}

//provjeravamo mozemo li kraljicu postaviti na mjesto ploca[x][y]
int provjeri(int **ploca, int N, int x, int y) {
    int i, j;
    for (i = 0; i < N; i++)
        if (ploca[x][i] == 1)
            return 0;

    for (i = 0; i < N; i++) 
        if (ploca[i][y] == 1)
            return 0;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            if (ploca[i][j] == 1 && (aps(i - x) == aps(j - y)))
                return 0;

    return 1;
}

void dfs_N_queens(int **ploca, int N, int row) {
    int i, j;
    if (br_kraljica == N) {
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++)
                if (ploca[i][j] == 1) cout << "q ";
                else cout << ". ";
            cout << endl;
        }
        br_rjesenja++;
        cout << endl;
        return;
    }

    for (i = row; i < N; i++) {
        for (j = 0; j < N; j++)
            if (ploca[i][j] == 0) {
                if (provjeri(ploca, N, i, j)) {
                    ploca[i][j] = 1;
                    br_kraljica++;
                    dfs_N_queens(ploca, N, row + 1);
                    br_kraljica--;
                    ploca[i][j] = 0;
                }
            } 
    }
    
    return;
}


int main (void) {
    int** ploca;
    int N, i, j;
    cout << "Unesite broj kraljica > "; cin >> N;
    ploca = new int*[N];
    for (i = 0; i < N; i++)
        ploca[i] = new int[N];
    
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            ploca[i][j] = 0;

    dfs_N_queens(ploca, N, 0);
    cout << "Broj rjesenja: " << br_rjesenja << endl;

    return 0;
}