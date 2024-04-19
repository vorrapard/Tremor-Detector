#include<mbed.h>
#include<iostream>
using namespace std;

int main() {

    printf("Hello World");
    int i = 0;

    while(1) {
        cout << i << "\n";
        i++;

        // Wait for 1000 milliseconds
        ThisThread::sleep_for(1000ms);
    }
}