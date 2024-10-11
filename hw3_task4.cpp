//TP 2023/2024: Zadaća 3, Zadatak 4
#include <iostream>
#include <list>
#include <stdexcept>
#include <vector>

std::vector<int> Razbrajanje(int N, int M) {
    std::list<int> dstrkt;
    for(int i = 1; i <= N; i++) {
        dstrkt.push_back(i);
    }
    std::vector<int> rezultat;
    auto it = dstrkt.begin();
    rezultat.push_back(*it);
    it = dstrkt.erase(it);
    while(!dstrkt.empty()) {
        for(int i = 0; i < M - 1; i++) {
            it++;
            if(it == dstrkt.end()) {
                it = dstrkt.begin();
            }
        }
        rezultat.push_back(*it);
        it = dstrkt.erase(it);
        if(it == dstrkt.end()) {
            it = dstrkt.begin();
        }
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