//TP 2023/2024: Zadaća 4, Zadatak 5
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <memory>
#include <cctype>
#include <iterator>
#include <utility>

class Student {
    int broj_indeksa;
    std::string godina_studija, ime_prezime, adresa, broj_telefona;
    void ValidirajParametre(int broj_indeksa, const std::string &godina_studija, const std::string &ime_prezime, const std::string &adresa, const std::string &broj_telefona) {
        if (broj_indeksa < 10000 || broj_indeksa > 99999)
            throw std::domain_error("Neispravni parametri");

        std::string validne_godine[] = {"1", "2", "3", "1/B", "2/B", "3/B", "1/M", "2/M", "1/D", "2/D", "3/D"};
        bool validna_godina = false;
        for (const auto &godina : validne_godine) {
            if (godina_studija == godina) {
                validna_godina = true;
                break;
            }
        }
        if (!validna_godina)
            throw std::domain_error("Neispravni parametri");

        if (broj_telefona.find('/') >= broj_telefona.size() - 1 || broj_telefona.find('-') >= broj_telefona.size() - 1)
            throw std::domain_error("Neispravni parametri");
    }

    std::string OcistiString(const std::string &str) {
        std::string rezultat = str;
        rezultat.erase(rezultat.begin(), std::find_if(rezultat.begin(), rezultat.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        rezultat.erase(std::find_if(rezultat.rbegin(), rezultat.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), rezultat.end());
        auto novi_kraj = std::unique(rezultat.begin(), rezultat.end(), [](char a, char b) { return std::isspace(a) && std::isspace(b); });
        rezultat.erase(novi_kraj, rezultat.end());
        return rezultat;
    }

public:
    Student(int broj_indeksa, std::string godina_studija, std::string ime_prezime, std::string adresa, std::string broj_telefona) {
        ValidirajParametre(broj_indeksa, godina_studija, ime_prezime, adresa, broj_telefona);
        this->broj_indeksa = broj_indeksa;
        this->godina_studija = godina_studija;
        this->ime_prezime = OcistiString(ime_prezime);
        this->adresa = OcistiString(adresa);
        this->broj_telefona = broj_telefona;
    }

    int DajIndeks() const { return broj_indeksa; }
    std::string DajGodinuStudija() const {
        if (godina_studija == "1") return "1/B";
        if (godina_studija == "2") return "2/B";
        if (godina_studija == "3") return "3/B";
        return godina_studija;
    }
    std::string DajImePrezime() const { return ime_prezime; }
    std::string DajAdresu() const { return adresa; }
    std::string DajTelefon() const { return broj_telefona; }

    void Ispisi() const {
        std::cout << "Broj indeksa: " << broj_indeksa << "\n"
                  << "Godina studija: " << DajGodinuStudija() << "\n"
                  << "Ime i prezime: " << ime_prezime << "\n"
                  << "Adresa: " << adresa << "\n"
                  << "Telefon: " << broj_telefona << "\n";
    }
};

class Laptop {
    int ev_broj;
    std::string naziv, karakteristike;
    Student* kod_koga_je;

public:
    Laptop(int ev_broj, std::string naziv, std::string karakteristike) : ev_broj(ev_broj), naziv(naziv), karakteristike(karakteristike), kod_koga_je(nullptr) {
        if (ev_broj < 0)
            throw std::domain_error("Neispravni parametri");
    }

    int DajEvidencijskiBroj() const { return ev_broj; }
    std::string DajNaziv() const { return naziv; }
    std::string DajKarakteristike() const { return karakteristike; }

    void Zaduzi(Student &student) {
        if (kod_koga_je != nullptr)
            throw std::domain_error("Laptop vec zaduzen");
        kod_koga_je = &student;
    }

    void Razduzi() {
        kod_koga_je = nullptr;
    }

    bool DaLiJeZaduzen() const {
        return kod_koga_je != nullptr;
    }

    Student& DajKodKogaJe() const {
        if (!kod_koga_je)
            throw std::domain_error("Laptop nije zaduzen");
        return *kod_koga_je;
    }

    Student* DajPokKodKogaJe() const {
        return kod_koga_je;
    }

    void Ispisi() const {
        std::cout << "Evidencijski broj: " << ev_broj << "\n"
                  << "Naziv: " << naziv << "\n"
                  << "Karakteristike: " << karakteristike << "\n";
    }
};

class Administracija {
    std::map<int, Student*> studenti;
    std::map<int, Laptop*> laptopi;

    void ValidirajStudenta(int broj_indeksa) const {
        if (studenti.find(broj_indeksa) == studenti.end())
            throw std::domain_error("Student nije nadjen");
    }

    void ValidirajLaptop(int ev_broj) const {
        if (laptopi.find(ev_broj) == laptopi.end())
            throw std::domain_error("Laptop nije nadjen");
    }

public:
    Administracija() = default;
    ~Administracija() {
        for (auto &par : studenti)
            delete par.second;
        for (auto &par : laptopi)
            delete par.second;
    }

    void RegistrirajNovogStudenta(int broj_indeksa, std::string godina_studija, std::string ime_prezime, std::string adresa, std::string broj_telefona) {
        if (studenti.count(broj_indeksa))
            throw std::domain_error("Student s tim indeksom vec postoji");
        studenti[broj_indeksa] = new Student(broj_indeksa, godina_studija, ime_prezime, adresa, broj_telefona);
    }

    void RegistrirajNoviLaptop(int ev_broj, std::string naziv, std::string karakteristike) {
        if (laptopi.count(ev_broj))
            throw std::domain_error("Laptop s tim evidencijskim brojem vec postoji");
        laptopi[ev_broj] = new Laptop(ev_broj, naziv, karakteristike);
    }

    Student& NadjiStudenta(int broj_indeksa) {
        ValidirajStudenta(broj_indeksa);
        return *studenti[broj_indeksa];
    }

    const Student& NadjiStudenta(int broj_indeksa) const {
        ValidirajStudenta(broj_indeksa);
        return *studenti.at(broj_indeksa);
    }

    Laptop& NadjiLaptop(int ev_broj) {
        ValidirajLaptop(ev_broj);
        return *laptopi[ev_broj];
    }

    const Laptop& NadjiLaptop(int ev_broj) const {
        ValidirajLaptop(ev_broj);
        return *laptopi.at(ev_broj);
    }

    void IzlistajStudente() const {
        for (const auto &par : studenti) {
            par.second->Ispisi();
            std::cout << "\n";
        }
    }

    void IzlistajLaptope() const {
        for (const auto &par : laptopi) {
            par.second->Ispisi();
            if (par.second->DaLiJeZaduzen()) {
                std::cout << "Zaduzio(la): " << par.second->DajKodKogaJe().DajImePrezime() << " (" << par.second->DajKodKogaJe().DajIndeks() << ")\n";
            }
            std::cout << "\n";
        }
    }

    void ZaduziLaptop(int broj_indeksa, int ev_broj) {
        ValidirajStudenta(broj_indeksa);
        ValidirajLaptop(ev_broj);
        if (laptopi[ev_broj]->DaLiJeZaduzen())
            throw std::domain_error("Laptop vec zaduzen");
        for (const auto &par : laptopi) {
            if (par.second->DaLiJeZaduzen() && par.second->DajKodKogaJe().DajIndeks() == broj_indeksa)
                throw std::domain_error("Student je vec zaduzio laptop");
        }
        laptopi[ev_broj]->Zaduzi(NadjiStudenta(broj_indeksa));
    }

    int NadjiSlobodniLaptop() const {
        for (const auto &par : laptopi) {
            if (!par.second->DaLiJeZaduzen())
            return par.first;
        }
        throw std::domain_error("Nema slobodnih laptopa");
    }

    void RazduziLaptop(int ev_broj) {
        ValidirajLaptop(ev_broj);
        if (!laptopi[ev_broj]->DaLiJeZaduzen())
            throw std::domain_error("Laptop nije zaduzen");
        laptopi[ev_broj]->Razduzi();
    }

    void PrikaziZaduzenja() const {
        bool ima_zaduzenja = false;
        for (const auto &par : laptopi) {
            if (par.second->DaLiJeZaduzen()) {
                std::cout << "Student " << par.second->DajKodKogaJe().DajImePrezime() << " (" << par.second->DajKodKogaJe().DajIndeks() << ") zaduzio/la laptop broj " << par.second->DajEvidencijskiBroj() << "\n";
                ima_zaduzenja = true;
            }
        }
        if (!ima_zaduzenja)
            std::cout << "Nema zaduzenja\n";
    }
};

int main() {
    Administracija administracija;
    while (true) {
        std::cout << "\nOdaberite jednu od sljedecih opcija: "<<std::endl
                  << "1 - RegistrirajNovogStudenta\n"
                  << "2 - RegistrirajNoviLaptop\n"
                  << "3 - IzlistajStudente\n"
                  << "4 - IzlistajLaptope\n"
                  << "5 - NadjiSlobodniLaptop\n"
                  << "6 - ZaduziLaptop\n"
                  << "7 - RazduziLaptop\n"
                  << "8 - PrikaziZaduzenja\n"
                  << "K - Kraj programa\n";
        char izbor;
        std::cin >> izbor;

        switch (izbor) {
            case '1': {
                int indeks;
                std::string godina_studija, ime_prezime, adresa, telefon;
                std::cout << "Unesite broj indeksa: ";
                std::cin >> indeks;
                std::cout << "\nUnesite godinu studija (formata A/X, gdje je A godina studija, a X prima vrijednosti B,M,D, zavisno od studija): ";
                std::cin >> godina_studija;
                std::cin.ignore(10000, '\n');
                std::cout << "\nUnesite ime i prezime studenta: ";
                std::getline(std::cin, ime_prezime);
                std::cout << "\nUnesite adresu studenta: ";
                std::getline(std::cin, adresa);
                std::cout << "\nUnesite broj telefona (formata x/x-x): ";
                std::getline(std::cin, telefon);
                try {
                    administracija.RegistrirajNovogStudenta(indeks, godina_studija, ime_prezime, adresa, telefon);
                } catch (std::domain_error &e) {
                    std::cout << "Izuzetak: " << e.what() << "!" <<"\n";
                }
                break;
            }
            case '2': {
                int ev_broj;
                std::string naziv, karakteristike;
                std::cout << "Unesite evidencijski broj laptopa ";
                std::cin >> ev_broj;
                std::cin.ignore(10000, '\n');
                std::cout << "\nUnesite naziv laptopa ";
                std::getline(std::cin, naziv);
                std::cout << "\nUnesite karakteristike laptopa: ";
                std::getline(std::cin, karakteristike);
                try {
                    administracija.RegistrirajNoviLaptop(ev_broj, naziv, karakteristike);
                    std::cout << "\nLaptop uspjesno registrovan!" << std::endl;
                } catch (std::domain_error &e) {
                    std::cout << "Izuzetak: " << e.what() << "!" << std::endl;
                }
                break;
            }
            case '3':
                administracija.IzlistajStudente();
                break;
            case '4':
                administracija.IzlistajLaptope();
                break;
            case '5': {
                try {
                    int slobodni_laptop = administracija.NadjiSlobodniLaptop();
                    std::cout << "Prvi slobodni laptop ima evidencijski broj: " << slobodni_laptop << "\n";
                } catch (std::domain_error &e) {
                    std::cout << "Izuzetak: " << e.what() << "!" <<"\n";
                }
                break;
            }
            case '6': {
                int indeks, ev_broj;
                std::cout << "Unesite broj indeksa studenta: ";
                std::cin >> indeks;
                std::cout << "Unesite evidencijski broj laptopa: ";
                std::cin >> ev_broj;
                try {
                    administracija.ZaduziLaptop(indeks, ev_broj);
                } catch (std::domain_error &e) {
                    std::cout << "Izuzetak: " << e.what() << "!" <<"\n";
                }
                break;
            }
            case '7': {
                int ev_broj;
                std::cout << "Unesite evidencijski broj laptopa: ";
                std::cin >> ev_broj;
                try {
                    administracija.RazduziLaptop(ev_broj);
                } catch (std::domain_error &e) {
                    std::cout << "Izuzetak: " << e.what() << "!" <<"\n";
                }
                break;
            }
            case '8':
                administracija.PrikaziZaduzenja();
                break;
            case 'K':
            case 'k':
                return 0;
            default:
                std::cout << "Nepostojeca opcija. Molimo pokusajte ponovo.\n";
                break;
        }
    }
    return 0;
}
