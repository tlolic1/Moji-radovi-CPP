// TP 2023/2024: Zadaća 3, Zadatak 1
#include <vector>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <functional>
typedef std::vector<std::pair<double, double>> vektor_cvorova;
typedef std::pair<double, double> dabl_par;
std::function<double(double)> KubnaInterpolacija(const vektor_cvorova cvorovi) {
  int sajz = cvorovi.size();
  if (sajz < 4)
  throw std::domain_error("Nedovoljan broj cvorova");
  for (int i = 0; i <= sajz - 2; i++) {
  if (cvorovi.at(i).first == cvorovi.at(i + 1).first)
  throw std::domain_error("Neispravni cvorovi");
  } //izuzeci
  auto sortirano = cvorovi;
  std::sort(sortirano.begin(), sortirano.end(), [](dabl_par x, dabl_par y) {
            return x.first < y.first;
            });
  return [sajz,cvorovi](double lol) {
    
    if (lol < cvorovi.at(0).first || lol > cvorovi.at(sajz - 1).first) {
      throw std::range_error("Argument izvan opsega!");
    } //izuzeci
    if (lol == cvorovi.at(sajz - 1).first)
      return cvorovi.at(sajz - 1).second;
    int i=0;
    for (int j = 0; j <= sajz - 2; j++) {
      if (cvorovi.at(j).first <= lol && cvorovi.at(j + 1).first > lol) {
        i = j;
        break;
      }
    }
    if (i == 0)
      i++;
    if (i == sajz - 2)
      i--;
    double zbir = 0., proizvod;
    for (int k = i - 1; k < i + 3; k++) {
      proizvod = 1.;
      for (int j = i - 1; j < i + 3; j++) {
        if (k != j)
          proizvod *= (lol - cvorovi.at(j).first) / (cvorovi.at(k).first - cvorovi.at(j).first);
      }
      zbir += cvorovi.at(k).second * proizvod;
    }
    return zbir;
  };
}

std::function<double(double)> KubnaInterpolacija(std::function<double(double)> f, double poc, double kraj, double del) {
  if (poc >= kraj || del <= 0)
    throw std::domain_error("Nekorektni parametri"); //izuzeci
  vektor_cvorova cvorovi;
  for (double x = poc; x <= kraj; x += del) {
    cvorovi.push_back({x, f(x)});
  }
  return KubnaInterpolacija(cvorovi);
}

int main() {
  int odabir;
  std::cout << "Odaberite opciju (1 - unos cvorova, 2 - aproksimacija): ";
  std::cin >> odabir;
  try {
  if(odabir == 1) {
      int broj_cvorova;
      std::cout << "Unesite broj cvorova: ";
      std::cin >> broj_cvorova;
      vektor_cvorova cvorovi;
      std::cout << "Unesite cvorove kao parove x y: ";
      for(int i = 0; i <= broj_cvorova - 1; i++){
          double x,y;
          std::cin >> x >> y;
          cvorovi.push_back({x,y});
      }
      auto funkcija = KubnaInterpolacija(cvorovi);
      double argument;
      std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
      while(std::cin >> argument){
        try{
            double rezultat = funkcija(argument);
            std::cout << "f(" << argument << ") = " << rezultat << std::endl;
          } catch(std::range_error& e){
            std::cout << e.what() << std::endl;
          }
          std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
      }
  }
  else if(odabir == 2){
      double pocetak, kraj, delta;
      std::cout << "Unesite krajeve intervala i korak: ";
      std::cin >> pocetak >> kraj >> delta;
      auto funkcija = [](double x) {return x*x + std::sin(x) + std::log(x + 1);};
      auto funkcija_approx = KubnaInterpolacija(funkcija,pocetak,kraj,delta);
      double ar;
      std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
      while(std::cin >> ar){
          try{
              double rezultat = funkcija(ar);
              double aproks;
              try{
                  aproks = funkcija_approx(ar);
                  std::cout << "f(" << ar << ") = " << rezultat << " fapprox(" << ar << ") = " << aproks << std::endl; }
  catch(std::range_error &e){ std::cout << e.what()<<std::endl;
              }
          } catch(std::range_error &e){
              std::cout << e.what()<<std::endl;
          }
          std::cout << "Unesite argument (ili \"kraj\" za kraj): ";
      }
  }
  else std::cout << "Neispravna opcija!"<< std::endl;
  } catch (std::domain_error &e) {
      std::cout << e.what();
  }
  return 0;
}




