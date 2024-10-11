#include <iostream>
#include <vector>
int main() {
    std::vector<double> A = {0.2, 0.5, 0.8, 1.0, 0.7, 0.3, 0.0, 0.0, 0.0};
    std::vector<double> B = {0.0, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.0, 1.0};
    std::vector<double> presjek(9);
    std::vector<double> unija(9);
    for (int i = 0; i < 9; ++i) {
        if (A[i] < B[i]) presjek[i] = A[i];
        else presjek[i] = B[i];
    }
     for (int i = 0; i < 9; ++i) {
        if (A[i] > B[i]) unija[i] = A[i]; 
        else unija[i] = B[i];
    }
    std::cout << "Fuzzy presjek: ";
    for (int i = 0; i < 9; ++i) {
        if(presjek[i]!=0) std::cout << presjek[i] <<"/K"<< i+1 <<" ";
    }
    std::cout << std::endl;
    std::cout << "Fuzzy unija: ";
    for (int i = 0; i < 9; ++i) {
        std::cout << unija[i] <<"/K"<< i+1 <<" ";
    }
    return 0;
}