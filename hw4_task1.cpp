//TP 2023/2024: Zadaća 4, Zadatak 1
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cmath>
#include <vector>

typedef std::pair<double, double> Tacka;

enum Pozicija {GoreLijevo, GoreDesno, DoljeLijevo, DoljeDesno};
enum Smjer {Nalijevo, Nadesno};

class Pravougaonik {
    Tacka t1, t2;
    static bool DaLiSePresijecaju(double min1, double max1, double min2, double max2) {
    return std::max(min1, min2) <= std::min(max1, max2);
    }
public:
    Pravougaonik(const Tacka &t1, const Tacka &t2) : t1(t1), t2(t2) {}
    void Postavi(const Tacka &t1, const Tacka &t2) {
        this->t1 = t1;
        this->t2 = t2;
    }
  void Postavi(Pozicija p, const Tacka &t) {
    switch (p) {
        case GoreLijevo:
            t1 = Tacka(std::min(t.first, t2.first), std::max(t.second, t2.second));
            break;
        case GoreDesno:
            t1 = Tacka(std::max(t.first, t1.first), std::max(t.second, t2.second));
            break;
        case DoljeLijevo:
            t2 = Tacka(std::min(t.first, t2.first), std::min(t.second, t1.second));
            break;
        case DoljeDesno:
            t2 = Tacka(std::max(t.first, t1.first), std::min(t.second, t1.second));
            break;
    }
}
    void Centriraj(const Tacka &t) {
        double dx = t.first - DajCentar().first;
        double dy = t.second - DajCentar().second;
        Transliraj(dx, dy);
    }
    Tacka DajTjeme(Pozicija p) const {
        switch (p) {
            case GoreLijevo: return Tacka(std::min(t1.first, t2.first), std::max(t1.second, t2.second));
            case GoreDesno: return Tacka(std::max(t1.first, t2.first), std::max(t1.second, t2.second));
            case DoljeLijevo: return Tacka(std::min(t1.first, t2.first), std::min(t1.second, t2.second));
            case DoljeDesno: return Tacka(std::max(t1.first, t2.first), std::min(t1.second, t2.second));
        }
        return Tacka(0, 0);
    }
   Tacka DajCentar() const {
    double x = (t1.first + t2.first) / 2;
    double y = (t1.second + t2.second) / 2;
    return Tacka((std::abs(x) < 0.00001 ? 0 : x), (std::abs(y) < 0.00001 ? 0 : y));
}
    double DajHorizontalnu() const {
        return std::fabs(t1.first - t2.first);
    }
    double DajVertikalnu() const {
        return std::fabs(t1.second - t2.second);
    }
    double DajObim() const {
        return 2 * (DajHorizontalnu() + DajVertikalnu());
    }
    double DajPovrsinu() const {
        return DajHorizontalnu() * DajVertikalnu();
    }
    static Pravougaonik Presjek(const Pravougaonik &p1, const Pravougaonik &p2) {
        double min_x = std::max(std::min(p1.t1.first, p1.t2.first), std::min(p2.t1.first, p2.t2.first));
        double max_x = std::min(std::max(p1.t1.first, p1.t2.first), std::max(p2.t1.first, p2.t2.first));
        double min_y = std::max(std::min(p1.t1.second, p1.t2.second), std::min(p2.t1.second, p2.t2.second));
        double max_y = std::min(std::max(p1.t1.second, p1.t2.second), std::max(p2.t1.second, p2.t2.second));
        if (min_x > max_x || min_y > max_y) {
            throw std::domain_error("Pravougaonici se ne presjecaju");
        }
        return Pravougaonik(Tacka(min_x, max_y), Tacka(max_x, min_y));
    }
    void Transliraj(double delta_x, double delta_y) {
        t1.first += delta_x;
        t1.second += delta_y;
        t2.first += delta_x;
        t2.second += delta_y;
    }
    void Rotiraj(const Tacka &t, Smjer s) {
        auto rotirajTacku = [](Tacka &tacka, const Tacka &centar, Smjer s) {
            double x = tacka.first - centar.first;
            double y = tacka.second - centar.second;
            if (s == Nalijevo) {
                tacka.first = centar.first - y;
                tacka.second = centar.second + x;
            } else {
                tacka.first = centar.first + y;
                tacka.second = centar.second - x;
            }
        };
        rotirajTacku(t1, t, s);
        rotirajTacku(t2, t, s);
    }
    void Ispisi() const {
    std::cout << "{{" << std::min(t1.first, t2.first) << "," << std::max(t1.second, t2.second) << "},{"
                  << std::max(t1.first, t2.first) << "," << std::min(t1.second, t2.second) << "}}";
    }
    friend bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2);
    friend bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2);
};

bool DaLiSePoklapaju(const Pravougaonik &p1, const Pravougaonik &p2) {
    return (std::min(p1.t1.first, p1.t2.first) == std::min(p2.t1.first, p2.t2.first) &&
            std::max(p1.t1.first, p1.t2.first) == std::max(p2.t1.first, p2.t2.first) &&
            std::min(p1.t1.second, p1.t2.second) == std::min(p2.t1.second, p2.t2.second) &&
            std::max(p1.t1.second, p1.t2.second) == std::max(p2.t1.second, p2.t2.second));
}

bool DaLiSuPodudarni(const Pravougaonik &p1, const Pravougaonik &p2) {
    return (p1.DajHorizontalnu() == p2.DajHorizontalnu() && p1.DajVertikalnu() == p2.DajVertikalnu()) ||
           (p1.DajHorizontalnu() == p2.DajVertikalnu() && p1.DajVertikalnu() == p2.DajHorizontalnu());
}

bool DaLiSuSlicni(const Pravougaonik &p1, const Pravougaonik &p2) {
    if ((p1.DajHorizontalnu() == 0 && p1.DajVertikalnu() == 0) ||
        (p2.DajHorizontalnu() == 0 && p2.DajVertikalnu() == 0)) {
        return true;
    }  
    double odnos1 = p1.DajHorizontalnu() / p1.DajVertikalnu();
    double odnos2 = p2.DajHorizontalnu() / p2.DajVertikalnu();
    double obrnuti_odnos1 = p1.DajVertikalnu() / p1.DajHorizontalnu();
    double obrnuti_odnos2 = p2.DajVertikalnu() / p2.DajHorizontalnu();
    return (std::abs(odnos1 - odnos2) < 0.00001 || std::abs(odnos1 - obrnuti_odnos2) < 0.00001 ||
    std::abs(obrnuti_odnos1 - odnos2) < 0.00001 || std::abs(obrnuti_odnos1 - obrnuti_odnos2) < 0.00001);
}

int main() {
    int n;
    std::cout << "Unesite n: ";
    std::cin >> n;
    Pravougaonik** pravougaonici = new Pravougaonik*[n];
    for (int i = 0; i < n; i++) {
        double x1, y1, x2, y2;
        std::cout << "Unesite 1. tjeme pravougaonika " << i + 1 << ": ";
        std::cin >> x1 >> y1;
        std::cout << "Unesite 2. tjeme pravougaonika " << i + 1 << ": ";
        std::cin >> x2 >> y2;
        pravougaonici[i] = new Pravougaonik(Tacka(x1, y1), Tacka(x2, y2));
    }
    std::cout << "Unesite podatke za transliranje (dx dy): ";
    std::vector<std::pair<double, double>> translacije;
    for (int i = 0; i < n; i++) {
        double dx, dy;
        std::cin >> dx >> dy;
        translacije.push_back(std::make_pair(dx, dy));
    }
    std::transform(pravougaonici, pravougaonici + n, translacije.begin(), pravougaonici, 
                   [](Pravougaonik* p, const std::pair<double, double>& translacija) {
                    p->Transliraj(translacija.first, translacija.second);
                       return p;
                   });
    std::for_each(pravougaonici, pravougaonici + n, [](Pravougaonik* p) {
        Tacka centar = p->DajCentar();
        p->Rotiraj(centar, Nalijevo);
    });
    std::cout << "Pravougaonici, nakon transformacija:" << std::endl;
    std::for_each(pravougaonici, pravougaonici + n, [](Pravougaonik* p) {
        p->Ispisi();
        std::cout << std::endl;
    });
    auto max_povrsina_pravougaonik = std::max_element(pravougaonici, pravougaonici + n, [](Pravougaonik* p1, Pravougaonik* p2) {
        return p1->DajPovrsinu() < p2->DajPovrsinu(); 
    });
    std::cout << "Pravougaonik s najvecom povrsinom: ";
    (*max_povrsina_pravougaonik)->Ispisi(); 
    std::cout << std::endl;
    for (int i = 0; i < n; i++) {
        delete pravougaonici[i];
    }
    delete[] pravougaonici;
    return 0;
}
