//TP 2023/2024: Zadaća 3, Zadatak 5
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

struct Distrikt{
    int broj_distrikta;
    Distrikt *sljedeci;
};
std::vector<int> Razbrajanje(int N, int M) {
   Distrikt *pocetak=nullptr, *prethodni=nullptr;
   for(int i = 1; i <= N; i++) {
        Distrikt *novi = new Distrikt{i, nullptr};
        if(!pocetak) {
            pocetak=novi;
        }
        else {
            prethodni->sljedeci=novi;
        }
        prethodni=novi;
    }
    prethodni->sljedeci=pocetak;
    Distrikt *p = pocetak;
    std::vector<int> rezultat;
    int kontrola=N;
    while(N>0) {    
        if(N==kontrola){
            rezultat.push_back(p->broj_distrikta);
            prethodni->sljedeci = p->sljedeci;
            Distrikt *zaBrisanje = p;
            p = prethodni->sljedeci;
            delete zaBrisanje;
            N--;
        }
       
        for(int i = 0; i < M - 1; i++) {
            prethodni = p; p = p->sljedeci;
        }
        rezultat.push_back(p->broj_distrikta);
        prethodni->sljedeci = p->sljedeci;
        Distrikt *zaBrisanje = p;
        p = prethodni->sljedeci;
        delete zaBrisanje;
        N--;
    }
    return rezultat;
}
int OdabirKoraka(int N, int K) {
    for(int M = 1; M <= N; M++) {
        std::vector<int> rzlt = Razbrajanje(N, M);
        if(rzlt.at(rzlt.size()-1) == K) {
            return M;
        }
    }
    throw std::domain_error("Broj blokova i redni broj bloka su pozitivni cijeli brojevi i redni broj bloka ne moze biti veci od broja blokova");
}
int main() {
    int N, K;
    std::cout << "Unesite broj distrikta u gradu: ";
    std::cin >> N;
    std::cout << "Unesite redni broj distrikta u kojem se nalazi restoran: ";
    std::cin >> K;
    try{
       int M = OdabirKoraka(N, K);
    std::cout << "Trazeni korak: " << M << std::endl;
    }
    catch(const std::domain_error &e){
        std::cout<<e.what();
    }
    return 0;
}