//TP 2023/2024: Zadaća 4, Zadatak 4
#include <iostream>
#include <tuple>
#include <stdexcept>
#include <utility>
#include <iomanip>
#include <initializer_list>
#include <algorithm>
#include <vector>
#include <memory>

class Datum {
    int dan, mjesec, godina;
    bool jePrestupna(int godina) const {
      return (godina % 4 == 0 && godina % 100 != 0) || (godina % 400 == 0);
    }

    bool jeValidan(int dan, int mjesec, int godina) const {
      if (godina < 1 || mjesec < 1 || mjesec > 12 || dan < 1)
        return false;
      int daniumjesecu[] = {
          31, jePrestupna(godina) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30,
          31};
      return dan <= daniumjesecu[mjesec - 1];
    }

public:
    Datum(int dan, int mjesec, int godina) {
        Postavi(dan, mjesec, godina);
    }

    void Postavi(int dan, int mjesec, int godina) {
      if (!jeValidan(dan, mjesec, godina))
        throw std::domain_error("Neispravan datum");
      this->dan = dan;
      this->mjesec = mjesec;
      this->godina = godina;
    }

    std::tuple<int, int, int> Ocitaj() const {
        return std::make_tuple(dan, mjesec, godina);
    }

    void Ispisi() const {
        std::cout << dan << "/" << mjesec << "/" << godina;
    }

    bool operator==(const Datum &dr) const {
        return dan == dr.dan && mjesec == dr.mjesec && godina == dr.godina;
    }
};

class Vrijeme {
    int sati, minute;
public:
    Vrijeme(int sati, int minute) {
        Postavi(sati, minute);
    }
    void Postavi(int sati, int minute) {
        if(sati < 0 || sati > 23 || minute < 0 || minute > 59)
        throw std::domain_error("Neispravno vrijeme");
        this->sati = sati;
        this->minute = minute;
    }
    std::pair<int, int> Ocitaj() const {
        return std::make_pair(sati, minute);
    }
    void Ispisi() const {
    std::cout << (sati < 10 ? "0" : "") << sati << ":" << (minute < 10 ? "0" : "") << minute;
    }
};

class Pregled {
    std::string imePacijenta;
    Datum datumPregleda;
    Vrijeme vrijemePregleda;
    bool jePrestupna(int godina) const {
      return (godina % 4 == 0 && godina % 100 != 0) || (godina % 400 == 0);
    }

public:
    Pregled(const std::string &ime, const Datum &datum, const Vrijeme &vrijeme)
        : imePacijenta(ime), datumPregleda(datum), vrijemePregleda(vrijeme) {}
    Pregled(const std::string &ime, int dan, int mjesec, int godina, int sati, int minute)
        : imePacijenta(ime), datumPregleda(dan, mjesec, godina), vrijemePregleda(sati, minute) {}
    Pregled &PromijeniPacijenta(const std::string &ime) {
        imePacijenta = ime;
        return *this;
    }
    Pregled &PromijeniDatum(const Datum &datum) {
        datumPregleda = datum;
        return *this;
    }
    Pregled &PromijeniVrijeme(const Vrijeme &vrijeme) {
        vrijemePregleda = vrijeme;
        return *this;
    }
    void PomjeriDanUnaprijed() {
        int dan, mjesec, godina;
        std::tie(dan, mjesec, godina) = datumPregleda.Ocitaj();
        dan++;
        int daniumjesecu[] = {31, jePrestupna(godina) ? 29 : 28,
                              31, 30,
                              31, 30,
                              31, 31,
                              30, 31,
                              30, 31};
        if (dan > daniumjesecu[mjesec - 1]) {
          dan = 1;
          mjesec++;
          if (mjesec > 12) {
            mjesec = 1;
            godina++;
          }
        }
        datumPregleda.Postavi(dan, mjesec, godina);
    }
    void PomjeriDanUnazad() {
        int dan, mjesec, godina;
        std::tie(dan, mjesec, godina) = datumPregleda.Ocitaj();
        dan--;
        if (dan < 1) {
            mjesec--;
            if (mjesec < 1) {
                mjesec = 12;
                godina--;
            }
            int daniumjesecu[] = {31, jePrestupna(godina) ? 29 : 28,
                                  31, 30,
                                  31, 30,
                                  31, 31,
                                  30, 31,
                                  30, 31};
            dan = daniumjesecu[mjesec - 1];
        }
        datumPregleda.Postavi(dan, mjesec, godina);
    }
    std::string DajImePacijenta() const {
        return imePacijenta;
    }
    Datum DajDatumPregleda() const {
        return datumPregleda;
    }
    Vrijeme DajVrijemePregleda() const {
        return vrijemePregleda;
    }
    static bool DolaziPrije(const Pregled &p1, const Pregled &p2) {
        int dan1, mjesec1, godina1;
        std::tie(dan1, mjesec1, godina1) = p1.DajDatumPregleda().Ocitaj();
        int dan2, mjesec2, godina2;
        std::tie(dan2, mjesec2, godina2) = p2.DajDatumPregleda().Ocitaj();
        if (godina1 < godina2) return true;
        if (godina1 > godina2) return false;
        if (mjesec1 < mjesec2) return true;
        if (mjesec1 > mjesec2) return false;
        if (dan1 < dan2) return true;
        if (dan1 > dan2) return false;
        int sati1, minute1;
        std::tie(sati1, minute1) = p1.DajVrijemePregleda().Ocitaj();
        int sati2, minute2;
        std::tie(sati2, minute2) = p2.DajVrijemePregleda().Ocitaj();
        if (sati1 < sati2) return true;
        if (sati1 > sati2) return false;
        return minute1 < minute2;
    }
    void Ispisi() const {
        std::cout << std::left << std::setw(30) << imePacijenta;
        datumPregleda.Ispisi();
        std::cout << " ";
        vrijemePregleda.Ispisi();
        std::cout << std::endl;
    }
};

class Pregledi {
    std::vector<std::shared_ptr<Pregled>> pregledi;
public:
    Pregledi() = default;

    Pregledi(std::initializer_list<Pregled> lista) {
        for (const auto& pregled : lista) {
            pregledi.push_back(std::make_shared<Pregled>(pregled));
        }
    }

    Pregledi(const Pregledi &dr) {
        for (const auto &pregled : dr.pregledi) {
            pregledi.push_back(std::make_shared<Pregled>(*pregled));
        }
    }

    Pregledi &operator=(const Pregledi &dr) {
        if (this == &dr)
            return *this;
        pregledi.clear();
        for (const auto &pregled : dr.pregledi) {
            pregledi.push_back(std::make_shared<Pregled>(*pregled));
        }
        return *this;
    }

    Pregledi(Pregledi &&dr) noexcept = default;

    Pregledi &operator=(Pregledi &&dr) noexcept {
        if (this != &dr) {
            pregledi = std::move(dr.pregledi);
        }
        return *this;
    }

    void RegistrirajPregled(const Pregled& pregled) {
        pregledi.push_back(std::make_shared<Pregled>(pregled));
    }

    void RegistrirajPregled(std::shared_ptr<Pregled> pregled) {
        pregledi.push_back(std::move(pregled));
    }

    void RegistrirajPregled(const std::string& ime, int dan, int mjesec, int godina, int sati, int minute) {
        if(dan < 1 || dan > 31 || mjesec < 1 || mjesec > 12 || godina < 1 || sati < 0 || sati > 23 || minute < 0 || minute > 59)
            throw std::domain_error("Neispravan datum ili vrijeme");
        pregledi.push_back(std::make_shared<Pregled>(ime, dan, mjesec, godina, sati, minute));
    }

    void RegistrirajPregled(const std::string& ime, const Datum& datum, const Vrijeme& vrijeme) {
        pregledi.push_back(std::make_shared<Pregled>(ime, datum, vrijeme));
    }

    int DajBrojPregleda() const {
        return pregledi.size();
    }

    int DajBrojPregledaNaDatum(const Datum& datum) const {
        return std::count_if(pregledi.begin(), pregledi.end(),
                             [&datum](const std::shared_ptr<Pregled>& pregled) {
                                 return pregled->DajDatumPregleda() == datum;
                             });
    }

    const Pregled& DajNajranijiPregled() const {
        if (pregledi.empty())
            throw std::domain_error("Nema registriranih pregleda");
        return **std::min_element(pregledi.begin(), pregledi.end(),
                                  [](const std::shared_ptr<Pregled>& p1, const std::shared_ptr<Pregled>& p2) {
                                      return Pregled::DolaziPrije(*p1, *p2);
                                  });
    }

    Pregled& DajNajranijiPregled() {
        if (pregledi.empty())
            throw std::domain_error("Nema registriranih pregleda");
        return **std::min_element(pregledi.begin(), pregledi.end(),
                                  [](const std::shared_ptr<Pregled>& p1, const std::shared_ptr<Pregled>& p2) {
                                      return Pregled::DolaziPrije(*p1, *p2);
                                  });
    }

    void IsprazniKolekciju() {
        pregledi.clear();
    }

    void ObrisiNajranijiPregled() {
        if (pregledi.empty())
            throw std::range_error("Prazna kolekcija");
        auto it = std::min_element(pregledi.begin(), pregledi.end(),
                                   [](const std::shared_ptr<Pregled>& p1, const std::shared_ptr<Pregled>& p2) {
                                       return Pregled::DolaziPrije(*p1, *p2);
                                   });
        pregledi.erase(it);
    }

    void ObrisiPregledePacijenta(const std::string& ime) {
        pregledi.erase(
            std::remove_if(pregledi.begin(), pregledi.end(),
                           [&ime](const std::shared_ptr<Pregled>& pregled) {
                               return pregled->DajImePacijenta() == ime;
                           }),
            pregledi.end());
    }

   void IspisiPregledeNaDatum(const Datum& datum) const {
    std::vector<std::shared_ptr<Pregled>> preglediNaDatum;
    for (const auto& pregled : pregledi) {
        if (pregled->DajDatumPregleda() == datum) {
            preglediNaDatum.push_back(pregled);
        }
    }

    std::sort(preglediNaDatum.begin(), preglediNaDatum.end(),
              [](const std::shared_ptr<Pregled>& p1, const std::shared_ptr<Pregled>& p2) {
                  return Pregled::DolaziPrije(*p1, *p2);
              });

    for (const auto& pregled : preglediNaDatum) {
        pregled->Ispisi();
    }
}


    void IspisiSvePreglede() const {
        std::vector<std::shared_ptr<Pregled>> kopija(pregledi.begin(), pregledi.end());
        std::sort(kopija.begin(), kopija.end(),
                  [](const std::shared_ptr<Pregled>& p1, const std::shared_ptr<Pregled>& p2) {
                      return Pregled::DolaziPrije(*p1, *p2);
                  });

        for (const auto& pregled : kopija) {
            pregled->Ispisi();
        }
    }
};

int main() {
    int izbor;
    Pregledi pregledi;
    do {
        std::cout << "1. Registriraj Pregled\n"
                  << "2. Daj Broj Pregleda\n"
                  << "3. Daj Broj Pregleda Na Datum\n"
                  << "4. Daj Najraniji Pregled\n"
                  << "5. Obrisi Najraniji Pregled\n"
                  << "6. Obrisi Preglede Pacijenta\n"
                  << "7. Ispisi Preglede Na Datum\n"
                  << "8. Ispisi Sve Preglede\n"
                  << "9. Isprazni Kolekciju\n"
                  << "10. Izlaz\n";
        std::cout << "Unesite izbor: ";
        std::cin >> izbor;
        switch (izbor) {
           case 1: {
    std::string ime;
    int dan, mjesec, godina, sati, minute;
    std::cout << "Unesite ime pacijenta: ";
    std::cin.ignore(10000, '\n');
    std::getline(std::cin, ime);
    std::cout << "Unesite datum i vrijeme pregleda (dan mjesec godina sati minute): ";
    std::cin >> dan >> mjesec >> godina >> sati >> minute;
    if(dan < 1 || dan > 31 || mjesec < 1 || mjesec > 12 || godina < 1 || sati < 0 || sati > 23 || minute < 0 || minute > 59) {
        std::cout << "Neispravan datum ili vrijeme" << std::endl;
        break;
    }
    try {
        pregledi.RegistrirajPregled(ime, dan, mjesec, godina, sati, minute);
    } catch (std::range_error& e) {
        std::cout << e.what() << std::endl;
    }
    break;
}

            case 2: {
                std::cout << "Ukupan broj pregleda: " << pregledi.DajBrojPregleda() << std::endl;
                break;
            }
            case 3: {
                int dan, mjesec, godina;
                std::cout << "Unesite datum (dan mjesec godina): ";
                std::cin >> dan >> mjesec >> godina;
                Datum datum(dan, mjesec, godina);
                std::cout << "Broj pregleda na datum: " << pregledi.DajBrojPregledaNaDatum(datum) << std::endl;
                break;
            }
            case 4: {
                try {
                    const Pregled& najraniji = pregledi.DajNajranijiPregled();
                    std::cout << "Najraniji pregled je: " << najraniji.DajImePacijenta() << " ";
                    najraniji.DajDatumPregleda().Ispisi();
                    std::cout << " ";
                    najraniji.DajVrijemePregleda().Ispisi();
                    std::cout << std::endl;
                } catch (std::domain_error& e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 5: {
                try {
                    pregledi.ObrisiNajranijiPregled();
                    std::cout << "Najraniji pregled je obrisan." << std::endl;
                } catch (std::range_error& e) {
                    std::cout << e.what() << std::endl;
                }
                break;
            }
            case 6: {
                std::string ime;
                std::cout << "Unesite ime pacijenta: ";
                std::cin.ignore(10000, '\n');
                std::getline(std::cin, ime);
                pregledi.ObrisiPregledePacijenta(ime);
                std::cout << "Pregledi za pacijenta " << ime << " su obrisani." << std::endl;
                break;
            }
            case 7: {
                int dan, mjesec, godina;
                std::cout << "Unesite datum: ";
                std::cin >> dan >> mjesec >> godina;
                Datum datum(dan, mjesec, godina);
                pregledi.IspisiPregledeNaDatum(datum);
                break;
            }
            case 8: {
                std::cout << "Svi pregledi:" << std::endl;
                pregledi.IspisiSvePreglede();
                break;
            }
            case 9: {
                pregledi.IsprazniKolekciju();
                std::cout << "Kolekcija ispraznjena." << std::endl;
                break;
            }
            case 10: {
                std::cout << "Kraj programa." << std::endl;
                break;
            }
            default:
                std::cout << "Nepostojeca opcija." << std::endl;
        }
    } while (izbor != 10);

    return 0;
}