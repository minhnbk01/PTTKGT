#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    srand(time(0));
    ofstream fout("input.txt");

    int n1 = 60, C1 = 1000;
    fout << n1 << " " << C1 << "\n";
    for(int i = 0; i < n1; i++) fout << (rand() % C1 + 1) << " ";
    fout << "\n";
    int n2 = 80, C2 = 100;
    fout << n2 << " " << C2 << "\n";
    for(int i = 0; i < n2; i++) fout << (C2 / 2 + 1 + rand() % (C2 / 2 - 1)) << " ";
    fout << "\n";
    int n3 = 80, C3 = 500;
    fout << n3 << " " << C3 << "\n";
    for(int i = 0; i < n3; i++) fout << (rand() % 10 + 1) << " ";
    fout << "\n";
    int n4 = 50, C4 = 20000;
    fout << n4 << " " << C4 << "\n";
    for(int i = 0; i < n4; i++) fout << (rand() % 5000 + 1) << " ";
    fout << "\n";
    int n5 = 60, C5 = 100;
    fout << n5 << " " << C5 << "\n";
    for(int i = 0; i < n5; i++) {
        if (i % 3 == 0) fout << 51 << " ";
        else if (i % 3 == 1) fout << 26 << " ";
        else fout << 24 << " ";
    }
    fout << "\n";

    fout.close();
    cout << "Da tao thanh cong!\n";
    return 0;
}
