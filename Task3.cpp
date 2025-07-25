 #include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>
using namespace std;

class Bank_account {
    int acno;
    char name[50];
    int deposit;
    char type;

public:
    void create_account();
    void show_account() const;
    void modify();
    void dep(int);
    void draw(int);
    void report() const;
    int retacno() const;
    int retdeposit() const;
    char rettype() const;
};

void Bank_account::create_account() {
    cout << "\n\t\t\tEnter the Account No. : ";
    cin >> acno;
    cout << "\n\t\t\tEnter the Name of the Account holder : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tEnter type of the Account (C/S) : ";
    cin >> type;
    type = toupper(type);
    cout << "\n\t\t\tEnter The Initial amount : ";
    cin >> deposit;
    cout << "\n\t\t\tAccount Created..";
}

void Bank_account::show_account() const {
    cout << "\n\t\t\tAccount No. : " << acno;
    cout << "\n\t\t\tAccount Holder Name : " << name;
    cout << "\n\t\t\tType of Account : " << type;
    cout << "\n\t\t\tBalance amount : " << deposit;
}

void Bank_account::modify() {
    cout << "\n\t\t\tAccount No. : " << acno;
    cout << "\n\t\t\tModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "\n\t\t\tModify Type of Account : ";
    cin >> type;
    type = toupper(type);
    cout << "\n\t\t\tModify Balance amount : ";
    cin >> deposit;
}

void Bank_account::dep(int x) {
    deposit += x;
}

void Bank_account::draw(int x) {
    deposit -= x;
}

void Bank_account::report() const {
    cout << acno << setw(15) << name << setw(10) << type << setw(10) << deposit << endl;
}

int Bank_account::retacno() const {
    return acno;
}

int Bank_account::retdeposit() const {
    return deposit;
}

char Bank_account::rettype() const {
    return type;
}

void write_account() {
    Bank_account ac;
    ofstream outFile;
    outFile.open("account.dat", ios::binary | ios::app);
    ac.create_account();
    outFile.write(reinterpret_cast<char *>(&ac), sizeof(Bank_account));
    outFile.close();
}

void display_sp(int n) {
    Bank_account ac;
    bool flag = false;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Bank_account))) {
        if (ac.retacno() == n) {
            ac.show_account();
            flag = true;
        }
    }
    inFile.close();
    if (!flag)
        cout << "\n\n\t\t\tAccount number does not exist";
}

void modify_account(int n) {
    bool found = false;
    Bank_account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    while (!File.eof() && !found) {
        int pos = File.tellg();
        File.read(reinterpret_cast<char *>(&ac), sizeof(Bank_account));
        if (ac.retacno() == n) {
            ac.show_account();
            cout << "\n\n\t\t\tEnter The New Details of account" << endl;
            ac.modify();
            File.seekp(pos);
            File.write(reinterpret_cast<char *>(&ac), sizeof(Bank_account));
            cout << "\n\n\t\t\tRecord Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n\t\t\tRecord Not Found ";
}

void delete_account(int n) {
    Bank_account ac;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    inFile.seekg(0, ios::beg);
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Bank_account))) {
        if (ac.retacno() != n) {
            outFile.write(reinterpret_cast<char *>(&ac), sizeof(Bank_account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");
    cout << "\n\n\t\t\tRecord Deleted ..";
}

void display_all() {
    Bank_account ac;
    ifstream inFile;
    inFile.open("account.dat", ios::binary);
    if (!inFile) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    cout << "\n\n\t\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/c no.      NAME           Type    Balance\n";
    cout << "====================================================\n";
    while (inFile.read(reinterpret_cast<char *>(&ac), sizeof(Bank_account))) {
        ac.report();
    }
    inFile.close();
}

void deposit_withdraw(int n, int option) {
    int amt;
    bool found = false;
    Bank_account ac;
    fstream File;
    File.open("account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "File could not be open !! Press any Key...";
        return;
    }
    while (!File.eof() && !found) {
        int pos = File.tellg();
        File.read(reinterpret_cast<char *>(&ac), sizeof(Bank_account));
        if (ac.retacno() == n) {
            ac.show_account();
            if (option == 1) {
                cout << "\n\n\t\t\tTO DEPOSIT AMOUNT ";
                cout << "\n\n\t\t\tEnter The amount to be deposited: ";
                cin >> amt;
                ac.dep(amt);
            }
            if (option == 2) {
                cout << "\n\n\t\t\tTO WITHDRAW AMOUNT ";
                cout << "\n\n\t\t\tEnter The amount to be withdrawn: ";
                cin >> amt;
                if (amt > ac.retdeposit()) {
                    cout << "Insufficient balance";
                } else {
                    ac.draw(amt);
                }
            }
            File.seekp(pos);
            File.write(reinterpret_cast<char *>(&ac), sizeof(Bank_account));
            cout << "\n\n\t\t\tRecord Updated";
            found = true;
        }
    }
    File.close();
    if (!found)
        cout << "\n\n\t\t\tRecord Not Found ";
}

int main() {
    char ch;
    int num;

    do {
        system("CLS");
        cout << "\n\n\t\t\t============================\n";
        cout << "\t\t\t  BANK MANAGEMENT SYSTEM";
        cout << "\n\t\t\t============================\n";
        cout << "\n\t\t\t1. NEW ACCOUNT";
        cout << "\n\t\t\t2. DEPOSIT AMOUNT";
        cout << "\n\t\t\t3. WITHDRAW AMOUNT";
        cout << "\n\t\t\t4. BALANCE ENQUIRY";
        cout << "\n\t\t\t5. ALL ACCOUNT HOLDER LIST";
        cout << "\n\t\t\t6. CLOSE AN ACCOUNT";
        cout << "\n\t\t\t7. MODIFY AN ACCOUNT";
        cout << "\n\t\t\t8. EXIT";
        cout << "\n\n\t\t\tSelect Your Option (1-8): ";
        cin >> ch;

        switch (ch) {
            case '1':
                write_account();
                break;
            case '2':
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                deposit_withdraw(num, 1);
                break;
            case '3':
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                deposit_withdraw(num, 2);
                break;
            case '4':
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                display_sp(num);
                break;
            case '5':
                display_all();
                break;
            case '6':
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                delete_account(num);
                break;
            case '7':
                cout << "\n\n\t\t\tEnter The account No. : "; cin >> num;
                modify_account(num);
                break;
            case '8':
                cout << "\n\n\t\t\tThanks for using bank management system!";
                break;
            default:
                cout << "\n\n\t\t\tInvalid Option!";
        }
        cin.ignore();
        cin.get();
    } while (ch != '8');
    return 0;
}