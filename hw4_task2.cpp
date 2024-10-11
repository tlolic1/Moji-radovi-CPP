//TP 2023/2024: Zadaća 4, Zadatak 2
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>

class GradjaninBiH {
public:
    enum Pol { Musko, Zensko };
private:
    std::string ime_i_prezime;
    long long int jmbg;
    Pol pol;
    static GradjaninBiH* posljednji_gradjanin;
    GradjaninBiH* prethodni_gradjanin;
    static bool DaLiJeIspravanDatum(int dan, int mjesec, int godina) {
        int dani_u_mjesecu[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if ((godina % 4 == 0 && godina % 100 != 0) || godina % 400 == 0)
            dani_u_mjesecu[1]++;
        return dan > 0 && mjesec > 0 && mjesec <= 12 && dan <= dani_u_mjesecu[mjesec - 1] && godina<=2021;
    }
    static int IzracunajKontrolnuCifru(const long long int jmbg) {
        int c[13];
        long long int temp = jmbg;
        for (int i = 12; i >= 0; i--) {
            c[i] = temp % 10;
            temp /= 10;
        }
        int kontrolna_cifra = 11 - (7 * (c[0] + c[6]) + 6 * (c[1] + c[7]) + 5 * (c[2] + c[8]) +
        4 * (c[3] + c[9]) + 3 * (c[4] + c[10]) + 2 * (c[5] + c[11])) % 11;
        if (kontrolna_cifra == 11) return 0;
        if (kontrolna_cifra == 10) return -1;
        return kontrolna_cifra;
    }
    void ValidirajJMBG(long long int jmbg) {
        int c[13];
        long long int temp = jmbg;
        for (int i = 12; i >= 0; i--) {
            c[i] = temp % 10;
            temp /= 10;
        }
        int dan = c[0] * 10 + c[1];
        int mjesec = c[2] * 10 + c[3];
        int godina = c[4] * 100 + c[5] * 10 + c[6];
        if (godina < 100) godina += (godina < 21) ? 2000 : 1900;
        if (!DaLiJeIspravanDatum(dan, mjesec, godina))
            throw std::logic_error("JMBG nije validan");
        if (IzracunajKontrolnuCifru(jmbg) != c[12])
            throw std::logic_error("JMBG nije validan");
        for (GradjaninBiH* p = posljednji_gradjanin; p != nullptr; p = p->prethodni_gradjanin) {
            if (p->jmbg == jmbg)
                throw std::logic_error("Vec postoji gradjanin sa istim JMBG");
        }
    }
    static long long int KreirajJMBG(int dan, int mjesec, int godina, int sifra_regije, Pol pol, int kod) {
        int godina_skracena = godina % 1000;
        long long int jmbg = dan * 100000000000LL + mjesec * 1000000000LL + godina_skracena * 1000000LL + sifra_regije * 10000LL + kod * 10LL;
        int kontrolna_cifra = IzracunajKontrolnuCifru(jmbg);
        if (kontrolna_cifra == -1)
            throw std::logic_error("Neispravni podaci");
        return jmbg + kontrolna_cifra;
    }
    static int NadjiSlobodanKod(int dan, int mjesec, int godina, int sifra_regije, Pol pol) {
        int kod_pocetak = (pol == Musko) ? 0 : 500;
        int kod_kraj = (pol == Musko) ? 499 : 999;
        for (int kod = kod_pocetak; kod <= kod_kraj; kod++) {
            bool zauzet = false;
            long long int moguci_jmbg = KreirajJMBG(dan, mjesec, godina, sifra_regije, pol, kod);
            for (GradjaninBiH* p = posljednji_gradjanin; p != nullptr; p = p->prethodni_gradjanin) {
                if (p->jmbg == moguci_jmbg) {
                    zauzet = true;
                    break;
                }
            }
            if (!zauzet) return kod;
        }
        throw std::logic_error("Neispravni podaci");
    }

public:
    GradjaninBiH(const std::string& ime_i_prezime, long long int jmbg) : ime_i_prezime(ime_i_prezime) {
        ValidirajJMBG(jmbg);
        this->jmbg = jmbg;
        this->prethodni_gradjanin = posljednji_gradjanin;
        posljednji_gradjanin = this;

        int c10 = (jmbg / 1000) % 10;
        this->pol = (c10 < 5) ? Musko : Zensko;
    }
    GradjaninBiH(const std::string& ime_i_prezime, int dan_rodjenja, int mjesec_rodjenja, int godina_rodjenja, int sifra_regije, Pol pol)
        : ime_i_prezime(ime_i_prezime), pol(pol) {
        if (!DaLiJeIspravanDatum(dan_rodjenja, mjesec_rodjenja, godina_rodjenja) || sifra_regije < 0 || sifra_regije > 99)
        throw std::logic_error("Neispravni podaci");
        int kod = NadjiSlobodanKod(dan_rodjenja, mjesec_rodjenja, godina_rodjenja, sifra_regije, pol);
        this->jmbg = KreirajJMBG(dan_rodjenja, mjesec_rodjenja, godina_rodjenja, sifra_regije, pol, kod);

        this->prethodni_gradjanin = posljednji_gradjanin;
        posljednji_gradjanin = this;
    }
    ~GradjaninBiH() {
        if (this == posljednji_gradjanin) {
            posljednji_gradjanin = this->prethodni_gradjanin;
        } else {
            for (GradjaninBiH* p = posljednji_gradjanin; p != nullptr; p = p->prethodni_gradjanin) {
                if (p->prethodni_gradjanin == this) {
                    p->prethodni_gradjanin = this->prethodni_gradjanin;
                    break;
                }
            }
        }
    }
    std::string DajImeIPrezime() const { return ime_i_prezime; }
    long long int DajJMBG() const { return jmbg; }
    int DajDanRodjenja() const { return (jmbg / 100000000000LL) % 100; }
    int DajMjesecRodjenja() const { return (jmbg / 1000000000LL) % 100; }
    int DajGodinuRodjenja() const {
    int godina = (jmbg / 1000000LL) % 100;
    godina += (godina < 100) ? ((godina < 21) ? 2000 : 1900) : 0;
    return godina;
}
    int DajSifruRegije() const { return (jmbg / 10000) % 100; }
    Pol DajPol() const { return pol; }
    void PromijeniImeIPrezime(const std::string& novo_ime_i_prezime) { ime_i_prezime = novo_ime_i_prezime; }
    GradjaninBiH(const GradjaninBiH&) = delete;
    GradjaninBiH& operator=(const GradjaninBiH&) = delete;
};
GradjaninBiH* GradjaninBiH::posljednji_gradjanin = nullptr;

int main() {
    int broj_gradjana;
    std::cout << "Unesite broj gradjana: ";
    std::cin >> broj_gradjana;
    std::vector<std::unique_ptr<GradjaninBiH>> gradjani;
    for (int i = 0; i < broj_gradjana; i++) {
        std::string ime_i_prezime;
        int dan, mjesec, godina, sifra_regije, pol_int;
        GradjaninBiH::Pol pol;
        std::cout << "\nUnesite podatke za " << i + 1 << ". gradjanina:\n";
        std::cout << "Ime i prezime: ";
        std::cin.ignore(10000, '\n');
        std::getline(std::cin, ime_i_prezime);
        std::cout << "Datum rodjenja: ";
        std::cin >> dan >> mjesec >> godina;
        std::cout << "Sifra regije: ";
        std::cin >> sifra_regije;
        std::cout << "Pol ( 1-Musko, 2-Zensko ): \n";
        std::cin >> pol_int;
        pol = (pol_int == 1) ? GradjaninBiH::Musko : GradjaninBiH::Zensko;
        try {
            gradjani.push_back(std::make_unique<GradjaninBiH>(ime_i_prezime, dan, mjesec, godina, sifra_regije, pol));
        } catch (const std::logic_error& e) {
            std::cout << e.what()<<". Molimo ponovite unos."<< std::endl;
            i--;
        }
    }
    std::sort(gradjani.begin(), gradjani.end(), [](const std::unique_ptr<GradjaninBiH>& a, const std::unique_ptr<GradjaninBiH>& b) {
        if (a->DajGodinuRodjenja() != b->DajGodinuRodjenja())
            return a->DajGodinuRodjenja() < b->DajGodinuRodjenja();
        if (a->DajMjesecRodjenja() != b->DajMjesecRodjenja())
            return a->DajMjesecRodjenja() < b->DajMjesecRodjenja();
        return a->DajDanRodjenja() < b->DajDanRodjenja();
    });
    std::cout << "\nGradjani sortirani po starosti:\n";
    for (const auto& gradjanin : gradjani) {
        std::cout << gradjanin->DajImeIPrezime() << " "
                  << gradjanin->DajDanRodjenja() << "." 
                  << gradjanin->DajMjesecRodjenja() << "." 
                  << gradjanin->DajGodinuRodjenja() << " JMBG: " 
                  << gradjanin->DajJMBG() << std::endl;
    }
    return 0;
}