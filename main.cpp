#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

// ---------- UTILITY ----------
string currentTime() {
    time_t now = time(0);
    return ctime(&now);
}

void logTransaction(const string& user, const string& type, double amount) {
    ofstream file("transactions.txt", ios::app);
    file << user << " | " << type << " | " << amount << " | " << currentTime();
    file.close();
}

// ---------- ACCOUNT REQUEST ----------
void requestAccount() {
    string username;
    int pin;

    cout << "Enter Username: ";
    cin >> username;
    cout << "Set 4-digit PIN: ";
    cin >> pin;

    ofstream file("requests.txt", ios::app);
    file << username << " " << pin << endl;
    file.close();

    cout << "Account request sent to Admin\n";
}

// ---------- ADMIN PANEL ----------
void adminPanel() {
    int choice;
    while (true) {
        cout << "\n--- ADMIN PANEL ---\n";
        cout << "1. Approve Accounts\n2. View All Users\n3. View Transactions\n4. Logout\nChoose: ";
        cin >> choice;

        if (choice == 1) {
            ifstream req("requests.txt");
            ofstream acc("accounts.txt", ios::app);

            string user; int pin;
            while (req >> user >> pin) {
                acc << user << " " << pin << " 0\n";
                cout << "Approved: " << user << endl;
            }

            req.close(); acc.close();
            remove("requests.txt");
        }
        else if (choice == 2) {
            ifstream file("accounts.txt");
            string u; int p; double b;
            while (file >> u >> p >> b)
                cout << u << " | Balance: " << b << endl;
            file.close();
        }
        else if (choice == 3) {
            ifstream file("transactions.txt");
            string line;
            while (getline(file, line)) cout << line << endl;
            file.close();
        }
        else break;
    }
}

// ---------- USER PANEL ----------
void userPanel(const string& user) {
    int choice;
    double amount;

    while (true) {
        cout << "\n--- USER DASHBOARD ---\n";
        cout << "1.Deposit  2.Withdraw  3.Check Balance  4.Logout\nChoose: ";
        cin >> choice;

        ifstream in("accounts.txt");
        ofstream out("temp.txt");

        string u; int p; double b;
        while (in >> u >> p >> b) {
            if (u == user) {
                if (choice == 1) {
                    cout << "Amount: ";
                    cin >> amount;
                    b += amount;
                    logTransaction(user, "Deposit", amount);
                }
                else if (choice == 2) {
                    cout << "Amount: ";
                    cin >> amount;
                    if (amount <= b) {
                        b -= amount;
                        logTransaction(user, "Withdraw", amount);
                    }
                    else {
                        cout << "Insufficient balance\n";
                    }
                }
                else if (choice == 3) {
                    cout << "Current Balance: " << b << endl;
                }
            }
            out << u << " " << p << " " << b << endl;
        }

        in.close(); out.close();
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");

        if (choice == 4) break;
    }
}

// ---------- LOGIN ----------
void login() {
    string username;
    int pin;

    cout << "Username: ";
    cin >> username;
    cout << "PIN: ";
    cin >> pin;

    if (username == "admin" && pin == 1234) {
        adminPanel();
        return;
    }

    ifstream file("accounts.txt");
    string u; int p; double b;

    while (file >> u >> p >> b) {
        if (u == username && p == pin) {
            userPanel(username);
            file.close();
            return;
        }
    }
    file.close();

    cout << "Invalid login\n";
}

// ---------- MAIN ----------
int main() {
    int choice;
    while (true) {
        cout << "\nBANK MANAGEMENT SYSTEM\n";
        cout << "1. Login\n2. Request Account\n3. Exit\nChoose: ";
        cin >> choice;

        if (choice == 1) login();
        else if (choice == 2) requestAccount();
        else break;
    }
    return 0;
}
