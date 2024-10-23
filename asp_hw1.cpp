#include <iostream>
#include <stdexcept>

//zadatak1
template <typename Tip>
class Lista {
public:
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual const Tip& trenutni() const = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip& operator[](int i) = 0;
    virtual const Tip& operator[](int i) const = 0;
};

//zadatak2
template <typename Tip>
class NizLista : public Lista<Tip> {
    Tip* elementi;
    int kapacitet;
    int broj_elemenata;
    int trenutni_index;

    void prosiriNiz() {
        kapacitet *= 2;
        Tip* novi_niz = new Tip[kapacitet];
        for (int i = 0; i < broj_elemenata; i++) {
            novi_niz[i] = elementi[i];
        }
        delete[] elementi;
        elementi = novi_niz;
    }

public:
    NizLista() : kapacitet(10), broj_elemenata(0), trenutni_index(-1) {
        elementi = new Tip[kapacitet];
    }

    ~NizLista() {
        delete[] elementi;
    }

    int brojElemenata() const override {
        return broj_elemenata;
    }

    Tip& trenutni() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        return elementi[trenutni_index];
    }

    const Tip& trenutni() const override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        return elementi[trenutni_index];
    }

    bool prethodni() override {
        if (trenutni_index <= 0) return false;
        trenutni_index--;
        return true;
    }

    bool sljedeci() override {
        if (trenutni_index >= broj_elemenata - 1) return false;
        trenutni_index++;
        return true;
    }

    void pocetak() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        trenutni_index = 0;
    }

    void kraj() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        trenutni_index = broj_elemenata - 1;
    }

    void obrisi() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        for (int i = trenutni_index; i < broj_elemenata - 1; i++) {
            elementi[i] = elementi[i + 1];
        }
        broj_elemenata--;
        if (trenutni_index == broj_elemenata) trenutni_index--;
    }

    void dodajIspred(const Tip& el) override {
        if (broj_elemenata == kapacitet) prosiriNiz();
        if (broj_elemenata == 0) {
            elementi[0] = el;
            trenutni_index = 0;
        } else {
            for (int i = broj_elemenata; i > trenutni_index; i--) {
                elementi[i] = elementi[i - 1];
            }
            elementi[trenutni_index] = el;
            trenutni_index++;
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& el) override {
        if (broj_elemenata == kapacitet) prosiriNiz();
        if (broj_elemenata == 0) {
            elementi[0] = el;
            trenutni_index = 0;
        } else {
            for (int i = broj_elemenata; i > trenutni_index + 1; i--) {
                elementi[i] = elementi[i - 1];
            }
            elementi[trenutni_index + 1] = el;
        }
        broj_elemenata++;
    }

    Tip& operator[](int i) override {
        if (i < 0 || i >= broj_elemenata) throw std::range_error("Neispravan indeks");
        return elementi[i];
    }

    const Tip& operator[](int i) const override {
        if (i < 0 || i >= broj_elemenata) throw std::range_error("Neispravan indeks");
        return elementi[i];
    }
};

//zadatak3
template <typename Tip>
class JednostrukaLista : public Lista<Tip> {
    struct Cvor {
        Tip element;
        Cvor* sljedeci;
        Cvor(const Tip& element, Cvor* sljedeci = nullptr) : element(element), sljedeci(sljedeci) {}
    };
    
    Cvor* pocetak_liste;
    Cvor* kraj_liste;
    Cvor* trenutni_cvor;
    int broj_elemenata;

public:
    JednostrukaLista() : pocetak_liste(nullptr), kraj_liste(nullptr), trenutni_cvor(nullptr), broj_elemenata(0) {}

    ~JednostrukaLista() {
        while (pocetak_liste != nullptr) {
            Cvor* temp = pocetak_liste;
            pocetak_liste = pocetak_liste->sljedeci;
            delete temp;
        }
    }

    int brojElemenata() const override {
        return broj_elemenata;
    }

    Tip& trenutni() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        return trenutni_cvor->element;
    }

    const Tip& trenutni() const override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        return trenutni_cvor->element;
    }

    bool prethodni() override {
        if (trenutni_cvor == pocetak_liste) return false;
        Cvor* temp = pocetak_liste;
        while (temp->sljedeci != trenutni_cvor) {
            temp = temp->sljedeci;
        }
        trenutni_cvor = temp;
        return true;
    }

    bool sljedeci() override {
        if (trenutni_cvor == kraj_liste) return false;
        trenutni_cvor = trenutni_cvor->sljedeci;
        return true;
    }

    void pocetak() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        trenutni_cvor = pocetak_liste;
    }

    void kraj() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        trenutni_cvor = kraj_liste;
    }

    void obrisi() override {
        if (broj_elemenata == 0) throw std::range_error("Lista je prazna");
        if (trenutni_cvor == pocetak_liste) {
            Cvor* temp = pocetak_liste;
            pocetak_liste = pocetak_liste->sljedeci;
            delete temp;
            trenutni_cvor = pocetak_liste;
        } else {
            Cvor* temp = pocetak_liste;
            while (temp->sljedeci != trenutni_cvor) {
                temp = temp->sljedeci;
            }
            temp->sljedeci = trenutni_cvor->sljedeci;
            delete trenutni_cvor;
            trenutni_cvor = temp->sljedeci;
        }
        broj_elemenata--;
        if (broj_elemenata == 0) pocetak_liste = kraj_liste = trenutni_cvor = nullptr;
        else if (trenutni_cvor == nullptr) trenutni_cvor = kraj_liste;
    }

    void dodajIspred(const Tip& el) override {
        if (broj_elemenata == 0) {
            pocetak_liste = kraj_liste = trenutni_cvor = new Cvor(el);
        } else if (trenutni_cvor == pocetak_liste) {
            pocetak_liste = new Cvor(el, pocetak_liste);
        } else {
            Cvor* temp = pocetak_liste;
            while (temp->sljedeci != trenutni_cvor) {
                temp = temp->sljedeci;
            }
            temp->sljedeci = new Cvor(el, trenutni_cvor);
        }
        broj_elemenata++;
    }

    void dodajIza(const Tip& el) override {
        if (broj_elemenata == 0) {
            pocetak_liste = kraj_liste = trenutni_cvor = new Cvor(el);
        } else {
            trenutni_cvor->sljedeci = new Cvor(el, trenutni_cvor->sljedeci);
            if (trenutni_cvor == kraj_liste) kraj_liste = trenutni_cvor->sljedeci;
        }
        broj_elemenata++;
    }

    Tip& operator[](int i) override {
        if (i < 0 || i >= broj_elemenata) throw std::range_error("Neispravan indeks");
        Cvor* temp = pocetak_liste;
        for (int j = 0; j < i; j++) {
            temp = temp->sljedeci;
        }
        return temp->element;
    }

    const Tip& operator[](int i) const override {
        if (i < 0 || i >= broj_elemenata) throw std::range_error("Neispravan indeks");
        Cvor* temp = pocetak_liste;
        for (int j = 0; j < i; j++) {
            temp = temp->sljedeci;
        }
        return temp->element;
    }
};

//testovi
void testirajNizListu() {
    NizLista<int> l;
    l.dodajIza(5);
    l.dodajIza(10);
    l.dodajIspred(1);
    l.pocetak();
    std::cout << l.trenutni() << std::endl; // 1
    l.sljedeci();
    std::cout << l.trenutni() << std::endl; // 5
    l.obrisi();
    std::cout << l.trenutni() << std::endl; // 10
}

void testirajJednostrukuListu() {
    JednostrukaLista<int> l;
    l.dodajIza(5);
    l.dodajIza(10);
    l.dodajIspred(1);
    l.pocetak();
    std::cout << l.trenutni() << std::endl; // 1
    l.sljedeci();
    std::cout << l.trenutni() << std::endl; // 5
    l.obrisi();
    std::cout << l.trenutni() << std::endl; // 10
}

int main() {
    testirajNizListu();
    testirajJednostrukuListu();
    return 0;
}
