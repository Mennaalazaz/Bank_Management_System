#include <iostream>
#include <fstream>
#include <vector>
#include<iomanip>
#include<string>
#include <algorithm>
#include <limits>
using namespace std;

enum enMainMenuOptions { eShowClientList = 1, eAddClient = 2, eDeleteClient = 3, eUpdataClient = 4, eFindClient = 5, eTransacrion = 6, eManage_Users = 7, eLogOut = 8 };
enum eTransacrionsOptions { eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eMainMenu = 4 };
enum eManageUsers { eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMain_Menu = 6 };
enum enPermissions { eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64 };

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkDelete = false;
};

struct sUsers {
    string name;
    string password;
    int permissions=0;
    mutable bool markDelete = false;
};

const string FileName = "Clients.txt";
const string UserFileName = "users.txt";

// Global user
sUsers  CurrentUser;

void RunProgram(vector <sClient>& vClients, vector <sUsers> vUsers); // Declaration

// THe program start from having some clients on a file
// so we have to load this data from BankClients file
// and store them in a vector to use and print them
vector<string> SplitString(string S1, string Delim) {
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.  
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#") { // change vector to structure

    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double

    return Client;
}

vector <sClient> Load_Cleints_DataFromFile(string FileName) { //store client data to vector of structure
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);  //read Mode
    if (MyFile.is_open())
    {
        string Line;
        sClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void ShowAllClients(vector <sClient> vClients) {

    for (sClient c : vClients) {
        cout << "| " << setw(15) << left << c.AccountNumber << "| " << setw(12) << left << c.PinCode << "| " << setw(32) << left << c.Name << "| " << setw(16) << left << c.Phone << "| " << setw(15) << left << c.AccountBalance << " \n";

    }
    cout << endl;
    cout << "-------------------------------------------------------------------------------------------\n";
}

void ShowClientsList(vector <sClient> vClients) {
    cout << "                  Client List (" << vClients.size() << ") Client(s).                          \n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << " Account Number" << "| " << setw(12) << left << " Pin Code" << "| " << setw(32) << left << " Client Name " << "| " << setw(16) << left << " Phone Number " << "| " << setw(15) << left << " Balance \n";
    cout << "--------------------------------------------------------------------------------------------\n";
    ShowAllClients(vClients);
}



// Adding New Client and upload it into  BankClients file
vector <sClient> AddClient(vector <sClient>& vClients) {
    sClient Client;
    bool accountExists;

    do {
        accountExists = false;
        cout << "Enter Account Number (10 digits): ";
        getline(cin >> ws, Client.AccountNumber);

        // Validate if the account number is exactly 10 digits
        while (Client.AccountNumber.length() != 10 || !all_of(Client.AccountNumber.begin(), Client.AccountNumber.end(), ::isdigit)) {
            cout << "Invalid Account Number! It should be exactly 10 digits. Enter again: ";
            getline(cin, Client.AccountNumber);
        }

        for (const sClient& c : vClients) {
            if (c.AccountNumber == Client.AccountNumber) {
                accountExists = true;
                cout << "\nClient with [" << c.AccountNumber << "] already exists. Enter another account number: ";
                break;
            }
        }
    } while (accountExists);

    // Validate if the Pin Code is exactly 4 digits
    cout << "Enter Pin Code (4 digits): ";
    getline(cin, Client.PinCode);
    while (Client.PinCode.length() != 4 || !all_of(Client.PinCode.begin(), Client.PinCode.end(), ::isdigit)) {
        cout << "Invalid Pin Code! It should be exactly 4 digits. Enter again: ";
        getline(cin, Client.PinCode);
    }

    // Validate if the Phone Number is exactly 11 digits
    cout << "Enter Phone Number (11 digits): ";
    getline(cin, Client.Phone);
    while (Client.Phone.length() != 11 || !all_of(Client.Phone.begin(), Client.Phone.end(), ::isdigit)) {
        cout << "Invalid Phone Number! It should be exactly 11 digits. Enter again: ";
        getline(cin, Client.Phone);
    }

    // Validate Name
    cout << "Enter Name: ";
    getline(cin, Client.Name);
    while (!all_of(Client.Name.begin(), Client.Name.end(), [](char c) { return isalpha(c) || isspace(c); })) {
        cout << "Invalid Name! It should only contain letters and spaces. Enter again: ";
        getline(cin, Client.Name);
    }

    // Validate if the Account Number is positive
    cout << "Enter Account Balance: ";
    cin >> Client.AccountBalance;
    while (cin.fail() || Client.AccountBalance < 0) {
        cout << "Invalid Balance! Enter a positive number: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> Client.AccountBalance;
    }

    vClients.push_back(Client);

    cout << "Do you want to add more Clients ? Y/N" << endl;
    char choice;
    cin >> choice;
    if (tolower(choice) == 'y') { AddClient(vClients); }


    return vClients;
}

string ConvertRecordToLine(sClient c, string seperator = "#//#") {
    string Line = "";

    Line += c.AccountNumber + seperator;
    Line += c.PinCode + seperator;
    Line += c.Name + seperator;
    Line += c.Phone + seperator;
    Line += to_string(c.AccountBalance);
    return Line;
}

void Load_Cleints_DataToFile(string FileName, vector <sClient> vClients) {
    fstream MyFile;
    string line;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {
        for (const sClient& c : vClients) {
            MyFile << ConvertRecordToLine(c) << endl;
        }
        MyFile.close();
    }
}


// Delete Client and save it to a new vector to
// Note >> load new vector to the file is in function RunProgram()
vector <sClient> save_clients_in_New_vector_to_preform_Delete(vector <sClient>& vClients) {
    vector <sClient> vNewClients;
    for (sClient& c : vClients) {
        if (c.MarkDelete == false) {
            vNewClients.push_back(c);
        }
    }
    return vNewClients;
}

void PrintClientCard(sClient Client) {
    cout << "The Following are Client details:" << endl;
    cout << "-------------------------------------" << endl;
    cout << "Account NUmber: " << Client.AccountNumber << endl;
    cout << "Pin Code: " << Client.PinCode << endl;
    cout << "Name: " << Client.Name << endl;
    cout << "phone: " << Client.Phone << endl;
    cout << "Account Balance: " << Client.AccountBalance << endl;
    cout << "-------------------------------------" << endl;
}

vector <sClient> DeleteClient(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "     Delete client Screen     \n";
    cout << "------------------------------\n";

    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;
    vector <sClient> vNewClients;
    for (sClient& c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);
            cout << "Are you sure you want to delete this client? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y')) {
                c.MarkDelete = true;
                vNewClients = save_clients_in_New_vector_to_preform_Delete(vClients);
                cout << "Client Deleted Successfully." << endl;
            }
            return vNewClients;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}


// UpdateClientInfo must use pass by reference
vector <sClient> UpdateClientInfo(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "     Update client Screen     \n";
    cout << "------------------------------\n";
    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;
    for (sClient& c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);
            cout << "Are you sure you want to update this client? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y'))
            {
                c.AccountNumber = AccountNumber;
                cout << "Enter PinCode? ";
                getline(cin >> ws, c.PinCode);

                cout << "Enter Name? ";
                getline(cin, c.Name);

                cout << "Enter Phone? ";
                getline(cin, c.Phone);

                cout << "Enter AccountBalance? ";
                cin >> c.AccountBalance;

                cout << "Client updated successfully.\n";
            }
            return vClients;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}


void FindClient(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "     Find client Screen     \n";
    cout << "------------------------------\n";

    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;
    for (sClient& c : vClients) {
        if (c.AccountNumber == AccountNumber) {
            PrintClientCard(c);
            return;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
}

// Function to Inform the user to press any key to go back to the main menu
void clear() {
    cout << "\n\npress any key to go back to main menu...";
    system("pause");  // Pauses the execution and waits for the user to press any key
    system("cls");

}

enMainMenuOptions ShowMenuScreen() {
    cout << "================================================\n";
    cout << "             Main Menu Screen                   \n";
    cout << "================================================\n";
    cout << "            [1] Show Client List                \n";
    cout << "            [2] Add New Client                   \n";
    cout << "            [3] Delete  Client                   \n";
    cout << "            [4] Update  Client                   \n";
    cout << "            [5] Find Client                      \n";
    cout << "            [6] Transactions                      \n";
    cout << "            [7] Manage Users                      \n";
    cout << "            [8] Log Out                             \n";
    cout << "=================================================\n";

    cout << "choose What do you want to do ? [1 to 8]?";
    int Answer;
    cin >> Answer;
    return (enMainMenuOptions)Answer;
}


// --------------> Transaction section
eTransacrionsOptions ShowTransactionMenuScreen() {
    system("cls");
    cout << "================================================\n";
    cout << "             Transaction Menu Screen            \n";
    cout << "================================================\n";
    cout << "            [1] Deposit                          \n";
    cout << "            [2] Withdraw                         \n";
    cout << "            [3] Total Balances                   \n";
    cout << "            [4]  Main Menu                        \n";
    cout << "=================================================\n";

    cout << "What do you want to do ? [1 to 4]?";
    int Answer;
    cin >> Answer;
    return (eTransacrionsOptions)Answer;
}

vector <sClient> Deposit(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "       Deposit Screen         \n";
    cout << "------------------------------\n";
    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;

    for (sClient& c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);
            cout << "\nPlease enter deposit amount ?";
            double amount;
            cin >> amount;

            cout << "Are you sure you want to Preform this transaction? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y')) {
                c.AccountBalance += amount;
                Load_Cleints_DataToFile(FileName, vClients);
            }

            return vClients;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}

void TotalBalances(vector <sClient> vClients) {
    cout << "                  Client List (" << vClients.size() << ") Client(s).                          \n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << " Account Number" << "| " << setw(32) << left << " Client Name " << "| " << setw(15) << left << " Balance \n";
    cout << "--------------------------------------------------------------------------------------------\n";

    double TotalBalances = 0;
    for (sClient c : vClients) {
        cout << "| " << setw(15) << left << c.AccountNumber << "| " << setw(32) << left << c.Name << "| " << setw(15) << left << c.AccountBalance << " \n";
        TotalBalances += c.AccountBalance;
    }
    cout << endl;
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << "                                      total balances = " << TotalBalances;


}

vector <sClient> Withdraw(vector <sClient>& vClients) {
    cout << "------------------------------\n";
    cout << "       Withdraw Screen        \n";
    cout << "------------------------------\n";
    cout << "\n\n Please enter account Number?\n";
    string AccountNumber;
    cin >> AccountNumber;

    for (sClient& c : vClients) {
        if (AccountNumber == c.AccountNumber) {
            PrintClientCard(c);

            cout << "\nPlease enter withdraw amount ?";
            double amount;
            cin >> amount;
            while (amount > c.AccountBalance) {
                cout << "Amount Exceeds the balance,you can withdraw up to " << c.AccountBalance << "\n";
                cout << "\nPlease enter another amount ?";
                cin >> amount;
            }

            cout << "Are you sure you want to Preform this transaction? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y')) {
                c.AccountBalance -= amount;
                cout << "\nTransaction completed successfully. New balance: " << c.AccountBalance << endl;
                Load_Cleints_DataToFile(FileName, vClients);
            }
            else {
                cout << "\nTransaction canceled.\n";
            }
            return vClients;
        }
    }
    cout << "\nClient with [" << AccountNumber << "] does not exist \n";
    return vClients;
}

void RunTransaction(vector <sClient>& vClients, vector <sUsers> vUsers) {
    eTransacrionsOptions options;

    do {
        options = ShowTransactionMenuScreen();
        system("cls");
        switch (options) {
        case eTransacrionsOptions::eDeposit:
        {
            Deposit(vClients);
            clear();
            break;
        }
        case eTransacrionsOptions::eWithdraw:
        {
            Withdraw(vClients);
            clear();
            break;
        }
        case eTransacrionsOptions::eTotalBalances:
        {
            TotalBalances(vClients);
            clear();
            break;
        }
        case eTransacrionsOptions::eMainMenu:
        {
            RunProgram(vClients, vUsers);
            clear();
            break;
        }
        default: {
            cout << "Invalid Option. Please choose again.\n";
            break;
        }

        }
    } while (options != eTransacrionsOptions::eMainMenu);
}


// --------------> Manage Users section
string Convert_User_RecordToLine(sUsers c, string seperator = "#//#") {
    string Line = "";

    Line += c.name + seperator;
    Line += c.password + seperator;
    Line += to_string(c.permissions);
    return Line;
}

void Load_User_DataToFile(string FileName, vector <sUsers> &vUser) {
    fstream MyFile;
    string line;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {
        for (const sUsers& c : vUser) {
            MyFile << Convert_User_RecordToLine(c) << endl;
        }
        MyFile.close();
    }
}

sUsers Convert_User_LinetoRecord(string Line, string Seperator = "#//#") { // change vector to structure

    sUsers User;
    vector<string> vUsers;
    vUsers = SplitString(Line, Seperator);

    User.name = vUsers[0];
    User.password = vUsers[1];
    User.permissions = stoi(vUsers[2]);

    return User;
}

vector <sUsers> Load_Users_DataFromFile(string FileName) {
    vector <sUsers> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);  //read Mode
    if (MyFile.is_open())
    {
        string Line;
        sUsers user;
        while (getline(MyFile, Line)) {
            user = Convert_User_LinetoRecord(Line);
            vUsers.push_back(user);
        }
        MyFile.close();
    }
    return vUsers;
}


void  ShowAllUsers(vector <sUsers> vUsers) {
    for (const sUsers& c : vUsers) {
        cout << "| " << setw(15) << left << c.name << "| " << setw(12) << left << c.password << "| " << setw(32) << left << c.permissions << "\n";
    }
    cout << "-----------------------------------------------------------------------------------------------\n";
}

void users_List(vector <sUsers> vUsers) {
    cout << "                  User List (" << vUsers.size() << ") user(s). \n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    cout << "| " << setw(15) << left << " User Name" << "| " << setw(12) << left << " Password" << "| " << setw(32) << left << " Permissions \n";
    cout << "-----------------------------------------------------------------------------------------------\n";
    ShowAllUsers(vUsers);
}


vector <sUsers> AddNewUser(vector <sUsers>& vUsers) {

    sUsers User;
    bool accountExists;

    do {
        accountExists = false;
        cout << "Enter Username: ";
        getline(cin >> ws, User.name);


        for (const sUsers& c : vUsers) {
            if (c.name == User.name) {
                accountExists = true;
                cout << "\nUser with [" << c.name << "] already exists. Enter another Username: ";
                break;
            }
        }
    } while (accountExists);

    cout << "Enter Password: ";
    getline(cin >> ws, User.password);
    cout << "Do you want to give him full access? Y/N";
    char Answer;
    cin >> Answer;
    if (tolower(Answer) == tolower('Y'))  User.permissions = -1;
    else {
        cout << "Do you want to give access to: \n";

        cout << "Show Client list?\n";
        char Answer;
        cin >> Answer;
        if (tolower(Answer) == tolower('Y'))  User.permissions |= enPermissions::pListClients;

        cout << "Delete client?\n";
        cin >> Answer;
        if (tolower(Answer) == tolower('Y'))  User.permissions |= enPermissions::pDeleteClient;

        cout << "Update client?\n";
        cin >> Answer;
        if (tolower(Answer) == tolower('Y'))  User.permissions |= enPermissions::pUpdateClients;

        cout << "Find client?\n";
        cin >> Answer;
        if (tolower(Answer) == tolower('Y'))  User.permissions |= enPermissions::pFindClient;

        cout << "Add new client?\n";
        cin >> Answer;
        if (tolower(Answer) == tolower('Y'))  User.permissions |= enPermissions::pAddNewClient;
    }


    vUsers.push_back(User);

    return vUsers;
}


vector <sUsers> save_Users_in_New_vector_to_preform_Delete(vector <sUsers>& vUsers) {
    vector <sUsers> vNewUsers;
    for (sUsers& c : vUsers) {
        if (c.markDelete == false) {
            vNewUsers.push_back(c);
        }
    }
    return vNewUsers;
}

void PrintUserCard(sUsers user) {
    cout << "The Following are User details:" << endl;
    cout << "-------------------------------------" << endl;
    cout << "username: " << user.name << endl;
    cout << "Password: " << user.password << endl;
    cout << "permission: " << user.permissions << endl;
    cout << "-------------------------------------" << endl;
}

vector <sUsers> DeleteUser(vector <sUsers>& vUsers) {
    cout << "------------------------------\n";
    cout << "     Delete User Screen     \n";
    cout << "------------------------------\n";
    bool UserExists = false;

    cout << "\n\n Please enter Username?\n";
    string Answer;
    getline(cin >> ws, Answer);


    if (Answer == "Admin")
    {
        cout << "\n\nYou cannot Delete This User.";
        return vUsers;

    }

    for (sUsers& c : vUsers) {
        if (c.name == Answer) {
            PrintUserCard(c);
            cout << "Are you sure you want to delete this user? [y][n]?";
            char choice;
            cin >> choice;

            if (tolower(choice) == tolower('Y')) {
                c.markDelete = true;
                vUsers = save_Users_in_New_vector_to_preform_Delete(vUsers);
                cout << "user Deleted Successfully." << endl;
                UserExists = true;
                break;
            }
        }
    }
    if (!UserExists) cout << "User doesn't exist\n";

    return vUsers;
}


vector <sUsers> UpdateUserInfo(vector <sUsers>& vUsers) {
    cout << "------------------------------\n";
    cout << "     Update User Screen     \n";
    cout << "------------------------------\n";

    cout << "\n\n Please enter Username?\n";
    string Answer;
    getline(cin >> ws, Answer);

    for (sUsers& c : vUsers) {
        if (Answer == c.name) {
            PrintUserCard(c);
            cout << "Are you sure you want to update this user? [y][n]?";
            char choice;
            cin >> choice;
            if (tolower(choice) == tolower('Y'))
            {

                cout << "Enter Password? ";
                getline(cin, c.password);

                cout << "Do you want to give him full access? Y/N";
                char Answer;
                cin >> Answer;
                if (tolower(Answer) == tolower('Y'))  c.permissions = -1;
                else {
                    cout << "Do you want to give access to: \n";

                    cout << "Show Client list?\n";
                    cin >> Answer;
                    if (tolower(Answer) == tolower('Y'))  c.permissions += enPermissions::pListClients;

                    cout << "Delete client?\n";
                    cin >> Answer;
                    if (tolower(Answer) == tolower('Y'))  c.permissions += enPermissions::pDeleteClient;

                    cout << "Update client?\n";
                    cin >> Answer;
                    if (tolower(Answer) == tolower('Y'))  c.permissions += enPermissions::pUpdateClients;

                    cout << "Find client?\n";
                    cin >> Answer;
                    if (tolower(Answer) == tolower('Y'))  c.permissions += enPermissions::pFindClient;

                    cout << "Add new client?\n";
                    cin >> Answer;
                    if (tolower(Answer) == tolower('Y'))  c.permissions += enPermissions::pAddNewClient;
                }

            }
        }
    }
    return vUsers;
}



void FindUser(vector <sUsers>& vUsers) {
    cout << "------------------------------\n";
    cout << "     Find User Screen     \n";
    cout << "------------------------------\n";

    cout << "\n\n Please enter username?\n";
    string Ans;
    cin >> Ans;
    for (sUsers& c : vUsers) {
        if (c.name == Ans) {
            PrintUserCard(c);
            return;
        }
    }
    cout << "\nuser with [" << Ans << "] does not exist \n";
}

// Function to check if a user has a specific permission
// using a bitwise operation to determine if the specific
// permission is included in the user's permissions.
// If the user has full access (eAll) or the specific
// permission is set, the function returns true; otherwise, it returns false.
bool CheckPermission(const sUsers& user, enPermissions permission) {
    if (user.permissions == enPermissions::eAll || (user.permissions & permission) != 0) {
        return true;
    }
    return false;
}

eManageUsers showManageUsersScreen() {
    system("cls");
    cout << "================================================\n";
    cout << "           Manage users Menu Screen            \n";
    cout << "================================================\n";
    cout << "            [1] List users                      \n";
    cout << "            [2] Add new users                   \n";
    cout << "            [3] Delete user                     \n";
    cout << "            [4] Update user                     \n";
    cout << "            [5]  Find user                      \n";
    cout << "            [6]  Main Menu                      \n";
    cout << "=================================================\n";

    cout << "What do you want to do ? [1 to 6]?";
    int Answer;
    cin >> Answer;
    return (eManageUsers)Answer;
}

void RunManageUsers(vector <sUsers>& vUsers) {

    eManageUsers options;

    do {
        options = showManageUsersScreen();
        system("cls");
        switch (options) {
        case eManageUsers::eAddNewUser:
        {
            AddNewUser(vUsers);
            Load_User_DataToFile(UserFileName, vUsers);
            clear();
            break;
        }
        case eManageUsers::eDeleteUser:
        {
            DeleteUser(vUsers);
            Load_User_DataToFile(UserFileName, vUsers);
            clear();
            break;
        }
        case eManageUsers::eUpdateUser:
        {
            UpdateUserInfo(vUsers);
            Load_User_DataToFile(UserFileName, vUsers);
            clear();
            break;
        }
        case eManageUsers::eFindUser:
        {
            FindUser(vUsers);
            clear();
            break;
        }
        case eManageUsers::eListUsers:
        {
            users_List(vUsers);
            clear();
            break;
        }
        case eManageUsers::eMain_Menu:
        {
            ShowMenuScreen();
            break;
        }
        default: {
            cout << "Invalid Option. Please choose again.\n";
            break;
        }
        }
        }while (options != eManageUsers::eMain_Menu);
}

void loginScreen(vector <sUsers> vUsers) {
    system("cls");
    cout << "====================================================\n";
    cout << "                   Login screen                       \n ";
    cout << "====================================================\n";

    bool found = false;

    do {
        cout << "Enter Username: ";
        string Username;
        getline(cin>>ws, Username);
        cout << "Enter Password: ";
        string Password;
        getline(cin, Password);
        for (const sUsers& c : vUsers) {
            if (c.name == Username && c.password == Password) {
                found = true;
                CurrentUser = c;
                break;
            }
        }

        if (!found) {
            cout << "Invalid Username or Password!!\n";
        }
    } while (!found);

}


void Exit(vector <sUsers> vUsers) {
    system("cls");
    cout << "------------------------------\n";
    cout << "           Log out            \n";
    cout << "------------------------------\n";
    loginScreen(vUsers);
}




// function to Execute the whole program functions
void RunProgram(vector <sClient>& vClients, vector <sUsers> vUsers) {


    enMainMenuOptions  option;

    loginScreen(vUsers);
  

    do {
        system("cls");
        option = ShowMenuScreen();
        switch (option) {
        case enMainMenuOptions::eShowClientList:
        {
            if (CheckPermission(CurrentUser, pListClients)) {
                system("cls");
                ShowClientsList(vClients);
            }
            else cout << "User not have acccess to this feature";
            clear();
            break;
        }
        case enMainMenuOptions::eAddClient:
        {
            if (CheckPermission(CurrentUser, pAddNewClient)) {
                system("cls");
                AddClient(vClients);
                Load_Cleints_DataToFile(FileName, vClients);
            }
            else cout << "User not have acccess to this feature";
            clear();
            break;
        }
        case enMainMenuOptions::eDeleteClient:
        {
            if (CheckPermission(CurrentUser, pDeleteClient)) {
                system("cls");
                vClients = DeleteClient(vClients);
                Load_Cleints_DataToFile(FileName, vClients);
            }
            else cout << "User not have acccess to this feature";
            clear();
            break;
        }
        case enMainMenuOptions::eUpdataClient:
        {
            if (CheckPermission(CurrentUser, pUpdateClients)) {
                system("cls");
                UpdateClientInfo(vClients);
                Load_Cleints_DataToFile(FileName, vClients);
            }
            else cout << "User not have acccess to this feature";
            clear();
            break;
        }
        case enMainMenuOptions::eFindClient:
        {

            if (CheckPermission(CurrentUser, pFindClient)) {

                system("cls");
                FindClient(vClients);
            }
            else cout << "User not have acccess to this feature";
            clear();
            break;
        }
        case enMainMenuOptions::eTransacrion:
        {
            if (CheckPermission(CurrentUser, pTranactions)) {
                system("cls");
                RunTransaction(vClients, vUsers);
            }
            else cout << "User not have acccess to this feature";
            clear();
            break;
        }
        case enMainMenuOptions::eManage_Users:
        {
            if (CheckPermission(CurrentUser, pManageUsers)) {
                system("cls");
                RunManageUsers(vUsers);
            }
            else cout << "User not have acccess to this feature";
            clear();
            break;
        }

        case enMainMenuOptions::eLogOut:
        {
            system("cls");
            Exit(vUsers);
            break;
        }
        default: {
            cout << "Invalid Option. Please choose again.\n";
            break;
        }

        }
    } while (true);
}

int main() {
    vector <sClient> vClients = Load_Cleints_DataFromFile(FileName);
    vector <sUsers> vUsers = Load_Users_DataFromFile(UserFileName);

    RunProgram(vClients,vUsers);


    system("pause>0");
}