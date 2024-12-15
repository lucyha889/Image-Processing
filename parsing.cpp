#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "None of the arguments passed!" << endl;
    }
    else {
        cout << "First argument is: " << argv[1] << endl;
    }
    return 0;
}