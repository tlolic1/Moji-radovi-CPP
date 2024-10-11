//TP 2023/2024: Zadaća 3, Zadatak 6
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>
#include <memory>

struct Distrikt{
    int broj_distrikta;
    std::shared_ptr<Distrikt> sljedeci;
};
std::vector<int> Razbrajanje(int N, int M) {
   std::shared_ptr<Distrikt> pocetak=nullptr, prethodni=nullptr;
   for(int i = 1; i <= N; i++) {
        std::shared_ptr<Distrikt> novi = std::make_shared<Distrikt>();
        novi->broj_distrikta = i;
        if(!pocetak) {
            pocetak=novi;
        }
        else {
            prethodni->sljedeci=novi;
        }
        prethodni=novi;
    }
    prethodni->sljedeci=pocetak;
    std::shared_ptr<Distrikt> p = pocetak;
    std::vector<int> rezultat;
    int kontrola=N;
    while(N>0) {    
        if(N==kontrola){
            rezultat.push_back(p->broj_distrikta);
            prethodni->sljedeci = p->sljedeci;
            p = prethodni->sljedeci;
            N--;
        }
        for(int i = 0; i < M - 1; i++) {
            prethodni = p; p = p->sljedeci;
        }
        rezultat.push_back(p->broj_distrikta);
        prethodni->sljedeci = p->sljedeci;
        p = prethodni->sljedeci;
        N--;
    }
    prethodni->sljedeci = nullptr;
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