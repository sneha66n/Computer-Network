#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <cstdint>
#include <limits>
#include <cctype>

using namespace std;

// ============================================================
// Convert IPv4 string into 4 integer octets
// ============================================================
bool parseIPv4(const string& ip, array<int, 4>& address)
{
    stringstream ss(ip);
    string part;
    int index = 0;

    while (getline(ss, part, '.'))
    {
        if (index >= 4 || part.empty())
            return false;

        for (char c : part)
        {
            if (!isdigit(static_cast<unsigned char>(c)))
                return false;
        }

        try
        {
            address[index] = stoi(part);
        }
        catch (...)
        {
            return false;
        }

        if (address[index] < 0 || address[index] > 255)
            return false;

        index++;
    }

    return index == 4;
}


// ============================================================
// Convert 4 octets into a 32-bit unsigned integer
// ============================================================
uint32_t ipToInteger(const array<int, 4>& address)
{
    return (static_cast<uint32_t>(address[0]) << 24) |
           (static_cast<uint32_t>(address[1]) << 16) |
           (static_cast<uint32_t>(address[2]) << 8)  |
            static_cast<uint32_t>(address[3]);
}


// ============================================================
// Convert 32-bit integer back to IPv4 string
// ============================================================
string integerToIP(uint32_t ip)
{
    return to_string((ip >> 24) & 255) + "." +
           to_string((ip >> 16) & 255) + "." +
           to_string((ip >> 8) & 255) + "." +
           to_string(ip & 255);
}


// ============================================================
// Convert an octet to 8-bit binary
// ============================================================
string binary8(int value)
{
    string result;

    for (int i = 7; i >= 0; i--)
    {
        if (value & (1 << i))
            result += '1';
        else
            result += '0';
    }

    return result;
}


// ============================================================
// Structure containing class information
// ============================================================
struct ClassInfo
{
    char ipClass;
    string range;
    string defaultMask;
    int networkBits;
    int hostBits;
    uint64_t usableHosts;
    uint64_t numberOfNetworks;
};


// ============================================================
// Determine IP class
// ============================================================
ClassInfo getClassInfo(int firstOctet)
{
    // Class A
    if (firstOctet >= 1 && firstOctet <= 126)
    {
        return {
            'A',
            "1.0.0.0 - 126.255.255.255",
            "255.0.0.0",
            8,
            24,
            (1ULL << 24) - 2,
            126
        };
    }

    // Class B
    if (firstOctet >= 128 && firstOctet <= 191)
    {
        return {
            'B',
            "128.0.0.0 - 191.255.255.255",
            "255.255.0.0",
            16,
            16,
            (1ULL << 16) - 2,
            16384
        };
    }

    // Class C
    if (firstOctet >= 192 && firstOctet <= 223)
    {
        return {
            'C',
            "192.0.0.0 - 223.255.255.255",
            "255.255.255.0",
            24,
            8,
            254,
            2097152
        };
    }

    // Class D
    if (firstOctet >= 224 && firstOctet <= 239)
    {
        return {
            'D',
            "224.0.0.0 - 239.255.255.255",
            "N/A",
            0,
            0,
            0,
            0
        };
    }

    // Class E
    return {
        'E',
        "240.0.0.0 - 255.255.255.255",
        "N/A",
        0,
        0,
        0,
        0
    };
}


// ============================================================
// PART 1
// Classful IPv4 Address Information
// ============================================================
void classfulAnalysis(
    const string& ip,
    const array<int, 4>& address)
{
    int firstOctet = address[0];

    cout << "\n";
    cout << "============================================================\n";
    cout << "             PART 1 - CLASSFUL IP ANALYSIS\n";
    cout << "============================================================\n";

    cout << "IP Address           : " << ip << endl;

    cout << "First Octet          : "
         << firstOctet << endl;

    cout << "First Octet (Binary) : "
         << binary8(firstOctet) << endl;


    // --------------------------------------------------------
    // Loopback
    // --------------------------------------------------------
    if (firstOctet == 127)
    {
        cout << "IP Class             : Special / Loopback" << endl;
    }
    else
    {
        ClassInfo info = getClassInfo(firstOctet);

        cout << "IP Class             : "
             << info.ipClass << endl;

        cout << "IP Range             : "
             << info.range << endl;

        cout << "Default Subnet Mask  : "
             << info.defaultMask << endl;


        // Class A, B and C have network/host information
        if (info.ipClass == 'A' ||
            info.ipClass == 'B' ||
            info.ipClass == 'C')
        {
            cout << "Network Bits         : "
                 << info.networkBits << endl;

            cout << "Host Bits            : "
                 << info.hostBits << endl;

            cout << "Usable Hosts         : "
                 << info.usableHosts << endl;

            cout << "Number of Networks   : "
                 << info.numberOfNetworks << endl;

            cout << "Host Formula         : 2^"
                 << info.hostBits
                 << " - 2" << endl;
        }
        else if (info.ipClass == 'D')
        {
            cout << "Purpose              : Multicast" << endl;
            cout << "Network/Host Bits    : Not Applicable" << endl;
        }
        else
        {
            cout << "Purpose              : Experimental / Reserved"
                 << endl;

            cout << "Network/Host Bits    : Not Applicable"
                 << endl;
        }
    }


    // ========================================================
    // SPECIAL ADDRESS CHECK
    // ========================================================

    cout << "\n---------------- SPECIAL ADDRESS CHECK ----------------\n";

    uint32_t value = ipToInteger(address);

    bool foundSpecial = false;


    // 0.0.0.0
    if (value == 0)
    {
        cout << "- 0.0.0.0 : Default / Unspecified Address\n";
        foundSpecial = true;
    }


    // 127.x.x.x
    if (firstOctet == 127)
    {
        cout << "- 127.x.x.x : Loopback Address\n";
        foundSpecial = true;
    }


    // 255.255.255.255
    if (value == 0xFFFFFFFF)
    {
        cout << "- 255.255.255.255 : Limited Broadcast Address\n";
        foundSpecial = true;
    }


    // 169.254.x.x
    if (firstOctet == 169 && address[1] == 254)
    {
        cout << "- 169.254.x.x : APIPA / Link-Local Address\n";
        foundSpecial = true;
    }


    // Private Class A
    if (firstOctet == 10)
    {
        cout << "- 10.x.x.x : Private IP Address\n";
        foundSpecial = true;
    }


    // Private Class B
    if (firstOctet == 172 &&
        address[1] >= 16 &&
        address[1] <= 31)
    {
        cout << "- 172.16.x.x - 172.31.x.x : Private IP Address\n";
        foundSpecial = true;
    }


    // Private Class C
    if (firstOctet == 192 &&
        address[1] == 168)
    {
        cout << "- 192.168.x.x : Private IP Address\n";
        foundSpecial = true;
    }


    // Multicast
    if (firstOctet >= 224 && firstOctet <= 239)
    {
        cout << "- 224.x.x.x - 239.x.x.x : Multicast Address\n";
        foundSpecial = true;
    }


    // Experimental
    if (firstOctet >= 240)
    {
        cout << "- 240.x.x.x - 255.x.x.x : Experimental / Reserved\n";
        foundSpecial = true;
    }


    if (!foundSpecial)
    {
        cout << "- Normal unicast IPv4 address\n";
    }
}


// ============================================================
// PART 2
// SUBNETTING
// ============================================================
bool subnetting(
    const array<int, 4>& address,
    int requiredSubnets)
{
    int firstOctet = address[0];


    // --------------------------------------------------------
    // Subnetting only for Class A, B and C
    // --------------------------------------------------------
    if (firstOctet == 127 ||
        firstOctet >= 224)
    {
        cout << "\nSubnetting is supported only for "
             << "Class A, B and C addresses.\n";

        return false;
    }


    ClassInfo info = getClassInfo(firstOctet);

    int defaultNetworkBits = info.networkBits;


    // --------------------------------------------------------
    // Calculate borrowed bits
    // --------------------------------------------------------
    int borrowedBits = 0;

    int actualSubnets = 1;

    while (actualSubnets < requiredSubnets)
    {
        borrowedBits++;
        actualSubnets *= 2;
    }


    // --------------------------------------------------------
    // Check whether enough host bits are available
    // --------------------------------------------------------
    int subnetPrefix =
        defaultNetworkBits + borrowedBits;

    if (subnetPrefix > 30)
    {
        cout << "\nERROR: Too many subnets requested.\n";
        cout << "There must be at least 2 host bits "
             << "for conventional subnetting.\n";

        return false;
    }


    // --------------------------------------------------------
    // Calculate host bits
    // --------------------------------------------------------
    int hostBits = 32 - subnetPrefix;


    // --------------------------------------------------------
    // Addresses per subnet
    // --------------------------------------------------------
    uint32_t addressesPerSubnet =
        (1u << hostBits);


    // --------------------------------------------------------
    // Usable host addresses
    // --------------------------------------------------------
    uint32_t usableHosts =
        addressesPerSubnet - 2;


    // --------------------------------------------------------
    // Create subnet mask
    // --------------------------------------------------------
    uint32_t mask;

    if (subnetPrefix == 0)
    {
        mask = 0;
    }
    else
    {
        mask =
            0xFFFFFFFFu << (32 - subnetPrefix);
    }


    // --------------------------------------------------------
    // Convert input IP to 32-bit integer
    // --------------------------------------------------------
    uint32_t ipValue =
        ipToInteger(address);


    // --------------------------------------------------------
    // Find original network address
    // --------------------------------------------------------
    uint32_t networkAddress =
        ipValue & mask;


    // ========================================================
    // DISPLAY SUBNET INFORMATION
    // ========================================================

    cout << "\n";
    cout << "============================================================\n";
    cout << "                 PART 2 - SUBNETTING\n";
    cout << "============================================================\n";

    cout << "Required Subnets      : "
         << requiredSubnets << endl;

    cout << "Borrowed Bits         : "
         << borrowedBits << endl;

    cout << "Actual Number Subnets : "
         << actualSubnets << endl;

    cout << "Subnet Prefix         : /"
         << subnetPrefix << endl;

    cout << "Host Bits             : "
         << hostBits << endl;

    cout << "Addresses per Subnet  : "
         << addressesPerSubnet << endl;

    cout << "Usable IPs per Subnet : "
         << usableHosts << endl;

    cout << "Subnet Mask           : "
         << integerToIP(mask) << endl;


    // ========================================================
    // PRINT EVERY SUBNET
    // ========================================================

    cout << "\n";
    cout << "------------------------------------------------------------\n";
    cout << "                    SUBNET DETAILS\n";
    cout << "------------------------------------------------------------\n";


    for (int i = 0; i < actualSubnets; i++)
    {
        uint32_t subnetNetwork =
            networkAddress +
            (static_cast<uint32_t>(i) *
             addressesPerSubnet);


        uint32_t broadcast =
            subnetNetwork +
            addressesPerSubnet -
            1;


        uint32_t firstIP =
            subnetNetwork + 1;


        uint32_t lastIP =
            broadcast - 1;


        cout << "\nSubnet " << i + 1 << endl;

        cout << "Network Address : "
             << integerToIP(subnetNetwork)
             << endl;

        cout << "First IP        : "
             << integerToIP(firstIP)
             << endl;

        cout << "Last IP         : "
             << integerToIP(lastIP)
             << endl;

        cout << "Broadcast       : "
             << integerToIP(broadcast)
             << endl;

        cout << "------------------------------------------------------------\n";
    }

    return true;
}


// ============================================================
// MAIN FUNCTION
// ============================================================
int main()
{
    cout << "============================================================\n";
    cout << "       IPv4 CLASSFUL ADDRESSING AND SUBNETTING\n";
    cout << "============================================================\n";


    // ========================================================
    // INPUT IP ADDRESS
    // ========================================================

    string ip;
    array<int, 4> address{};


    while (true)
    {
        cout << "\nEnter IPv4 address: ";
        cin >> ip;


        if (parseIPv4(ip, address))
        {
            break;
        }


        cout << "Invalid IPv4 address!\n";
        cout << "Example: 195.1.1.10\n";
    }


    // ========================================================
    // PART 1
    // ========================================================

    classfulAnalysis(ip, address);


    // ========================================================
    // INPUT NUMBER OF SUBNETS
    // ========================================================

    int requiredSubnets;


    while (true)
    {
        cout << "\nEnter number of required subnets: ";
        cin >> requiredSubnets;


        if (cin &&
            requiredSubnets > 0)
        {
            break;
        }


        cout << "Please enter a positive integer.\n";


        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );
    }


    // ========================================================
    // PART 2
    // ========================================================

    subnetting(address, requiredSubnets);


    cout << "\n";
    cout << "============================================================\n";
    cout << "              PROGRAM COMPLETED SUCCESSFULLY\n";
    cout << "============================================================\n";


    return 0;
}
