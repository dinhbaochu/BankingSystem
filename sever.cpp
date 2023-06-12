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
#include "function_list.h"

using namespace std;

int main(int argc, char const* argv[])
{
    new_socket=create (8081);

    // Initialize the Owner
    Owner owner("Techcombank");
    while (true) {
        //process input

    char buffer[1024] = { 0 };
	valread = read(new_socket, buffer, 1024);
	printf("Message from client: %s\n", buffer);
		
    string information = buffer; // info sent from client

        // Using istringstream for splitting string by '_'
        istringstream iss(information);
        vector<string> tokens;
        string token;
        while (getline(iss, token, '_')) {
            tokens.push_back(token);
        }

        // Switch case on first character of information
        int command = stoi(tokens[0]);  // convert string to integer

    switch (command) {
        case 1:  // addAccount
            if (tokens.size() == 5) {
                try {
                    Account* account = owner.addAccount(generateUserId(), tokens[2], tokens[1], stof(tokens[3]), tokens[4]);
                    if (account != nullptr) {

                        std::string s = account->getAccountNumber();
                        const int length = s.length();
                        char* char_array = new char[length + 1];
                        strcpy(char_array, s.c_str()); 
                        send(new_socket, char_array , length +1 , 0);
                        
                        cout << "Account created successfully. Account Number: " << account->getAccountNumber() << endl;
                        
                    } else {
                        cout << "Failed to create account. Account already exists." << endl;
                        std::string s = "Failed to create account. Account already exists.";
                        const int length = s.length();
                        char* char_array = new char[length + 1];
                        strcpy(char_array, s.c_str()); 
                        send(new_socket, char_array , length +1 , 0);
                            
                    }
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid balance" << endl;
                    std::string s = "Invalid balance";
                    const int length = s.length();
                    char* char_array = new char[length + 1];
                    strcpy(char_array, s.c_str()); 
                    send(new_socket, char_array , length +1 , 0);
                }
            }
            else {
                cout << "Invalid parameters for addAccount" << endl;
                std::string s = "Invalid parameters for addAccount";
                const int length = s.length();
                char* char_array = new char[length + 1];
                strcpy(char_array, s.c_str()); 
                send(new_socket, char_array , length +1 , 0);
            }
            break;

        case 2:  // log_in
            if (tokens.size() == 3) {
                Account* account = owner.log_in(tokens[1], tokens[2]);
                if (account != nullptr) {
                    if (tokens[2] == account->getPassword()){
                     
                    std::string s = "SUCCESSFULLY " ;
                    const int length = s.length();
                    char* char_array = new char[length + 1];
                    strcpy(char_array, s.c_str());
                    printf ("%s \n", char_array);
                    send(new_socket, char_array , length +1 , 0);
                    }
                else {
                    std::string s = "WRONG PASSWORD!" ;
                    const int length = s.length();
                    char* char_array = new char[length + 1];
                    strcpy(char_array, s.c_str());
                    printf ("%s \n", char_array);
                    send(new_socket, char_array , length +1 , 0);
                }

                } else {
                    cout << "Failed to log in. User not found." << endl;
                    std::string s = "Failed to log in. User not found." ;
                    const int length = s.length();
                    char* char_array = new char[length + 1];
                    strcpy(char_array, s.c_str());
                    
                    send(new_socket, char_array , length +1 , 0);
                }
            } else {
                cout << "Invalid parameters for log_in" << endl;
                std::string s = "Invalid parameters for log_in" ;
                    const int length = s.length();
                    char* char_array = new char[length + 1];
                    strcpy(char_array, s.c_str());
                    printf ("%s", char_array);
                    send(new_socket, char_array , length +1 , 0);
            }
            break;

        case 3:  // addTransaction
            if (tokens.size() == 4) {
                try {
                    pair<string, string>* messages = owner.addTransaction(generate_transaction_ID(), tokens[1], tokens[2], stof(tokens[3]));
                    cout << "Source: " << messages->first << "\nDestination: " << messages->second << endl;
                        std::string s = "Source: " + messages->first ; 
                        const int length = s.length();
                        char* char_array = new char[length + 1];
                        strcpy(char_array, s.c_str());
                        send(new_socket, char_array , length +1 , 0);
                        std::string test = messages->first;
                    if (test[0] == 'T' ){
                  //// sent noti to destinated client
                  std::string s2= "Destination: " + messages->second;
                  send_message(s2);
                  //////
                }
                }
                 catch (const std::invalid_argument& e) 
                 {
                    cout << "Invalid transaction amount" << endl;
                    std::string s = "Invalid transaction amount" ; 
                        const int length = s.length();
                        char* char_array = new char[length + 1];
                        strcpy(char_array, s.c_str());
                        send(new_socket, char_array , length +1 , 0);
                }
            }
             else {
                cout << "Invalid parameters for addTransaction" << endl;
                std::string s = "Invalid transaction amount" ; 
                        const int length = s.length();
                        char* char_array = new char[length + 1];
                        strcpy(char_array, s.c_str());
                        send(new_socket, char_array , length +1 , 0);
            }
            break;

        case 4:  // updateAccount
            if (tokens.size() == 4) {
                try {
                    string* message = owner.updateAccount(tokens[1], tokens[2], stof(tokens[3]));
                    cout << *message << endl;
                    std::string s = *message ;
                            
                    const int length = s.length();
    
                    char* char_array = new char[length + 1];
  
                        strcpy(char_array, s.c_str());
                     
                        send(new_socket, char_array , length +1 , 0);
                    delete message;
                } catch (const std::invalid_argument& e) {
                    cout << "Invalid new balance" << endl;
                    std::string s = "Invalid new balance"  ;
                            
                    const int length = s.length();
    
                    char* char_array = new char[length + 1];
  
                        strcpy(char_array, s.c_str());
                        
                        send(new_socket, char_array , length +1 , 0);
                }
            } else {
                cout << "Invalid parameters for updateAccount" << endl;
                   std::string s = "Invalid parameters for updateAccount" ;
                            
                    const int length = s.length();
    
                    char* char_array = new char[length + 1];
  
                        strcpy(char_array, s.c_str());
                       
                        send(new_socket, char_array , length +1 , 0);
            }
            break;

        case 5:  // currencyConversion
            if (tokens.size() == 3) {
                owner.currencyConversion(tokens[1], tokens[2]);
                cout << "Currency conversion completed." << endl;
                std::string s = "Currency conversion completed."  ;
                            
                    const int length = s.length();
    
                    char* char_array = new char[length + 1];
  
                        strcpy(char_array, s.c_str());
                       
                        send(new_socket, char_array , length +1 , 0);
            } else {
                cout << "Invalid parameters for currencyConversion" << endl;
                std::string s = "Invalid parameters for currencyConversion"  ;
                            
                    const int length = s.length();
    
                    char* char_array = new char[length + 1];
  
                        strcpy(char_array, s.c_str());
                    
                        send(new_socket, char_array , length +1 , 0);
            }
            break;

        case 6:  // viewAccountTransactionHistory
            if (tokens.size() == 2) {
                vector<string>* history = owner.viewAccountTransactionHistory(tokens[1]);
                string response = "History for " + tokens[1] + ":\n";

                for (const auto& transaction : *history) {
                    response += transaction + "\n";
                }

                delete history;
                cout << response;
                std::string s = response;     
                const int length = s.length();
                char* char_array = new char[length + 1];
                strcpy(char_array, s.c_str());
                
                send(new_socket, char_array , length +1 , 0);
            } 
            else {
                cout << "Invalid parameters for viewAccountTransactionHistory\n";
            }
            break;

        case 7:  // removeAccount
            if (tokens.size() == 2) {
                string* message = owner.removeAccount(tokens[1]);
                cout << *message << endl;

                std::string s = *message;       
                const int length = s.length();
                char* char_array = new char[length + 1];
                strcpy(char_array, s.c_str());
               
                send(new_socket, char_array , length +1 , 0);

                delete message;
            } else {
                cout << "Invalid parameters for removeAccount" << endl;
                std::string s = "Invalid parameters for viewAccountTransactionHistory";
                            
                    const int length = s.length();
    
                    char* char_array = new char[length + 1];
  
                        strcpy(char_array, s.c_str());
                       
                        send(new_socket, char_array , length +1 , 0);
            }
            break;
        case 8: //view ballance
            if (tokens.size()==2){
                int s1 = owner.viewBalance(tokens[1]);
               
            std::string s = to_string(s1);
                const int length = s.length();
                char* char_array = new char[length + 1];
                strcpy(char_array, s.c_str());
                printf ("%s \n", char_array);
                send(new_socket, char_array , length +1 , 0);
            }
            else{
                cout << "Invalid parameters for removeAccount" << endl;
            }
            break;

        default:
            cout << "Invalid command" << endl;
            break;
    }

    }

    return 0;
}
