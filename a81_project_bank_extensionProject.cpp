#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;

struct stClient {
    string accountNumber{};
    string pinCode{};
    string name{};
    string phonenumber{};
    double balance{};
    bool isDeleted{};
};

/*
* module 1 - show screen
*/

// repeat string
string repeatString(string str, int times) {
    string temp{};
    while (times--) {
        temp += str;
    }
    return temp;
}

void mainMenuScreen(const string& TITLE) {
    cout << repeatString("=", 45) << endl;
    cout << right << setw(32) << TITLE << endl;
    cout << repeatString("=", 45) << endl;
}

// show menu of options

void menu() {
    mainMenuScreen("Main Menu Screen");
    cout << repeatString("\t", 2) << "[1] Show client list." << endl;
    cout << repeatString("\t", 2) << "[2] Add new client." << endl;
    cout << repeatString("\t", 2) << "[3] Delete client." << endl;
    cout << repeatString("\t", 2) << "[4] Update client info." << endl;
    cout << repeatString("\t", 2) << "[5] Find client." << endl;
    cout << repeatString("\t", 2) << "[6] Transactions." << endl;
    cout << repeatString("\t", 2) << "[7] Exit." << endl;
    cout << repeatString("=", 45) << endl;
}

/*
* module 2 - list of options
*/

enum enChoice { SHOW_CLIENT_LIST = 1, ADD_NEW_CLIENT = 2, DELETE_CLIENT = 3, UPDATE_CLIENT_INFO = 4, FIND_CLIENT = 5, TRANSACTION_LIST = 6, EXIT = 7 };

// read choice
int readInRange(const string& MESSAGE, const int& FROM, const int& TO ) {
    int temp{ 1 };
    // validate string
    bool isString{};
    do {
        cout << MESSAGE << " ";
        cin >> temp;
        isString = cin.fail();

        if (isString) {
            // ignore error
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "You entered a string which is not valid!";
        }
        else if (temp < FROM || temp > TO)
            cout << "Number is out of range. Enter a number between (" << FROM << " and " << TO << ")\n";

    } while ((temp < FROM || temp > TO) || isString);

    return temp;
}

// int to enum
enChoice getUserOption(const string& MESSAGE, const int& FROM, const int& TO) {
    return (enChoice)readInRange(MESSAGE, FROM, TO);
}

/*
* module 3 - show client info
*/

// menu_header

void clientListHeader(const int& numberOfClients) {
    cout << "\n" << right << setw(60) << "Client list " << "(" << numberOfClients << ") client(s)\n\n";
    cout << repeatString("_", 110) << "\n\n";
    cout << "| " << left << setw(15) << "Account number";
    cout << "| " << left << setw(15) << "Pin code";
    cout << "| " << left << setw(30) << "Name";
    cout << "| " << left << setw(25) << "Phone";
    cout << "| " << left << setw(15) << "Account balance\n";
    cout << repeatString("_", 110) << endl << endl;
}

// read file
// convert line to struct
// split string

vector<string> splitString(string line, const string& delimeter) {
    // define a container
    vector<string> vTokens;
    // locate delimeter
    int delimPos{};
    string token{};
    while ((delimPos = line.find(delimeter)) != std::string::npos) {
        // get token
        token = line.substr(0, delimPos);
        // ignore adjacent delimeters
        if (token != "")
            vTokens.push_back(token);
        // remove old token
        line.erase(0, delimPos + delimeter.length());
    }
    // ignore post last element delimeters
    if (line != "")
        vTokens.push_back(line);

    return vTokens;
}

stClient convertLineToStruct(vector<string>& vRecord) {
    vector<string>::iterator ptr = vRecord.begin();
    stClient client;
    client.accountNumber = *ptr++;
    client.pinCode = *ptr++;
    client.name = *ptr++;
    client.phonenumber = *ptr++;
    client.balance = stod(*ptr);
    // no need for know isDelete state since we only need to delete at the run time, and if so, this record won't be even in the file!
    return client;
}

vector<stClient> readFile(const string& fileName, const string& delimeter) {
    // declare file
    fstream myFile;
    // define a container
    vector<stClient> vClients;
    vector<string> vRecord;
    // select mode
    myFile.open(fileName, ios::in);
    // begin reading
    string line{};
    if (myFile.is_open()) {  // safety measurements
        while (getline(myFile, line)) {
            // safety measurement
            if (line != "") {
                vRecord = splitString(line, delimeter);
                vClients.push_back(convertLineToStruct(vRecord));
            }
        }
        myFile.close();
    }
    return vClients;
}

// print client record

void printClientRecord(const stClient& client) {
    cout << "| " << left << setw(15) << client.accountNumber;
    cout << "| " << left << setw(15) << client.pinCode;
    cout << "| " << left << setw(30) << client.name;
    cout << "| " << left << setw(25) << client.phonenumber;
    cout << "| " << left << setw(15) << client.balance << endl;
}

void printRecords(const vector<stClient>& vClients) {
    for (const stClient& client : vClients) {
        printClientRecord(client);
    }
    cout << "\n\n" << repeatString("_", 110) << "\n\n\n";

}

void showClientList(const string& fileName, const string& delimeter) {
    vector<stClient> vClients = readFile(fileName, delimeter);
    if (!vClients.empty()) {
        clientListHeader(vClients.size());
        printRecords(vClients);
    }
    else
        cout << "\nFile is Empty! you have to enter some data first :-)\n\n";
}

/*
* module 4 - finding client
*/

// header
// read accountNumber
// read data from file
// line to struct
// get client index
// print info

void header(string message) {
    cout << repeatString("-", 20) << endl;
    cout << right << setw(15) << message << '\n';
    cout << repeatString("-", 20) << endl;
}

string readString(string message) {
    cout << message << " ";
    string temp{};
    cin >> temp;
    return temp;
}

enum enIsFound { FOUND = 1, NOT_FOUND = 2 };

enIsFound isFound(const stClient& client, const string& accountNumber) {
    return (client.accountNumber == accountNumber) ? enIsFound::FOUND : enIsFound::NOT_FOUND;
}

int searchForClient(const vector<stClient>& client, const string& accountNumber) {
    for (int idx = 0; idx < client.size(); idx++) {
        if (isFound(client.at(idx), accountNumber) == enIsFound::FOUND)
            return idx;
    }
    return -1;
}

void printClientInfo(const vector<stClient>& vClients, const int& index, const string& accountNumber) {
    if (index != -1) {
        cout << "\nThe following is the client details\n";
        cout << repeatString("-", 20) << endl;
        cout << left << setw(12) << "Account number" << ": " << vClients.at(index).accountNumber << '\n';
        cout << left << setw(14) << "Pin code" << ": " << vClients.at(index).pinCode << '\n';
        cout << left << setw(14) << "name" << ": " << vClients.at(index).name << '\n';
        cout << left << setw(14) << "Phone number" << ": " << vClients.at(index).phonenumber << '\n';
        cout << left << setw(14) << "Balance" << ": $" << vClients.at(index).balance << '\n';
        cout << repeatString("-", 20) << endl;
    }
    else
        cout << "Client with account number " << accountNumber << " is not found\n";
}

void search(const vector<stClient>& vClients) {
    header("Find client screen");
    const string accountNumber = readString("Enter account number?");
    const int idx = searchForClient(vClients, accountNumber);
    printClientInfo(vClients, idx, accountNumber);
}

/*
* module 5 - add a new client
*/

// read account number 
// validate client is unique
// read client info
// read balance
// write to file
// do you want to add more?

double readClientBalance(const string& message) {
    double temp{ 1 };
    bool isString{};
    do {
        cout << message << " ";
        cin >> temp;
        // validate string
        isString = cin.fail();

        if (isString) {
            // ignore
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "You can't enter a string!\n";
        }
        else if (temp < 0)
            cout << "You can't enter a negative number\n";
        else if (temp == 0)
            cout << "Balance can't be zero\n";
    } while (isString || temp <= 0);
    return temp;
}

stClient readClientInfo(const string& accountNumber) {
    stClient client;
    client.accountNumber = accountNumber;
    cout << "Enter Pin Code: ";
    getline(cin >> ws, client.pinCode); // enable multiple reading
    cout << "Enter Name: ";
    getline(cin, client.name);
    cout << "Enter Phone Number: ";
    getline(cin, client.phonenumber);
    client.balance = readClientBalance("Enter Balance");

    return client;
}

/*
* write in file
*/

// struct to line

string convertStructToLine(const stClient& client, const string& delimeter) {
    string record{ "" };
    record += client.accountNumber + delimeter;
    record += client.pinCode + delimeter;
    record += client.name + delimeter;
    record += client.phonenumber + delimeter;
    return record += to_string(client.balance);
}

//vector<string> convertStructsToLines(const vector<stClient>& vClients, const string& delimeter) {
//    vector<string> vRecords;
//    for (const stClient client : vClients) {
//        vRecords.push_back(convertStructToLine(client, delimeter));
//    }
//    return vRecords;
//}

// you can append if you read a struct not a vector of them

void writeInFile(const string& fileName, vector<stClient> vClients, const string& delimeter, bool append = false) {
    // define a container of lines
    // vector<string> vRecords = convertStructsToLines(vClients, delimeter);

    // declare file
    fstream myFile;
    // select a mode
    myFile.open(fileName, ios::out);
    // begin
    string temp{};
    if (myFile.is_open() && !vClients.empty()) {

        for (const stClient& client : vClients) {
            temp = convertStructToLine(client, delimeter);
            if (!temp.empty() && !client.isDeleted) {
                myFile << temp << endl;
            }
        }
    }
    // end writing
    myFile.close();
}

// case insinsitive
bool isUpperChar(const char& c) {
    return (c >= 65 && c <= 90);
}

char toLowerChar(char c) {
    if (isUpperChar(c))
        return c += 32;
    else
        return c;
}

// ignore duplicates
bool isUniqueClient(const vector<stClient>& vClients, const string& accountNumber) {
    /*return(searchForClient(vClients, accountNumber) == -1) ? true : false;*/
    return (searchForClient(vClients, accountNumber) == -1);
}

void addNewClient(vector<stClient>& vClients, const string& delimeter, const string& fileName) {
    header("Adding new client screen");
    cout << "\nAdding a new client\n\n";
    string message{ "Enter account number" };
    string accountNumber{ "" };
    bool unique{};
    char ans = 'y';
    do {
        accountNumber = readString(message);
        if ((unique = isUniqueClient(vClients, accountNumber)) == true) {
            // read client data
            vClients.push_back(readClientInfo(accountNumber));
            // write in file
            writeInFile(fileName, vClients, delimeter);
            cout << "Client added successfully, Do you want to add more clients? (Y-N)? ";
            cin >> ans;
            message = "Enter account number";
        }
        else {
            cout << "Client with account number " << accountNumber << " already exists, ";
            message = "Enter another account number";
        }

    } while (toLowerChar(ans) == 'y' || !unique);
}

/*
* module 6 - delete client
*/

// if client it there, delete it
void markClientForDeletion(stClient& client) {
    client.isDeleted = true;
}

bool sayYes(string message) {
    cout << message << " ";
    char ans;
    cin >> ans;
    return((toLowerChar(ans)) == 'y');
}

void deleteClient(vector<stClient>& vClients, const string& delimeter, const string& fileName) {
    header("Deletion screen");
    string accountNumber = readString("Enter account number?");
    int index = searchForClient(vClients, accountNumber);
    if (index != -1) {
        printClientInfo(vClients, index, accountNumber);
        if (sayYes("Are you sure you want to delete it? (y-n)?")) {
            markClientForDeletion(vClients.at(index));
            writeInFile(fileName, vClients, delimeter);
            cout << "\n\nClient deleted successfully\n\n";
        }
    }
    else
        cout << "Client with account number (" << accountNumber << ") is not there!\n\n";

    // go back screen
}

/*
* module 7 - update client info
*/

// if client is there, update it

bool updateClientInfo(vector<stClient>& vClients, const string& delimeter, const string& fileName) {
    header("Update Client Info Screen");
    string accountNumber = readString("Enter account number?");
    int index = searchForClient(vClients, accountNumber);
    if (index != -1) {
        printClientInfo(vClients, index, accountNumber);
        if (sayYes("Are you sure you want to update this client? (y-n)?")) {
            vClients.at(index) = readClientInfo(accountNumber);
            writeInFile(fileName, vClients, delimeter);
            cout << "\n\nClient updated successfully\n";
            return true;
        }
    }
    else
        cout << "\n\nClient with account number (" << accountNumber << ") is not there\n";
    // go back screen


    return false;
}

/*
* module 8 - perform transactions
*/

/*
* show transaction menu
*/

void transactionMenu() {
    mainMenuScreen("Transactions Menu Screen");
    cout << repeatString("\t", 2) << "[1] Deposit." << endl;
    cout << repeatString("\t", 2) << "[2] Withdraw." << endl;
    cout << repeatString("\t", 2) << "[3] Total Balance." << endl;
    cout << repeatString("\t", 2) << "[4] Main menu." << endl;
}

enum enTransactionOption {DEPOSIT = 1, WITHDRAW = 2, TOTAL_BALANCE = 3, MAIN_MENU = 4};

enTransactionOption getTransactionOption(const int& FROM, const int& TO) {
    return (enTransactionOption)readInRange("Choose what do you want to do? (1 - 4)?", 1, 4);
}

/*
* Deposit
*/

// if account number exists, make a deposit

void makeADeposit(stClient& client, const double& AMOUNT) {
    client.balance += AMOUNT;
}

void depositOrWithdraw(vector<stClient>& vClients, const string& SCREEN_MESSAGE, bool deposit = true) {
    header(SCREEN_MESSAGE);
    string accountNumber{}, message{"Enter account number?"};
    int index{};
    double amount{};
    do {
        accountNumber = readString(message);
        if ( (index = searchForClient(vClients,accountNumber)) != -1 ) {
            printClientInfo(vClients, index, accountNumber);
            // make a withdrawal
            if (!deposit) {
                while ((amount = readClientBalance("Please, Enter amount?")) > vClients.at(index).balance) {
                    cout << "Amount exceeds the balance. You can withdraw up to " << vClients.at(index).balance << "\n";
                }
                amount *= -1;
            }
            else // make a deposit
                amount = readClientBalance("Please, Enter amount?");

            if (sayYes("Are you sure you want to perform this transaction? (y-n)?")) {
                makeADeposit(vClients.at(index), amount);
                cout << "Done successfully. New balance becomes $" << vClients.at(index).balance << "\n";
            }
            
        }
        else {
            cout << "Client with account number (" << accountNumber << ") doesn't exist. ";
            message = "Enter another account number";
        }


    } while (index == -1);
}

/*
* get total balance
*/

// brief info

void briefClientListHeader(const int& NUM_OF_CLIENTS) {
    cout << "\n" << right << setw(60) << "Client list " << "(" << NUM_OF_CLIENTS << ") client(s)\n\n";
    cout << repeatString("_", 110) << "\n\n";
    cout << "| " << left << setw(15) << "Account number";
    cout << "| " << left << setw(30) << "Name";
    cout << "| " << left << setw(15) << "Account balance\n";
    cout << repeatString("_", 110) << endl << endl;
}

void showClientBriefInfo(const stClient& CLIENT) {
    cout << "| " << left << setw(15) << CLIENT.accountNumber;
    cout << "| " << left << setw(30) << CLIENT.name;
    cout << "| $" << left << setw(15) << CLIENT.balance << endl;
}

void showTotalBalance(const vector<stClient>& V_CLIENTS) {
    double sum{};
    briefClientListHeader(V_CLIENTS.size());
    for (const stClient& CLIENT : V_CLIENTS) {
        showClientBriefInfo(CLIENT);
        sum += CLIENT.balance;
    }
    cout << "\n\n" << right << setw(45) << "Total balance = $" << sum << endl;
    
}

/*
* start transactions
*/

void goBackSCreen() {
    cout << "Press any key to go back to the main menu... ";
    system("pause>0");
    system("cls");
}

enum enQuit { EXIT_PROGRAM = 1, STAY = 2 };

void runTransaction(vector<stClient>& vClients, const string& FILE_NAME, const string& DELIMETER) {
    enQuit quit = enQuit::STAY;
    while (quit == enQuit::STAY) {
        // show transaction menu
        transactionMenu();
        switch (getTransactionOption(1, 4)) {
        case enTransactionOption::DEPOSIT:
            system("cls");
            depositOrWithdraw(vClients, "Deposit screen");
            break;
        case enTransactionOption::WITHDRAW:
            system("cls");
            depositOrWithdraw(vClients, "Withdrawal screen", false);
            break;
        case enTransactionOption::TOTAL_BALANCE:
            system("cls");
            showTotalBalance(vClients);
            break;
        case enTransactionOption::MAIN_MENU:
            system("cls");
            quit = enQuit::EXIT_PROGRAM;
            break;
        default:
            return;
        }
        // update vector
        writeInFile(FILE_NAME, vClients, DELIMETER);

        if (quit != enQuit::EXIT_PROGRAM)
            goBackSCreen();
    }
}

/*
* final module
*/

void start(const string& fileName, const string& delimeter) {
    enQuit quit = enQuit::STAY;
    vector <stClient> vClients;
    bool isInTransactionMenu{};
    while (quit != enQuit::EXIT_PROGRAM) {
        menu();
        switch (getUserOption("Enter your choice", 1, 7))
        {
        case enChoice::SHOW_CLIENT_LIST:
            showClientList(fileName, delimeter);
            break;
        case enChoice::ADD_NEW_CLIENT:
            system("cls");
            addNewClient(vClients, delimeter, fileName);
            break;
        case enChoice::DELETE_CLIENT:
            system("cls");
            deleteClient(vClients, delimeter, fileName);
            break;
        case enChoice::UPDATE_CLIENT_INFO:
            system("cls");
            updateClientInfo(vClients, delimeter, fileName);
            break;
        case enChoice::FIND_CLIENT:
            system("cls");
            search(vClients);
            break;
        case enChoice::TRANSACTION_LIST:
            system("cls");
            runTransaction(vClients, fileName, delimeter);
            isInTransactionMenu = true;
            break;
        case enChoice::EXIT:
            system("cls");
            quit = enQuit::EXIT_PROGRAM;
            break;
        default:
            return;
        }
        // update vector
        vClients = readFile(fileName, delimeter);

        if (quit != enQuit::EXIT_PROGRAM && !isInTransactionMenu)
            goBackSCreen();
        isInTransactionMenu = false;
    }
    cout << "\n\nProgram has been terminated\n";
}

int main()
{
    start("client.txt", "#//#");
    system("pause > 0");
}
