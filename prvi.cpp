#include <iostream>
#include <string>

int main() {
    std::string ime;
    std::cout << "Kako se zoves? ";
    std::getline(std::cin, ime);
    std::cout << "Jao, kako ti je lijepo ime " << ime << " :)";
    return 0;
}