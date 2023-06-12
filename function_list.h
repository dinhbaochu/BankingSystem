#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>

using namespace std;

// Enum representing two supported currencies
enum Currency {
    USD,
    VND
};

// Function to convert string to the enum Currency
Currency stringToCurrency (const string& currency) {
    // const string&: make the string currency unchanged: save memory
    if (currency == "USD") {
        return Currency::USD;
    }
    else if (currency == "VND") {
        return Currency:: VND;
    }
    else {
        // Only support for VND and USD
        throw invalid_argument ("Invalid currency type.");
    }
}

// Function to convert the enum Currency to string
string currencyToString (Currency currency) {
    switch (currency) {
        case Currency::USD:
            return "USD";

        case Currency::VND:
            return "VND";
        
        default:
            return "Unknown";
    }
}

// Generate UserID
std::string generateUserId() {
    static int lastNumber = 0; // Initialize with a value lower than the desired range
    
    lastNumber++; // Increment the last generated number

    std::ostringstream oss;
    oss << std::setw(5) << std::setfill('0') << lastNumber;

    return oss.str();
}

// Generate transaction ID
std::string generate_transaction_ID() {
    static int transactionCount = 0; // Initialize with 0
    
    transactionCount++; // Increment the transaction count

    // Convert the count to a 5-digit string with leading zeros
    std::ostringstream oss;
    oss << "TECH" << std::setw(5) << std::setfill('0') << transactionCount;

    return oss.str();
}

// Represents a single transaction
class Transaction {
private:
    string transactionID;
    string sourceAccount;
    string destinationAccount;
    float amount;

public:
    // Constructor to initialize the Transaction
    Transaction(string transactionID, string sourceAccount, string destinationAccount, float amount) 
        : transactionID(transactionID), sourceAccount(sourceAccount), 
        destinationAccount(destinationAccount), amount(amount) {}

    // Getters for the class properties
    string getTransactionID() const {
        return transactionID;
    }

    string getSourceAccount() const {
        return sourceAccount;
    }

    string getDestinationAccount() const {
        return destinationAccount;
    }

    float getAmount() const {
        return amount;
    }
};

// Represents an account
class Account {
private:
    string accountNumber;
    string name;
    string password;
    float balance;
    Currency currency;
    // The vector to keep track of all transactions for this account
    vector<Transaction*> history; 

public:
    // Constructor to initialize the Account
    Account(string accountNumber, string password, string name, float balance, string currency) 
        : accountNumber(accountNumber), password(password), name(name), 
        balance(balance), currency(stringToCurrency(currency)) {}

    // Getters and Setters for the class properties
    string getAccountNumber() const {
        return accountNumber;
    }

    string getName() const {
        return name;
    }

    void setName(string name) {
        this->name = name;
    }

    float getBalance() const {
        return balance;
    }

    void setBalance(float newBalance) {
        balance = newBalance;
    }

    string getPassword () const {
        return password;
    }

    Currency getCurrency() const {
        return currency;
    }

    void setCurrency(Currency currency) {
        this->currency = currency;
    }

    vector<Transaction*> getHistory() const {
        return history;
    }

    // Method to add a transaction to this account's history
    void addTransactionToHistory(Transaction* transaction) {
        history.push_back(transaction);
    }
};

// Represents an owner with multiple accounts
class Owner {
private:
    string name;
    /*
    unordered_map is used to store accounts and transactions. 
    Each account is mapped to an account number, and each transaction is mapped to a transaction ID.
    */ 
    unordered_map<string, Account*> accounts; // Map to hold account objects, with account number as key
    unordered_map<string, Transaction*> transactions; // Map to hold transaction objects, with transaction ID as key

public:
    // Constructor to initialize the Owner
    Owner(string name) : name(name) {}

    // 1: Methods to add, remove and update an account
    Account* addAccount(string accountNumber, string name, string password, float balance, string currency) {
        /* Creates a new Account object on the heap,
        accounts[accountNumber]: assigns  new object to the map accounts where the key is accountNumber,
        the value is the pointer to the newly created Account object.
        */
        string message; // Declare the message string

        if (accounts.find(accountNumber) != accounts.end()) {
            message = "Account already exists.";
        } else {
            Account* newAccount = new Account(accountNumber, name, password, balance, currency);
            accounts[accountNumber] = newAccount;
            return newAccount;
        }

        return nullptr;
    }

    // 2: Method to log in
    Account* log_in(string accountNumber, string password) {
        if (accounts.find(accountNumber) != accounts.end()) {
            return accounts[accountNumber];
        } else {
            return nullptr;
        }
    }

    // 3: Method to add a transaction
    pair<string, string>* addTransaction(string transactionID, string sourceAccount, string destinationAccount, float amount) {
    // new Transaction object is created on the heap
    Transaction* t = new Transaction(transactionID, sourceAccount, destinationAccount, amount);

    // Declare the message strings for the source and the destination
    string messageSource, messageDestination;

    // Check if source and destination accounts exist
    if (accounts.find(sourceAccount) == accounts.end() || accounts.find(destinationAccount) == accounts.end()) {
        string errorMessage = "Invalid source or destination account.";
        pair<string, string>* messages = new pair<string, string>(errorMessage, errorMessage);
        return messages;
    }

    // retrieve Account object from the map, we can modify properties and methods of sourceAcc account
    Account* sourceAcc = accounts[sourceAccount];
    Account* destAcc = accounts[destinationAccount];

    // checking if the balance of the sourceAcc account is greater than or equal to the transaction amount
    if (sourceAcc->getBalance() >= amount) {
        sourceAcc->setBalance(sourceAcc->getBalance() - amount);
        sourceAcc->addTransactionToHistory(t);
        messageSource = "Transaction successfully executed. Your balance now is " + to_string(sourceAcc->getBalance());

        destAcc->setBalance(destAcc->getBalance() + amount);
        destAcc->addTransactionToHistory(t);
        messageDestination = "You received " + to_string(amount) + ". Your balance now is " + to_string(destAcc->getBalance());
    } 
    else {
        messageSource = "Insufficient funds in the source account.";
        messageDestination = "Failed due to insufficient funds in source account.";
    }

    // new Transaction object is then added to the transactions map only if the transaction was successful
    if (messageSource == "Transaction successfully executed. Your balance now is " + to_string(sourceAcc->getBalance())) {
        transactions[transactionID] = t;
    }

    pair<string, string>* messages = new pair<string, string>(messageSource, messageDestination);
    return messages;
}

    // 4: Method to update an account
    string* updateAccount(string accountNumber, string name, float balance) {
        string* message = new string;

        if (accounts.find(accountNumber) == accounts.end()) {
            *message = "Account does not exist.";
        } else {
            accounts[accountNumber]->setName(name);  // Access the setName method of the Account object: updating the account name
            accounts[accountNumber]->setBalance(balance);  // Access the setBalance method of the Account object: updating the account balance
            *message = "Account updated successfully.";
        }

        return message;
    }
    // 4.5 method to view  balance 
    int viewBalance (string accountNumber){
        Account *account1 = accounts[accountNumber];
        int message = account1->getBalance();
        return message;
    }

    // 5: Method to convert the currency of an account
    // Change the Currency value of the Account object and adjusting the balance accordingly.
    void currencyConversion(string accountNumber, string newCurrencyStr) {
        Currency newCurrency = stringToCurrency(newCurrencyStr); //convert the string representation of the new currency to corresponding Currency enum value.
        
        // check if the current currency of the account is not the same as the new currency:
        if (accounts.find(accountNumber) != accounts.end()) {
            if (accounts[accountNumber]->getCurrency() != newCurrency) {
                if (newCurrency == Currency::VND) {
                    accounts[accountNumber]->setBalance(accounts[accountNumber]->getBalance() * 23000);
                }

                else {
                    accounts[accountNumber]->setBalance(accounts[accountNumber]->getBalance() / 23000);
                }
                
                accounts[accountNumber]->setCurrency(newCurrency);
            }
        }
    }

    // 6: Method to view transaction history of account
    vector<string>* viewAccountTransactionHistory(string accountNumber) {
        vector<string>* historyMessages = new vector<string>;

        if (accounts.find(accountNumber) != accounts.end()) {
            // Iterate over the transaction history of the account
            for (auto transaction : accounts[accountNumber]->getHistory()) {
                string transactionDetails = "Transaction ID: " + transaction->getTransactionID() + "\n" +
                                            "Source Account: " + transaction->getSourceAccount() + "\n" +
                                            "Destination Account: " + transaction->getDestinationAccount() + "\n" +
                                            "Amount: " + to_string(transaction->getAmount()) + "\n";
                historyMessages->push_back(transactionDetails);
            }
        }

        return historyMessages;
    }
    
    // 7: Method to remove an account
    string* removeAccount(string accountNumber) {
        string* message = new string;
        // Checks if an account with the given account number exists in the map
        if (accounts.find(accountNumber) == accounts.end()) {
            *message = "Account does not exist.";
        } else {
            for (auto transaction : accounts[accountNumber]->getHistory()) {
                transactions.erase(transaction->getTransactionID());
            }

            delete accounts[accountNumber];
            accounts.erase(accountNumber);

            *message = "Account removed successfully.";
        }

        return message;
    }
};
int new_socket, valread;

int create(int PORT){
	

	static int server_fd;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	


	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt,
				sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address,
			sizeof(address))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket
		= accept(server_fd, (struct sockaddr*)&address,
				(socklen_t*)&addrlen))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	return new_socket; 
	}
  ////////// send noti to destination
void send_message(std::string s2 ){
    int server_fd2, new_socket2;
	struct sockaddr_in address2;
	int opt2 = 1;
	int addrlen2 = sizeof(address2);
	


	// Creating socket file descriptor
	if ((server_fd2 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd2, SOL_SOCKET,
				SO_REUSEADDR | SO_REUSEPORT, &opt2,
				sizeof(opt2))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address2.sin_family = AF_INET;
	address2.sin_addr.s_addr = INADDR_ANY;
	address2.sin_port = htons(8082);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd2, (struct sockaddr*)&address2,
			sizeof(address2))
		< 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd2, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket2
		= accept(server_fd2, (struct sockaddr*)&address2,
				(socklen_t*)&addrlen2))
		< 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
    char buffer2[1024] = { 0 };

                   
                    const int length2 = s2.length();   
                    char* char_array2 = new char[length2 + 1];                 
                    strcpy(char_array2, s2.c_str());
                    int valread2 = read(new_socket2, buffer2, 1024);	                    
	                send(new_socket2, char_array2 , length2 +1 , 0);}
                        
                    
