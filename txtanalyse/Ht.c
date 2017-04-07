#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define KOKO 200

typedef struct {
    char sana[KOKO];
    int lkm;
}Luettu;

void yleisimmatSanat(FILE * teksti);
void jaaMerkkijono(char jono[], Luettu taulukko [], int taulukon_koko, int * eri_sanojenlkm, int * sanojenlkm);
int tarkistus(char merkki);
int laskelkm(FILE * teksti);
int laskepaikka(char jono[]);
int luotaus(char * osajono, Luettu taulukko[], int pituus, int * eri_sanojenlkm);
void buildMaxHeap (Luettu taulukko[], int heap_size);
void maxHeapify(Luettu taulukko[], int i, int heap_size);
void exchangeLuettu(Luettu * ens, Luettu * toin);
void tulostataulukko(Luettu yleisimmat[], int sanojenlkm);

int main(void) {
    char nimi[KOKO];
	clock_t start,end;
	double totaltime;
    FILE * teksti = NULL;
    printf("Anna tiedoston nimi > ");
    scanf("%s", nimi);
    if((teksti = fopen(nimi, "r")) == NULL) {                          // Varmistetaan, ett� tarkasteltavaa tiedostoa p��st��n lukemaan
        if((teksti = fopen(nimi, "r")) == NULL) {
            printf("Ohjelma ei pystynyt avaamaan tiedostoa\n");
            return(0);
        }
        else {
            fclose(teksti);
            if((teksti = fopen(nimi, "r")) == NULL) {
                printf("Ohjelma ei pystynyt avaamaan tiedostoa\n");
                return(0);
            }
        }
    }
	start = clock();
    yleisimmatSanat(teksti);                                           // Tulostetaan tiedoston "teksti" 100 yleisint� sanaa
	end = clock();
	totaltime = (double)(end-start)/CLOCKS_PER_SEC;
    printf("\nOhjelman suoritukseen kuluu noin %f sekuntia.", totaltime);
    fclose(teksti);
    getch();
    return 0;
}

void yleisimmatSanat(FILE * teksti) {
    char luett[KOKO];
    memset(luett, '\0', sizeof(luett));                                // Alustetaan taulukon "luett" alkiot NUL-merkeill�
    Luettu yleisimmat[100];                                            // Taulukko, johon sijoitetaan tiedoston 100 yleisinta sanaa
    int i=0;
    int heap_size = 0;
    int taulukon_koko;
    int eri_sanojenlkm = 0;
    int raja;
    int sanojenlkm = 0;
    rewind(teksti);
    taulukon_koko = laskelkm(teksti);
    Luettu * taulukko;
    if(taulukon_koko < 10000) {                                           // Jos tiedostossa on v�h�n sanoja, niin t�m�n avulla voidaan mahdollisesti v�ltt�� ylivuotoa
        taulukon_koko = 2*taulukon_koko;
        taulukko = malloc(sizeof(Luettu)*taulukon_koko);
    }
    else if(taulukon_koko >= 10000 && taulukon_koko <= 100000)
        taulukko = malloc(sizeof(Luettu)*taulukon_koko);
    else if(taulukon_koko > 100000 && taulukon_koko <= 500000) {          // Jos taulukossa on paljon sanoja, niin riitt�� pienemm�nkin kokoinen taulukko tallentamaan sanoja ja niiden esiintymiskertoja
        taulukon_koko = taulukon_koko/5;
        taulukko = malloc(sizeof(Luettu)*taulukon_koko);
    }
    else {
        taulukon_koko = 100*(int)sqrt((double)taulukon_koko);
        taulukko = malloc(sizeof(Luettu)*taulukon_koko);
    }
    rewind(teksti);
    for(i=0; i<taulukon_koko; i++) {                                      // Alustetaan taulukon alkioiden merkkijonot NUL-merkeill� ja niiden esiintymiskerrat arvolla 0
        taulukko[i].lkm = 0;
        memset(taulukko[i].sana, '\0', sizeof(taulukko[i].sana));
    }
    while(fscanf(teksti, "%199s", luett)==1) {
        jaaMerkkijono(luett, taulukko, taulukon_koko, &eri_sanojenlkm, &sanojenlkm);    // Jaetaan luetut merkkijonot halutunlaisiksi sanoiksi ja k�ytet��n luotausta niiden lis��miseksi taulukkoon
    }
    heap_size = taulukon_koko;
    buildMaxHeap(taulukko, taulukon_koko);                                        // Rakennetaan taulukosta maksimikeko
    if(taulukon_koko < 100)                                                       // Jos tiedostossa on v�hemm�n kuin 100 sanaa, niin t�ss� varmistetaan, ettei se aiheuta virhett� seuraavassa for-silmukassa
        raja = -1;
    else
        raja = taulukon_koko-101;
    for(i=taulukon_koko-1; i>raja; i--) {                                         // J�rjestet��n tiedoston 100 yleisint� sanaa esiintymiskertojen mukaisessa j�rjestyksess�
        exchangeLuettu(&taulukko[0], &taulukko[i]);
        yleisimmat[(taulukon_koko-i-1)] = taulukko[i];                            // Indeksill� 0 on se sana, jolla on suurin esiintymiskerta
        heap_size = heap_size - 1;
        maxHeapify(taulukko, 1, heap_size);                                       // Tehd��n taulukosta uudelleen maksimikeko (vain alkio indeksill� 0 saattaa olla v��r�ss� paikassa, joten riitt�� yksi maxHeapify-kutsu)
    }
    tulostataulukko(yleisimmat, eri_sanojenlkm);
    printf("\n");
    //printf("\nMuuttujan \"taulukon_koko\" arvo on %d.", taulukon_koko);
    printf("\nTiedoston sanojen lukumaara on %d.", sanojenlkm);
    printf("\nTiedoston erilaisten sanojen lukumaara on %d.", eri_sanojenlkm);
}

void jaaMerkkijono(char jono[], Luettu taulukko [], int taulukon_koko, int * eri_sanojenlkm, int * sanojenlkm) {
    int i = 0;
    int j;
    char osajono[KOKO];
    strlwr(jono);                                  // Muuttaa merkkijonon "jono" kaikki kirjaimet pieniksi kirjaimiksi
    while(tarkistus(jono[i])) {                    // Kasvattaa i:n arvoa niin kauan, ett� vastaan tulee sellainen merkki, joka on joko kirjain v�lilt� a-z tai heittomerkki
        if(jono[i++] == '\0')
            return;
    }
    j = i+1;
    while(!tarkistus(jono[j])) {                   // Kasvattaa j:n (j l�htee arvosta i+1) arvoa niin kauan, ett� vastaan tulee jokin merkki, joka ei ole kirjain v�lilt� a-z tai my�sk��n heittomerkki
        j++;
    }
    strncpy(osajono, &jono[i], j-i);               // Kopioidaan merkkijonosta "jono" kaikki ne merkit, jotka ovat indeksin arvolla i tai indeksien i ja j arvojen v�lill� (indeksin j kohdalla olevaa merkki� ei kopioida)
    osajono[(j-i)]='\0';
    while(1) {                                                                 // Toteutetaan samaa kuin edell�
        (*sanojenlkm)++;                                                       // Lasketaan tarkasteltavan tiedoston sanojen lukum��r�
        if((luotaus(osajono, taulukko, taulukon_koko, eri_sanojenlkm)) == 0) {
            printf("Ylivuoto");                                                // Jos "taulukko" on t�ynn�, niin tulostetaan, ett� kaikki erilaiset sanat eiv�t mahdu taulukkoon
            exit(0);
        }
        if(jono[j] == '\0')
            return;                                                          // Jos p��st��n luetun merkkijonon loppuun, niin palataan kutsuvaan funktioon
        i = j+1;
        while(tarkistus(jono[i])) {
            if(jono[i++] == '\0')
                return;                                                      // Jos p��st��n luetun merkkijonon loppuun, niin palataan kutsuvaan funktioon
        }
        j = i+1;
        while(!tarkistus(jono[j])) {
            j++;
        }
        strncpy(osajono, &jono[i], j-i);
        osajono[(j-i)]='\0';
    }
}

int tarkistus(char merkki) {
    int vast_luku = (int) merkki;
    if(vast_luku == 39 || (vast_luku >= 97 && vast_luku <= 122))
        return 0;                                                              // Jos tarkasteltava merkki on heittomerkki tai v�lill� a-z, niin palautetaan arvo 0
    else
        return 1;
}

int laskelkm(FILE * teksti) {
    char luett[KOKO];
    int i = 0;
    while((fscanf(teksti, "%199s", luett))==1) {
        i++;
    }
    return(i);
}

int laskepaikka(char jono[]) {                                                 // Korotetaan tarkasteltavan merkkijonon kaikkien merkkien ASCII -koodien vastineet toiseen ja v�hennet��n niist� 1. Lopuksi palautetaan kaikkien n�iden yhteinen summa
    int pituus;
    int i = 0;
    int summa = 0;
    pituus = strlen(jono);
    for(i=0; i<pituus; i++) {
        summa = summa + (int) pow((double)((int) jono[i]), 2.0) - 1;           // Jokaisesta toiseen korotetusta luvusta v�hennet��n 1 siksi, ett� t�ll� tavalla summaan lis�tt�v� luku ei v�ltt�m�tt� ole yhdistetty luku, vaan sill� on mahdollisuus olla alkuluku
    }
    return(summa);
}

int luotaus(char * osajono, Luettu taulukko[], int pituus, int * lkm) {
    int arvo=0;
    int i = 0;
    int j = 0;
    arvo = laskepaikka(osajono);
    while(i<pituus) {
        j = (arvo + i) % pituus;                                               // Tiivistefunktio, kun k�ytet��n apuna laskepaikka-funktiota ja lineaarista luotausta
        if(taulukko[j].lkm == 0) {
            taulukko[j].lkm = 1;
            strcpy(taulukko[j].sana, osajono);
            (*lkm)++;                                                          // Lasketaan tarkasteltavan tiedoston erilaisten sanojen lukum��r�
            return 1;
        }
        else if((strcmp(taulukko[j].sana, osajono)) == 0) {
            taulukko[j].lkm = taulukko[j].lkm + 1;
            return 1;
        }
        i++;
    }
    return 0;                                                                  // Jos taulukko, johon sanoja ja niiden esiintymiskertoja sijoitetaan, on t�ynn�, niin i saa muuttujan "pituus" arvon ja palautetaan arvo 0
}

void buildMaxHeap (Luettu taulukko[], int heap_size) {                         // Kekolajittelu
    int j = 0;
    int kokonaisosa = heap_size/2;
    for(j=kokonaisosa; j>=1; j--) {
        maxHeapify(taulukko, j, heap_size);
    }
}

void maxHeapify(Luettu taulukko[], int i, int heap_size) {
    int largest;
    int lft = 2*i;
    int rgt = 2*i+1;
    if(lft<=heap_size && taulukko[(lft-1)].lkm > taulukko[(i-1)].lkm)          // Taulukossa indeksointi alkaa 0:sta, ja tarkastellaan vain sanojen esiintymiskertoja (sama my�hemp�n�)
        largest = lft;
    else
        largest = i;
    if(rgt<=heap_size && taulukko[(rgt-1)].lkm > taulukko[(largest-1)].lkm)
        largest = rgt;
    if(largest != i) {
        exchangeLuettu(&taulukko[(i-1)], &taulukko[(largest-1)]);              // Vaihdetaan Luettu-alkioiden paikkoja
        maxHeapify(taulukko, largest, heap_size);
    }
}

void exchangeLuettu(Luettu * ens, Luettu * toin) {
    Luettu temp = *ens;
    *ens = *toin;
    *toin = temp;
}

void tulostataulukko(Luettu yleisimmat[], int lkm) {
    int i = 0;
    if(lkm >= 100) {
        for(i=0; i<100; i++) {
            printf("\nSana \"%s\" esiintyy tiedostossa %d kertaa.", yleisimmat[i].sana, yleisimmat[i].lkm);
        }
    }
    else {
        printf("\nTiedostossa ei ole 100 sanaa, mutta tiedostossa esiintyvien sanojen esiintymiskerrat ovat seuraavat:");
        for(i=0; i<lkm; i++) {
            printf("\nSana \"%s\" esiintyy tiedostossa %d kertaa.", yleisimmat[i].sana, yleisimmat[i].lkm);
        }
    }
}
