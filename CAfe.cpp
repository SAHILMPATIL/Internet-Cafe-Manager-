#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Client {
    int id;
    double requestedBandwidth;
    double allocatedBandwidth;
    int remainingTime;
    
    Client(int id_, double bandwidth_, int time_) 
        : id(id_), requestedBandwidth(bandwidth_), allocatedBandwidth(0), remainingTime(time_) {}
};

class InternetCafe {
private:
    vector<Client> clients;
    double totalBandwidth;
    int maxClients;
    int nextClientId;

public:
    InternetCafe(double bandwidth = 100.0, int maxClients = 10) 
        : totalBandwidth(bandwidth), maxClients(maxClients), nextClientId(1) {}
    
    void allocateBandwidth() {
        if (clients.empty()) return;
        
        double demandSum = 0;
        for (const auto& client : clients) {
            demandSum += client.requestedBandwidth;
        }
        
        if (demandSum <= totalBandwidth) {
            for (auto& client : clients) {
                client.allocatedBandwidth = client.requestedBandwidth;
            }
        } else {
            for (auto& client : clients) {
                client.allocatedBandwidth = (client.requestedBandwidth / demandSum) * totalBandwidth;
            }
        }
    }
    
    void addClient(double requestedBandwidth, int timeMinutes) {
        if (clients.size() >= maxClients) {
            cout << "Cannot add more clients. Maximum limit reached.\n";
            return;
        }
        
        clients.emplace_back(nextClientId++, requestedBandwidth, timeMinutes);
        allocateBandwidth();
    }
    
    void simulateMinute() {
        cout << "----- Simulating 1 minute -----" << endl;
        
        for (auto& client : clients) {
            client.remainingTime--;
        }
        
        clients.erase(remove_if(clients.begin(), clients.end(), 
                      [](Client& c) { return c.remainingTime <= 0; }), 
                      clients.end());
        
        allocateBandwidth();
        displayStatus();
    }
    
    void displayStatus() const {
        cout << "Current Clients (" << clients.size() << "):\n";
        for (const auto& client : clients) {
            cout << "Client " << client.id 
                 << " - Allocated: " << client.allocatedBandwidth << " Mbps"
                 << ", Remaining Time: " << client.remainingTime << " minutes\n";
        }
        cout << "-------------------------------\n";
    }
    
    bool isEmpty() const {
        return clients.empty();
    }
};

class CafeSimulator {
private:
    InternetCafe cafe;
    int initialClients;

public:
    void setupClients() {
        cout << "Enter number of clients to connect (max allowed: 10): ";
        cin >> initialClients;
        
        if (initialClients > 10) {
            cout << "Too many clients! Limiting to 10.\n";
            initialClients = 10;
        }
        
        for (int i = 0; i < initialClients; ++i) {
            double bandwidth;
            int time;
            
            cout << "Client " << (i + 1) << " - Enter requested bandwidth (Mbps): ";
            cin >> bandwidth;
            cout << "Client " << (i + 1) << " - Enter session time (minutes): ";
            cin >> time;
            
            cafe.addClient(bandwidth, time);
        }
    }
    
    // Main simulation control - manages time-stepped execution
    void run() {
        setupClients();
        
        while (!cafe.isEmpty()) {
            cafe.simulateMinute();
        }
        
        cout << "Simulation complete. All clients disconnected.\n";
    }
};

int main() {
    CafeSimulator simulator;
    simulator.run();
    return 0;
}
