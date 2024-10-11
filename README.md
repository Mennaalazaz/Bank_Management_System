# Bank_Management_System
This is a C++ program for a basic bank management system. Here's its functionalities:

Data Structures:
- sClient: This structure stores information about a client, including account number, pin code, name, phone number, and account balance.
- sUsers: This structure stores information about a user, including name, password, and permissions.


File Operations:
-The program uses functions like Load_Cleints_DataFromFile and Load_Users_DataFromFile to read client and user data from text files (Clients.txt and users.txt) into vectors.
-Functions like Load_Cleints_DataToFile and Load_User_DataToFile are used to write updated client and user data back to the respective files.


User Interface:
-The program uses a menu-driven interface with different options for both clients and users based on their permissions.
-Functions like ShowMenuScreen and ShowTransactionMenuScreen display the menus and capture user input.


Client Management:
-The program allows adding new clients with basic validation for account number, pin code, phone number, name, and balance using the AddClient function.
-It also allows searching for existing clients, updating client information, and deleting clients (marked for deletion and saved on file update) using dedicated functions.


Transaction Management:
-Users can perform deposit, withdrawal, and view total balances for all clients using functions like Deposit, Withdraw, and TotalBalances.


User Management (Optional):
The program has functionalities for managing users (assumed to be administrators) with different permission levels. However, the provided code snippet only shows functions for loading user data and displaying a user list.


Error Handling:
The program includes basic validation for user input like account number format, pin code length, phone number format, and non-negative balance.
