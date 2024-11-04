#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 
#include <random>    
#include <ctime>    

using namespace std;

struct Question {
    string question;
    vector<string> answers;
    int correctAnswer;
};

class Milijunas {
private:
    vector<Question> questions;
    vector<int> prizeMoney = {100, 200, 300, 500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 125000, 250000, 500000, 1000000};
    int currentQuestion;
    int money;
    bool used5050;
    bool usedFriend;
    bool usedAudience;

public:
    Milijunas() : currentQuestion(0), money(0), used5050(false), usedFriend(false), usedAudience(false) {
        // Initialize questions
        questions = {
            {"Koje od ovih imena je zapravo i prezime jednog od profesora na ETF-u?", {"Vladimir", "Mirza", "Hamza", "Nedis"}, 2},
            {"Koja je rijec najcesce upotrijebljena na IM1 i IM2?", {"Integral", "Okolina", "Skup", "Imas 1 u 3 sansu, nije ovo"}, 1},
            {"Sta su zidari?", {"Specificni tip konstruktora", "Funkcije iz biblioteke <wall>", "Vrsta operatora", "Vrsta gradjevinskih radnika"}, 3},
            {"Kako se zove trkaca igrica na Steleksovom plejstejsnu?", {"Mario Kart", "Crash Team Racing", "Starlit Kart Racing", "Fallout 3: Game of The Year Edition"}, 2},
            {"U kojoj je drzavi osnovan EESTEC(nisu me platili da ovo postavim kao pitanje)?", {"Holandija", "Juzni Sudan", "Svicarska", "Austrija"}, 0},
            {"Koja od opcija predstavlja igricu prof. Samira Ribica?", {"Round-Robin", "EX-YU game", "Na balkanu nista novo", "Vodic za rjesavanje kvadratnih kongruencija"}, 2},
            {"Koja od od ponudjenih vrsta hrane se mogla uzeti u tzv. menzi ETF-a(bivsoj) prekoputa glavne zgrade?", {"Cijela kokosh", "Pizza na komad", "Losos", "Burek"}, 1},
            {"Koja je zadnja rijec u nazivu famoznog drajva Spas za neradnike?", {"galaksiji", "ikad", "BiH", "Vakuf"}, 3},
            {"Koja je pjesma muzicka podloga za spremanje DM-a oblast kombinatorika?", {"Jelena Rozga - Dobitna kombinacija", "Travis Scott - FE!N", "A realno ovo je pitanje zicer", "Zeljko Juric - Cener"}, 0},
            {"Our goal is to empower young professionals to lead this transformative journey. Koga sam ovdje citirao?", {"Franca Kafku", "Dijanu, princezu od Velsa", "Elona Muska", "Ko god je pravio DSAI ETF stranicu, e njega"}, 3},
            {"Kada je prof Mirza Hamza osvojio steleksov kviz, kako mu se zvala ekipa?", {"Profesori", "GLHF", "Mirzino jato", "Hamzino jato"}, 2},
            {"Koliko kosta kafica u Cue?", {"3 Eura", "2,5 KM", "Dzaba je", "Fuzzy logika"}, 1},
            {"Koji windows je bio operativni sistem pikada koji je sklonjen iz Steleksa(da, ovo pitanje ima smisla)?", {"Windows XP", "Windows 11", "Linux", "Windows 7"}, 3},
            {"Kako se zove ulica u kojoj se nalazi ETF?", {"Bosanska", "Dzemala Bijedica", "Zmaja od Bosne", "Wall street"}, 2},
            {"Koliko cuka stoji bez razloga svezano na kampusu UNSA?", {"0", "2", "1,5", "std::string"}, 1},
            {"Koji od ponudjenih dokumenata se ne moze naci u okviru predmeta TP?", {"Sve sto treba znati o pokazivacima, a sto vam mama nije rekla", "Da li je } kraj", "Cilega zivota ja san tija samo konstruktor", "Kako je propalo Rimsko carstvo"}, 2},
            {"Koje je godiste Gilbert Strang?", {"1934.", "1944.", "2002.", "1954."}, 0},
            {"Ako ubacis marku, koliko vremena dobijes na Steleksovom PS5?", {"10 min", "12 min", "sat i po", "15 m/s"}, 1},
            //nastavi
            {"Koji je najveca riba na svijetu?", {"Velika bijela ajkula", "Kitopsina", "Morski pas", "Tuna"}, 1},
            {"Koji je najveca zmija na svijetu?", {"Pitona", "Boa", "Anakonda", "Kobra"}, 2},
            {"Koji je najveca zaba na svijetu?", {"Goliath zaba", "Bullfrog", "Tree frog", "Poison dart frog"}, 0},
            {"Koji je najveca kornjaca na svijetu?", {"Galapagos kornjaca", "Leatherback kornjaca", "Green sea kornjaca", "Hawksbill kornjaca"}, 1},
            {"Koji je najveca zivotinja na kopnu?", {"Slon", "Nosorog", "Bivol", "Giraffe"}, 0},
            {"Koji je najveca zivotinja u vodi?", {"Plavi kit", "Delfin", "Morski pas", "Orca"}, 0},
            {"Koji je najveca ptica koja ne leti?", {"Noj", "Emu", "Kivi", "Pingvin"}, 0},
            {"Koji je najveca zivotinja u Africi?", {"Slon", "Nosorog", "Lav", "Bivol"}, 0},
            {"Koji je najveca zivotinja u Aziji?", {"Tigar", "Slon", "Nosorog", "Panda"}, 1},
            {"Koji je najveca zivotinja u Australiji?", {"Kangaroo", "Koala", "Emu", "Dingo"}, 0},
            {"Koji je najveca zivotinja u Evropi?", {"Medvjed", "Vuk", "Jelen", "Bivol"}, 0}
        };
        // Shuffle all questions and select the first 15
        auto rng = default_random_engine(time(0));
        shuffle(questions.begin(), questions.end(), rng);
        questions.resize(15); // Keep only the first 15 questions
    }

    void startGame() {
        // Display introductory dialog
        cout << "Dobrodosli na ETF milijunash! Malo ce vam potesko biti preci ovo ako niste student ETF-a ali nemojte se sikirati." << endl << "Da zapocnete kviz pritisnite bilo koje slovo: ";
        char start;
        cin >> start;

        for (int round = 0; round < 15; ++round) {
            currentQuestion = round; // Select the next question in the shuffled list

            displayQuestion();
            int choice = getPlayerChoice();
            if (choice == -1) {
                cout << "Nema odustajanja, zar si tako i na ETF-u odustao" << endl;
                return;
            } else if (choice == questions[currentQuestion].correctAnswer) {
                money = prizeMoney[round];
                cout << "Tacan odgovor! Trenutno imate " << money << " eura." << endl;
            } else {
                cout << "Netacan odgovor. Izgubili ste sve osvojeni kesh." << endl;
                return;
            }
        }
        cout << "Cestitamo! Osvojili ste 1.000.000 eura!" << endl;
    }

    void displayQuestion() {
        cout << questions[currentQuestion].question << endl;
        for (int i = 0; i < questions[currentQuestion].answers.size(); i++) {
            cout << i + 1 << ". " << questions[currentQuestion].answers[i] << endl;
        }
    }

    int getPlayerChoice() {
        int choice;
        cout << "Unesite broj odgovora (ili 0 za odustajanje): ";
        cin >> choice;
        return choice - 1;
    }
};

int main() {
    Milijunas game;
    game.startGame();
    return 0;
}