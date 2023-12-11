#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
#define N 8

//sluzi za 1. korak u algoritmu, odnosno kreiranje neke proizvoljne
//konfiguracije kraljica. Ideju za generator random brojeva koristim
//s kolegija Programiranje 1
void nasumicnaKonfiguracija(int ploca[][N], int* stanje) {
    srand(time(0));
    int i;
    for (i = 0; i < N; i++) {
        stanje[i] = rand() % N;
        ploca[stanje[i]][i] = 1;
    }
}

void ispisiPlocu(int ploca[][N]) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            if (ploca[i][j]) cout << "q ";
            else cout << ". ";
        cout << endl;
    }
}

//bit ce potrebna kada budemo trazili susjedna stanja kako bismo pronadjeno
//susjedno stanje usporedili s postojecim te na osnovu bool vrijednosti
//odlucujemo u kojem smjeru ce algoritam ici
bool usporediStanja(int *stanje1, int *stanje2) {
    int i;
    for (i = 0; i < N; i++)
        if (stanje1[i] != stanje2[i])
            return false;
    return true;
}

//nakon ili prije ispitivanja vrijednsti heuristicke funkcije bit ce 
//potrebno uciniti plocu praznom, tj. takvom da na njoj nije postavljena
//niti jedna kraljica za vrijednost = 0
void ispuniPlocu(int ploca[][N], int vrijednost) {
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            ploca[i][j] = vrijednost;
}

//racuna vrijednost heuristicke funkcije nad sahovskom plocom reprezentiranom
//u dvodim. polju ploca sa stanjem reprezentiranom u polju stanje
int h(int ploca[][N], int *stanje) {
    int ukupan_broj = 0; //ukupan broj medjusobnih napada kraljica
    int redak, stupac, i;
    //za svaku kraljicu provjeravamo nalazi li se neka druga kraljica
    //u njezinom retku ili pak na jednoj od cetiri dijagonale
    //ako je tome slucaj, vrijednost varijable ukupan_broj povecavamo za 1
    for (i = 0; i < N; i++) {
        //gledamo postoji li neka kraljica u lijevom dijelu retka
        redak = stanje[i], stupac = i - 1;
        while (stupac >= 0 && ploca[redak][stupac] != 1) stupac--;
        if (stupac >= 0 && ploca[redak][stupac] == 1) ukupan_broj++;

        //gledamo postoji li neka kraljica u desnom dijelu retka
        redak = stanje[i], stupac = i + 1;
        while (stupac < N && ploca[redak][stupac] != 1) stupac++;
        if (stupac < N && ploca[redak][stupac] == 1) ukupan_broj++;

        //u sljedeca 4 "odlomka" redom gledamo postoji li neka kraljica na 
        //jednoj od cetiri dijagonale: lijevo gore, desno dolje, lijevo
        //dolje i desno gore
        redak = stanje[i] - 1, stupac = i - 1;
        while (redak >= 0 && stupac >= 0 && ploca[redak][stupac] != 1) 
            stupac--, redak--;
        if (redak >= 0 && stupac >= 0 && ploca[redak][stupac] == 1) ukupan_broj++;

        redak = stanje[i] + 1, stupac = i + 1;
        while (redak < N && stupac < N && ploca[redak][stupac] != 1)
            stupac++, redak++;
        if (redak < N && stupac < N && ploca[redak][stupac] == 1) ukupan_broj++;

        redak = stanje[i] + 1, stupac = i - 1;
        while (redak < N && stupac >= 0 && ploca[redak][stupac] != 1)
            redak++, stupac--;
        if (redak < N && stupac >= 0 && ploca[redak][stupac] == 1) ukupan_broj++;

        redak = stanje[i] - 1, stupac = i + 1;
        while (redak >= 0 && stupac < N && ploca[redak][stupac] != 1)
            redak--, stupac++;
        if (redak >= 0 && stupac < N && ploca[redak][stupac] == 1) ukupan_broj++;
    }
    //vracamo dvostruko manji broj jer ako se kraljice k1 i k2 napadaju
    //onda smo taj napad racunali dva puta - jednom kada smo u for petlji
    //ispitivali napada li k1 nekoga, a drugi put kada smo u for petlji
    //ispitivali napada li k2 nekoga 
    return (int) ukupan_broj / 2;
}

//jednom kada odaberemo susjedno stanje, bit ce potrebno azurirati
//stanje na samoj ploci, odnosno azurirati polozaj kraljice koju smo
//pomaknuli
void kreirajZadanuPlocu(int ploca[][N], int* stanje) {
    ispuniPlocu(ploca, 0);
    int i;
    for (i = 0; i < N; i++)
        ploca[stanje[i]][i] = 1;
}

//bit ce potrebna prilikom trazenja optimalnog stanja jer cemo nakon
//pronalaska boljeg stanja to isto stanje htjeti spremiti u varijablu
//int *opt_stanje koja predstavlja do tad najbolje stanje da bismo dalje
//mogli usporedjivati i doci do globalnog optimuma
void kopirajStanja(int *stanje1, int *stanje2) {
    int i;
    for (i = 0; i < N; i++)
        stanje1[i] = stanje2[i];
}

//funkcija potrebna za korak 2 u opisu algoritma (dokumentacija). Vraca
//optimalno susjedstvo, odnosno ono susjedstvo koje ce najvise sniziti
//vrijednost heuristicke funkcije h
void opt_susjedstvo(int ploca[][N], int *stanje) {
    //sljedece dvije varijable ce na kraju funkcije sadrzavati informacije
    //o optimalnom susjedstvu
    int optPloca[N][N];
    int optStanje[N];
    kopirajStanja(optStanje, stanje);
    kreirajZadanuPlocu(optPloca, optStanje);

    int opt_vrijednost_h = h(optPloca, optStanje); //vrijednost funkcije h za trenutno stanje na ploci

    //sljedece dvije varijable su privremene te sluze za ispitivanje
    //odnosa izmedju dobivenog i (do tad) optimalnog susjedstva. Dakle,
    //u tempStanje ce biti spremljeno neko susjedstvo, a u tempPloca bit
    //ce raspored kraljica u skladu s tempStanje
    int tempPloca[N][N];
    int tempStanje[N];
    kopirajStanja(tempStanje, stanje);
    kreirajZadanuPlocu(tempPloca, tempStanje);

    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            //isprobavamo sve polozaje neke kraljice u njezinom stupcu
            if (j != stanje[i]) {
                //u sljedeca tri retka azuriramo pomak kraljice i
                tempStanje[i] = j;
                tempPloca[tempStanje[i]][i] = 1;
                tempPloca[stanje[i]][i] = 0;

                int temp_vrijednost_h = h(tempPloca, tempStanje);
                if (temp_vrijednost_h <= opt_vrijednost_h) {
                    opt_vrijednost_h = temp_vrijednost_h;
                    kopirajStanja(optStanje, tempStanje);
                    kreirajZadanuPlocu(optPloca, optStanje);
                }

                //kada dodjemo ovdje, ispitali smo pripadno susjedno
                //stanje te moramo vratiti pocetno stanje i raspored
                //kraljica na ploci za pravilno izvrsavanje iduce iteracije
                tempPloca[tempStanje[i]][i] = 0;
                tempStanje[i] = stanje[i];
                tempPloca[stanje[i]][i] = 1;
            }
        }
    }
    //u ovom trenutku u optStanje imamo optimalno susjedstvo pa cemo
    //to spremiti u varijablu stanje koja je proslijedjena funkciji
    //i uskladiti stanje na ploci
    kopirajStanja(stanje, optStanje);
    kreirajZadanuPlocu(ploca, stanje);
}  

//algoritam uspona na vrh - hill climbing
void hill_climbing(int ploca[][N], int* stanje) {
    int susjednaPloca[N][N];
    int susjednoStanje[N];
    kopirajStanja(susjednoStanje, stanje);
    kreirajZadanuPlocu(susjednaPloca, susjednoStanje);
    while (1) {
        kopirajStanja(stanje, susjednoStanje);
        kreirajZadanuPlocu(ploca, stanje);

        opt_susjedstvo(susjednaPloca, susjednoStanje);
        //ako je optimalno susjedno stanje jednako trenutnom stanju
        if (usporediStanja(stanje, susjednoStanje)) {
            int fja_h = h(susjednaPloca, susjednoStanje);
            if (fja_h > 0) 
                cout << "Nismo pronasli rjesenje!" << endl;
            else
                ispisiPlocu(ploca);
            break;
        } //ako else if prodje, to znaci da smo zapeli u lokalnom optimumu ili "ramenu"
        else if (h(ploca, stanje) == h(susjednaPloca, susjednoStanje)) {
            //rekli smo da cemo za bjezanje iz lokalnog optimuma koristiti
            //nasumicno odabrano susjedstvo
            susjednoStanje[rand() % N] = rand() % N;
            kreirajZadanuPlocu(susjednaPloca, susjednoStanje);
        }
    }
}

int main (void) {
    int stanje[N] = {};
    int ploca[N][N] = {};
    //krecemo s proizvoljnim rasporedom kraljica na sahovskoj ploci
    nasumicnaKonfiguracija(ploca, stanje);
    hill_climbing(ploca, stanje);
    return 0;
}