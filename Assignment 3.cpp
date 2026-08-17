#include <iostream>
using namespace std;

void ip_address() {
    int a, b, c, d;
    char dot1, dot2, dot3;

    cout << "\nEnter IP Address: ";
    cin >> a >> dot1 >> b >> dot2 >> c >> dot3 >> d;

    if (a >= 1 && a <= 126) {
        cout << "\nIP Address belongs to Class A\n";
        cout << "Range\t\t\t: 1 - 126\n";
        cout << "Default Subnet Mask\t: 255.0.0.0\n";
        cout << "Hosts per Network\t: 2^24 - 2 (16,777,214)\n";
        cout << "Number of Networks\t: 2^7 (126)\n";
    }
    else if (a >= 128 && a <= 191) {
        cout << "\nIP Address belongs to Class B\n";
        cout << "Range\t\t\t: 128 - 191\n";
        cout << "Default Subnet Mask\t: 255.255.0.0\n";
        cout << "Hosts per Network\t: 2^16 - 2 (65,534)\n";
        cout << "Number of Networks\t: 2^14 (16,384)\n";
    }
    else if (a >= 192 && a <= 223) {
        cout << "\nIP Address belongs to Class C\n";
        cout << "Range\t\t\t: 192 - 223\n";
        cout << "Default Subnet Mask\t: 255.255.255.0\n";
        cout << "Hosts per Network\t: 2^8 - 2 (254)\n";
        cout << "Number of Networks\t: 2^21 (2,097,152)\n";
    }
    else if (a >= 224 && a <= 239) {
        cout << "\nIP Address belongs to Class D (Multicast)\n";
        cout << "Range\t\t\t: 224 - 239\n";
        cout << "Default Subnet Mask\t: N/A\n";
        cout << "Hosts per Network\t: Multicast\n";
        cout << "Number of Networks\t: N/A\n";
    }
    else if (a >= 240 && a <= 255) {
        cout << "\nIP Address belongs to Class E (Experimental)\n";
        cout << "Range\t\t\t: 240 - 255\n";
        cout << "Default Subnet Mask\t: N/A\n";
        cout << "Hosts per Network\t: Experimental\n";
        cout << "Number of Networks\t: N/A\n";
    }
    else if (a == 127) {
        cout << "\nLoopback Address";
    }
    else {
        cout << "\nInvalid IP Address";
    }
}

int main() {

    cout << "Subnet: A subnet is a smaller network created by dividing a larger IP network into multiple smaller networks.\n\n";

    cout << "Subnet Mask: A subnet mask is a 32-bit number that separates an IP address into the Network ID and Host ID.\n\n";

    cout << "IPv4 Address Classes\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << "Class\tRange\t\tDefault Mask\t\tHosts/Network\t\t#Networks\n";
    cout << "-------------------------------------------------------------------------------\n";
    cout << "A\t1-126\t\t255.0.0.0\t\t2^24-2\t\t\t2^7\n";
    cout << "B\t128-191\t\t255.255.0.0\t\t2^16-2\t\t\t2^14\n";
    cout << "C\t192-223\t\t255.255.255.0\t\t2^8-2\t\t\t2^21\n";
    cout << "D\t224-239\t\tN/A\t\t\tMulticast\t\tN/A\n";
    cout << "E\t240-255\t\tN/A\t\t\tExperimental\t\tN/A\n";
    cout << "-------------------------------------------------------------------------------\n";

    ip_address();

    return 0;
}
