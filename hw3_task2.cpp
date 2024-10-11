//TP 2023/2024: Zadaća 3, Zadatak 2
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <tuple>
#include <map>
#include <stdexcept>

typedef std::map<std::string, std::vector<std::string>> Knjiga;
typedef std::map<std::string, std::set<std::tuple<std::string, int, int>>> IndeksPojmova;

IndeksPojmova KreirajIndeksPojmova(Knjiga tekst) {
    IndeksPojmova indeks_pojmova;
    for(const auto& poglavlje : tekst) {
        const std::string& naziv_poglavlja = poglavlje.first;
        const std::vector<std::string>& stranice = poglavlje.second;
        for(int i = 0; i < stranice.size(); i++) {
            const std::string& s = stranice[i];
            for(int j = 0; j < s.length(); j++)
                if(std::isalnum(s[j]) && (j == 0 || !std::isalnum(s[j - 1]))) { 
                    std::string rijec;
                    int pocetak = j;
                    while(j < s.length() && std::isalnum(s[j])) {
                        if(std::isalpha(s[j])) rijec += std::toupper(s[j++]); 
                        else rijec += s[j++];
                    }
                    indeks_pojmova[rijec].insert({naziv_poglavlja, i + 1, pocetak + 1});
                }
        }
    }
    return indeks_pojmova;
}

std::set<std::tuple<std::string, int, int>> PretraziIndeksPojmova(const std::string &rijec, const IndeksPojmova &indeks_pojmova) {
    std::string rijec_upper;
    for (char c : rijec) {
        if (!std::isalpha(c)) {
            throw std::domain_error("Neispravna rijec!");
        }
        rijec_upper += std::toupper(c);
    }
    auto it = indeks_pojmova.find(rijec_upper);
    if(it != indeks_pojmova.end()) return it->second;
    return {}; 
}

void IspisiIndeksPojmova(const std::map<std::string, std::set<std::tuple<std::string, int, int>>>& indeks_pojmova) {
  for (const auto& pojmovi : indeks_pojmova) {
     std::string rijec = pojmovi.first;
     std::set<std::tuple<std::string, int, int>> pozicije = pojmovi.second;
     std::string rijec_velikim;
    for (char c : rijec) {
      rijec_velikim += std::toupper(c);
    }
    std::cout << rijec_velikim << ": ";
    bool first = true;
    for (const auto& pozicija : pozicije) {
      std::string poglavlje;
      int stranica, indeks;
      std::tie(poglavlje, stranica, indeks) = pozicija;
      if (!first) {
        std::cout << ", ";
      }
      std::cout << poglavlje << "/" << stranica << "/" << indeks;
      first = false;
    }
    std::cout << std::endl;
  }
}

int main() {
    Knjiga tekst;
    std::string poglavlje_ime;
	int br_stranice=1;
    do {
        std::cout << "Unesite naziv poglavlja: ";
        std::getline(std::cin, poglavlje_ime);
        if (!poglavlje_ime.empty()) {
            std::string stranica_sadrzaj;
            do {
                std::cout << "Unesite sadrzaj stranice "<< br_stranice <<": ";
                std::getline(std::cin, stranica_sadrzaj);
                if (!stranica_sadrzaj.empty()) {
                    tekst[poglavlje_ime].push_back(stranica_sadrzaj);
                }
				br_stranice++;
            } while (!stranica_sadrzaj.empty());
        }
    } while (!poglavlje_ime.empty());
    IndeksPojmova indeks_pojmova = KreirajIndeksPojmova(tekst);
    std::cout << std::endl << "Kreirani indeks pojmova: "<<std::endl;
    IspisiIndeksPojmova(indeks_pojmova);
    std::string rijec;
    do {
        std::cout << std::endl<<"Unesite rijec: ";
        std::getline(std::cin, rijec);

        if (!rijec.empty()) {
            try {
                std::set<std::tuple<std::string, int, int>> rezultat = PretraziIndeksPojmova(rijec, indeks_pojmova);
                if (rezultat.empty()) {
                    std::cout << "Rijec nije nadjena!";
                } else {
                    std::cout << "Rijec nadjena na pozicijama: ";
                    for (const auto& pozicija : rezultat) {
                        std::string poglavlje;
                        int stranica, indeks;
                        std::tie(poglavlje, stranica, indeks) = pozicija;
                        std::cout << poglavlje << "/" << stranica << "/" << indeks<<" ";
                    }
                }
            } catch (const std::logic_error& e) {
                std::cout << e.what();
            }
        }
    } while (!rijec.empty());
    std::cout << "Dovidjenja!" << std::endl;
    return 0;
}
