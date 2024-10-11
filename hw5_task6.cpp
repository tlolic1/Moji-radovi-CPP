//TP 2023/2024: Zadaća 5, Zadatak 6
#include <iostream>
#include <fstream>
#include <functional>
#include <stdexcept>
#include <vector>
#include <cstring>

template <typename TipElemenata>
void SwapElementsInFile(std::fstream &file, int pos1, int pos2) {
    TipElemenata elem1, elem2;
    file.seekg(pos1);
    file.read(reinterpret_cast<char*>(&elem1), sizeof(TipElemenata));
    file.seekg(pos2);
    file.read(reinterpret_cast<char*>(&elem2), sizeof(TipElemenata));
    
    file.seekp(pos1);
    file.write(reinterpret_cast<const char*>(&elem2), sizeof(TipElemenata));
    file.seekp(pos2);
    file.write(reinterpret_cast<const char*>(&elem1), sizeof(TipElemenata));
}

template <typename TipElemenata>
void SortirajBinarnuDatoteku(const char ime_datoteke[],
                             std::function<bool(TipElemenata, TipElemenata)> kriterij = std::less<TipElemenata>()) {
    std::fstream file(ime_datoteke, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        throw std::logic_error("Datoteka ne postoji");
    }

    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();
    int elementCount = fileSize / sizeof(TipElemenata);

    if (fileSize % sizeof(TipElemenata) != 0) {
        throw std::logic_error("Problemi u pristupu datoteci");
    }

    for (int i = 0; i < elementCount - 1; ++i) {
        for (int j = 0; j < elementCount - i - 1; ++j) {
            TipElemenata elem1, elem2;
            file.seekg(j * sizeof(TipElemenata));
            file.read(reinterpret_cast<char*>(&elem1), sizeof(TipElemenata));
            file.read(reinterpret_cast<char*>(&elem2), sizeof(TipElemenata));

            if (!kriterij(elem1, elem2)) {
                SwapElementsInFile<TipElemenata>(file, j * sizeof(TipElemenata), (j + 1) * sizeof(TipElemenata));
            }
        }
    }
}

bool absgreater(short x, short y) {
    return std::abs(x) > std::abs(y);
}

int main() {

    return 0;
}
