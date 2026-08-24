#include <iostream>
using namespace std;

#define N 4
#define INF 999

int main()
{
    int cost[N][N];
    int dist[N][N];
    int nextHop[N][N];

    // Read cost matrix
    cout << "Enter the cost matrix (999 for no connection):\n";

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> cost[i][j];
        }
    }

    // Initialize distance and next hop
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            dist[i][j] = cost[i][j];

            if (i == j)
            {
                nextHop[i][j] = i;
            }
            else if (cost[i][j] != INF)
            {
                nextHop[i][j] = j;
            }
            else
            {
                nextHop[i][j] = -1;
            }
        }
    }

    // Repeat until no update occurs
    int updated;

    do
    {
        updated = 0;

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                for (int k = 0; k < N; k++)
                {
                    if (dist[i][k] != INF &&
                        dist[k][j] != INF &&
                        dist[i][j] > dist[i][k] + dist[k][j])
                    {
                        dist[i][j] = dist[i][k] + dist[k][j];

                        nextHop[i][j] = nextHop[i][k];

                        updated = 1;
                    }
                }
            }
        }

    } while (updated == 1);

    // Display final routing tables
    cout << "\n===== FINAL ROUTING TABLE =====\n";

    for (int i = 0; i < N; i++)
    {
        cout << "\nRouter " << char('A' + i) << ":\n";

        cout << "Destination\tCost\tNext Hop\n";

        for (int j = 0; j < N; j++)
        {
            cout << char('A' + j) << "\t\t";

            if (dist[i][j] == INF)
            {
                cout << "INF\t-\n";
            }
            else
            {
                cout << dist[i][j] << "\t"
                     << char('A' + nextHop[i][j]) << "\n";
            }
        }
    }

    // Source and destination
    char source, destination;

    cout << "\nEnter source router (A-D): ";
    cin >> source;

    cout << "Enter destination router (A-D): ";
    cin >> destination;

    int s = source - 'A';
    int d = destination - 'A';

    // Minimum distance
    cout << "\nMinimum distance from "
         << source << " to "
         << destination << " = "
         << dist[s][d] << endl;

    // Complete route
    cout << "Route: ";

    if (dist[s][d] == INF)
    {
        cout << "No route available";
    }
    else
    {
        int current = s;

        cout << char('A' + current);

        while (current != d)
        {
            current = nextHop[current][d];

            if (current == -1)
            {
                cout << "\nNo route available";
                break;
            }

            cout << " -> " << char('A' + current);
        }
    }

    cout << endl;

    return 0;
}
