#include <iostream>
using namespace std;

int main()
{
    int d1, d2, d3, d4;

    cout << "Enter 4 data bits (0 or 1): ";
    cin >> d1 >> d2 >> d3 >> d4;

    // Positions:
    // 1 2 3 4 5 6 7
    // P1 P2 D1 P4 D2 D3 D4

    int h[8];

    h[3] = d1;
    h[5] = d2;
    h[6] = d3;
    h[7] = d4;

    // Even parity bits
    h[1] = h[3] ^ h[5] ^ h[7];
    h[2] = h[3] ^ h[6] ^ h[7];
    h[4] = h[5] ^ h[6] ^ h[7];

    cout << "\n7-bit Hamming Code (Even Parity): ";
    for(int i = 1; i <= 7; i++)
        cout << h[i] << " ";

    // Receiver side
    int r[8];

    for(int i = 1; i <= 7; i++)
        r[i] = h[i];

    int flip;

    cout << "\n\nReceiver Side";
    cout << "\nEnter bit position to flip (1-7), 0 for no error: ";
    cin >> flip;

    if(flip >= 1 && flip <= 7)
        r[flip] = !r[flip];

    cout << "\nReceived Code: ";
    for(int i = 1; i <= 7; i++)
        cout << r[i] << " ";

    // Syndrome calculation
    int s1 = r[1] ^ r[3] ^ r[5] ^ r[7];
    int s2 = r[2] ^ r[3] ^ r[6] ^ r[7];
    int s4 = r[4] ^ r[5] ^ r[6] ^ r[7];

    int error = s4 * 4 + s2 * 2 + s1;

    cout << "\n\nSyndrome Bits:";
    cout << "\nS1 = " << s1;
    cout << "\nS2 = " << s2;
    cout << "\nS4 = " << s4;

    if(error == 0)
    {
        cout << "\nNo Error Detected.";
    }
    else
    {
        cout << "\nError detected at Bit Position = " << error;

        // Correct the error
        r[error] = !r[error];

        cout << "\nCorrected Code: ";
        for(int i = 1; i <= 7; i++)
            cout << r[i] << " ";
    }

    return 0;
}
