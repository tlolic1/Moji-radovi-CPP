//TP 2023/2024: Zadaća 5, Zadatak 2
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <iomanip>

class Berza {
    std::vector<int> cijene;
    int min_cijena, max_cijena;
    public:
    Berza(int min_cijena, int max_cijena);
    explicit Berza(int max_cijena);
    void RegistrirajCijenu(int cijena);
    int DajBrojRegistriranihCijena() const;
    void BrisiSve();
    int DajMinimalnuCijenu() const;
    int DajMaksimalnuCijenu() const;
    bool operator!() const;
    int DajBrojCijenaVecihOd(int cijena) const;
    void Ispisi() const;
    int operator[](int index) const;
    Berza& operator++();
    Berza operator++(int);
    Berza& operator--();
    Berza operator--(int);
    Berza operator-() const;
    Berza operator+(int y) const;
    Berza operator-(int y) const;
    friend Berza operator+(int y, const Berza& b);
    friend Berza operator-(int y, const Berza& b);
    Berza operator+(const Berza& drugi) const;
    Berza operator-(const Berza& drugi) const;
    Berza& operator+=(int y);
    Berza& operator-=(int y);
     Berza& operator+=(const Berza& drugi);
    Berza& operator-=(const Berza& drugi);
    bool operator==(const Berza& drugi) const;
    bool operator!=(const Berza& drugi) const;
};

Berza::Berza(int min_cijena, int max_cijena) {
    if (min_cijena < 0 || max_cijena < 0 || min_cijena > max_cijena)
        throw std::range_error("Ilegalne granicne cijene");
    this->min_cijena = min_cijena;
    this->max_cijena = max_cijena;
}

Berza::Berza(int max_cijena) : Berza(0, max_cijena) {}

void Berza::RegistrirajCijenu(int cijena) {
    if (cijena < min_cijena || cijena > max_cijena)
        throw std::range_error("Ilegalna cijena");
    cijene.push_back(cijena);
}

int Berza::DajBrojRegistriranihCijena() const {
    return cijene.size();
}

void Berza::BrisiSve() {
    cijene.clear();
}

int Berza::DajMinimalnuCijenu() const {
    if (cijene.empty())
        throw std::range_error("Nema registriranih cijena");
    return *std::min_element(cijene.begin(), cijene.end());
}

int Berza::DajMaksimalnuCijenu() const {
    if (cijene.empty())
        throw std::range_error("Nema registriranih cijena");
    return *std::max_element(cijene.begin(), cijene.end());
}

bool Berza::operator!() const {
    return cijene.empty();
}

int Berza::DajBrojCijenaVecihOd(int cijena) const {
    if (cijene.empty())
        throw std::range_error("Nema registriranih cijena");
    return std::count_if(cijene.begin(), cijene.end(), [cijena](int x) { return x > cijena; });
}

void Berza::Ispisi() const {
    std::vector<int> sorted_cijene = cijene;
    std::sort(sorted_cijene.begin(), sorted_cijene.end(), [](int a, int b) { return a > b; });
    for (int cijena : sorted_cijene) {
        std::cout << std::fixed << std::setprecision(2) << cijena / 100.0 << std::endl;
    }
}

int Berza::operator[](int index) const {
    if (index < 1 || index > DajBrojRegistriranihCijena())
        throw std::range_error("Neispravan indeks");
    return cijene[index - 1];
}

Berza& Berza::operator++() {
    std::transform(cijene.begin(), cijene.end(), cijene.begin(), [this](int cijena) {
        if (cijena + 100 > max_cijena)
            throw std::range_error("Prekoracen dozvoljeni opseg cijena");
        return cijena + 100;
    });
    return *this;
}

Berza Berza::operator++(int) {
    Berza old = *this;
    ++(*this);
    return old;
}

Berza& Berza::operator--() {
    std::transform(cijene.begin(), cijene.end(), cijene.begin(), [this](int cijena) {
        if (cijena - 100 < min_cijena)
            throw std::range_error("Prekoracen dozvoljeni opseg cijena");
        return cijena - 100;
    });
    return *this;
}

Berza Berza::operator--(int) {
    Berza old = *this;
    --(*this);
    return old;
}

Berza Berza::operator-() const {
    Berza nova(min_cijena, max_cijena);
    nova.cijene.resize(cijene.size());
    std::transform(cijene.begin(), cijene.end(), nova.cijene.begin(), [this](int cijena) {
        return max_cijena + min_cijena - cijena;
    });
    return nova;
}

Berza Berza::operator+(int y) const {
    Berza nova(min_cijena, max_cijena);
    nova.cijene.resize(cijene.size());
    std::transform(cijene.begin(), cijene.end(), nova.cijene.begin(), [this, y](int cijena) {
        int nova_cijena = cijena + y;
        if (nova_cijena > max_cijena)
            throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        return nova_cijena;
    });
    return nova;
}

Berza Berza::operator-(int y) const {
    Berza nova(min_cijena, max_cijena);
    nova.cijene.resize(cijene.size());
    std::transform(cijene.begin(), cijene.end(), nova.cijene.begin(), [this, y](int cijena) {
        int nova_cijena = cijena - y;
        if (nova_cijena < min_cijena)
            throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        return nova_cijena;
    });
    return nova;
}


Berza operator+(int y, const Berza& b) {
    return b + y;
}

Berza operator-(int y, const Berza& b) {
    Berza nova(b.min_cijena, b.max_cijena);
    nova.cijene.resize(b.cijene.size());
    std::transform(b.cijene.begin(), b.cijene.end(), nova.cijene.begin(), [&b, y](int cijena) {
        int nova_cijena = y - cijena;
        if (nova_cijena < b.min_cijena || nova_cijena > b.max_cijena)
            throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        return nova_cijena;
    });
    return nova;
}


Berza Berza::operator+(const Berza& drugi) const {
    if (min_cijena != drugi.min_cijena || max_cijena != drugi.max_cijena || cijene.size() != drugi.cijene.size())
        throw std::domain_error("Nesaglasni operandi");
    Berza nova(min_cijena, max_cijena);
    nova.cijene.resize(cijene.size());
    std::transform(cijene.begin(), cijene.end(), drugi.cijene.begin(), nova.cijene.begin(), [this](int c1, int c2) {
        int result = c1 + c2;
        if (result > max_cijena || result < min_cijena)
            throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        return result;
    });
    return nova;
}

Berza Berza::operator-(const Berza& drugi) const {
    if (min_cijena != drugi.min_cijena || max_cijena != drugi.max_cijena || cijene.size() != drugi.cijene.size())
        throw std::domain_error("Nesaglasni operandi");
    Berza nova(min_cijena, max_cijena);
    nova.cijene.resize(cijene.size());
    std::transform(cijene.begin(), cijene.end(), drugi.cijene.begin(), nova.cijene.begin(), [this](int c1, int c2) {
        int result = c1 - c2;
        if (result > max_cijena || result < min_cijena)
            throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        return result;
    });
    return nova;
}

Berza& Berza::operator+=(int y) {
    *this = *this + y;
    return *this;
}

Berza& Berza::operator-=(int y) {
    *this = *this - y;
    return *this;
}

bool Berza::operator==(const Berza& drugi) const {
    return cijene == drugi.cijene;
}

bool Berza::operator!=(const Berza& drugi) const {
    return !(*this == drugi);
}
Berza& Berza::operator+=(const Berza& drugi) {
    if (min_cijena != drugi.min_cijena || max_cijena != drugi.max_cijena || cijene.size() != drugi.cijene.size())
        throw std::domain_error("Nesaglasni operandi");
    std::transform(cijene.begin(), cijene.end(), drugi.cijene.begin(), cijene.begin(), [this](int c1, int c2) {
        int result = c1 + c2;
        if (result > max_cijena || result < min_cijena)
            throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        return result;
    });
    return *this;
}

Berza& Berza::operator-=(const Berza& drugi) {
    if (min_cijena != drugi.min_cijena || max_cijena != drugi.max_cijena || cijene.size() != drugi.cijene.size())
        throw std::domain_error("Nesaglasni operandi");
    std::transform(cijene.begin(), cijene.end(), drugi.cijene.begin(), cijene.begin(), [this](int c1, int c2) {
        int result = c1 - c2;
        if (result > max_cijena || result < min_cijena)
            throw std::domain_error("Prekoracen dozvoljeni opseg cijena");
        return result;
    });
    return *this;
}


int main() {
    
    return 0;
}

