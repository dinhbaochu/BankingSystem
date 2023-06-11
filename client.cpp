#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

string float_to_string(float numb){
        stringstream s;
        s << numb;
        return s.str();
}

int main(int argc, char const* argv[])
{
    
	int status, valread, client_fd;
	struct sockaddr_in serv_addr;
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(8081);

	// Convert IPv4 and IPv6 addresses from text to binary
	// form
	if (inet_pton(AF_INET, "127.16.19.255", &serv_addr.sin_addr)
		<= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((status
		= connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr)))
		< 0) {
		printf("\nConnection Failed \n");
		return -1;
	}


	// closing the connected socket
	//close(client_fd);

    int option = 0, count = 0;
    string message, message_server;
    string dash = "_";
    while(option != 3){
            do{
                if (count == 3){
                    option = 3;
                    cout << "You have input wrong 3 times. ";
                    count = 0;
                    break;
                }
                if (count > 0){
                    cout << "Wrong input. You have " << 3 - count << " more tries" <<endl; 
                }
                cout << "Welcome our customer" << endl;
                cout << "1. Create account" << endl;
                cout << "2. Log in" << endl;
                cout << "3. End program" << endl;
                cout << "Choose an option as number(Ex: 1): ";
                cin >> option;
                count += 1;
            }while(option!= 1 && option != 2 && option != 3);
        count = 0;
        switch(option) {
            case 1:
            {
                string name, password, currency;
                float balance = 0;
                cout << "Please fill in these information:" << endl;
                cout << "Your name: ";
                cin >> name;
                while(count < 3 && password.size() < 6){
                    cout << "Your password(At least 6 characters): ";
                    cin >> password;
                    if (password.size() < 6){
                        count += 1;
                        if (count == 3){
                        cout << "You have input wrong 3 times. Program terminated...";
                        option = 3;
                        count = 0;
                        break;
                    }
                        cout << "Wrong input. You have " << 3 - count << " more tries" <<endl; 
                    }
                }
                count = 0;
                if (option == 3){
                    break;
                }
                while(count < 3 && balance <=0){
                    cout << "Initial balance(Must be positive number): ";
                    cin >> balance;
                    if (balance <= 0){
                        count += 1;
                        if (count == 3){
                        cout << "You have input wrong 3 times. Program terminated...";
                        option = 3;
                        count = 0;
                        break;
                    }
                        cout << "Wrong input. You have " << 3 - count << " more tries" <<endl; 
                    }
                }
                count = 0;
                if (option == 3){
                    break;
                }
                while(count < 3 && currency != "VND" && currency != "USD"){
                    cout << "Currency(VND or USD): ";
                    cin >> currency;
                    if (currency != "VND" && currency != "USD"){
                        count += 1;
                        if (count == 3){
                        cout << "You have input wrong 3 times. Program terminated...";
                        option = 3;
                        count = 0;
                        break;
                    }
                        cout << "Wrong input. You have " << 3 - count << " more tries" <<endl; 
                    }
                }
                count = 0;
                if (option == 3){
                    break;
                }
                string s_balance = float_to_string(balance);
                message = "1" + dash + name + dash + password + dash + s_balance + dash + currency;
               
            // send message to server
            char *p= &message[0];
            send(client_fd, p, strlen(p), 0);
	        printf("Waiting... \n");
	       
            
            // Receive message from server. Message should be accountNumber or "Account already exists"
            valread = read(client_fd, buffer, 1024); 
	      
            if (buffer[0] !='I'){
                 cout << "Account created successfully! Your account number is: "<<buffer<<endl;
                
            }
            else{
                cout << buffer << endl; // "Account already exists"
            }
            break;
        }
        case 2:
        {   
            string accountNumber, password;
            cout<< "Your account number: ";
            cin >> accountNumber;
            cout<< "Password: ";
            cin >> password;
            message = "2" + dash + accountNumber + dash + password;
            // send message to serverd
            char *p= &message[0];
            send(client_fd, p, strlen(p), 0);
	        printf("Waiting ... \n");
            
            // Receive message from server
            valread = read(client_fd, buffer, 1024); 
            
            message_server = buffer; // Dòng này là để test các function sau khi log in thôi còn Hoàng sẽ send message server đến Nguyên
            if (message_server[0] =='S'){
                cout << message_server << "LOGGED IN!" << endl;
                int sw=0;
                while(option != 6){
                    cout <<"Please choose one of the following:"<< endl;
                    cout << "1. Make transaction" << endl;
                    cout << "2. Update account" << endl;
                    cout << "3. Convert currency" << endl;
                    cout << "4. View transaction history" << endl;
                    cout << "5. View balance" << endl;
                    cout << "6. Delete account" << endl;
                    cout << "7. Log out" << endl;
                    cout << "- Your option: ";
                    cin >> option;
                    switch(option){
                        case 1: // Make transaction
                        {
                            string destination_account, amount;
                            cout << "Destination account: ";
                            cin >> destination_account;
                            cout << "Amount: ";
                            cin >> amount;
                            message = "3" + dash + accountNumber + dash + destination_account + dash + amount;
                            // send message to server
                            char *p= &message[0];
            send(client_fd, p, strlen(p), 0);
	        printf("Waiting ... \n");
         
                            // Receive message from server
                            valread = read(client_fd, buffer, 1024); 
	                        printf("%s\n", buffer);
                            break;
                        }
                        case 2: // Update account
                        {
                            string name, balance;
                            cout << "Your name: ";
                            cin >> name;
                            cout << "New balance: ";
                            cin >> balance;
                            message = "4" + dash + accountNumber + dash + name + dash + balance;
                            // send message to server
            char *p= &message[0];
            send(client_fd, p, strlen(p), 0);
	        printf("Waiting \n");
           
                            // Receive message from server
                            valread = read(client_fd, buffer, 1024); 
	                        printf("%s\n", buffer);
                            break;
                        }
                        case 3: // Convert currency
                        {
                            string new_currency;
                            cout << "Your new currency: ";
                            cin >> new_currency;
                            message = "5" + dash + accountNumber + dash + new_currency;
                            // send message to server
                            char *p= &message[0];
                            send(client_fd, p, strlen(p), 0);
	                        printf("Waiting ... \n");
                            // Receive message from server
                            valread = read(client_fd, buffer, 1024); 
	                        printf("%s\n", buffer);
                            break;
                        }
                        
                        case 4: // View transaction history
                        {
                            message = "6" + dash + accountNumber;
                            // send message to server
                            char *p= &message[0];
                            send(client_fd, p, strlen(p), 0);
	                        printf("Waiting ... \n");
                            // Receive message from server
                            valread = read(client_fd, buffer, 1024); 
	                        printf("%s\n", buffer);
                            break;
                        }
                        case 5: // view balance
                        {
                            message="8" + dash + accountNumber;
                            char *p= &message[0];
                            send(client_fd, p, strlen(p), 0);
                            valread = read(client_fd, buffer, 1024); 
                            cout << "Your balance: " << buffer << endl; 
                            break;
                        }
                        case 6: // Remove account
                        {
                            message = "7" + dash + accountNumber;
                            // send message to server
                            char *p= &message[0];
                            send(client_fd, p, strlen(p), 0);
	                        printf("So sad ... \n");
                            // Receive message from server
                            valread = read(client_fd, buffer, 1024); 
	                        printf("%s\n", buffer);
                            option = 6;
                            break;
                        }
                        case 7: // log out
                        {
                            option = 6;
                            message = "Log out"; 
                            cout << "Log out..." << endl;
                        }
                        
                    }
                }
            }
            else{
                
                // Print message "Log in failed. Please try again"
                printf("%s\n", buffer);
            }
        }
        case 3: // End program
        {
            cout << "Program terminated..." << endl;
        }
    }
}
return 0;
}