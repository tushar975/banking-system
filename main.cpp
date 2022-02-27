

#include <iostream>
#include <fstream>
#include <regex>
#include <typeinfo>
#include "json/json.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

Json::Value getBankRecords() {
	Json::Value root;
	std::ifstream file("output.json");
    file >> root;

	file.close();
	return root;
}


void setBankRecords(Json::Value root) {
	// Write the output to a file
	std::ofstream outFile;
	outFile.open("output.json");
	outFile << root;
	outFile.close();    
}


string getName() {
	string str;
	cout << "Enter Account Holder's Name: ";
	getline(cin, str);

	if(str == "") {
		cout << "Enter a name!" << endl;
		getName();
	}
	return str;
}


int bankOptions() {
	cout << "1. Deposit Amount"<< endl;
	cout << "2. Withdraw Amount"<< endl;
	cout << "3. Balance Enquiry"<< endl;
	cout << "4. New Account"<< endl;
	cout << "5. Modify Account"<< endl;
	cout << "6. Close Account"<< endl;
	cout << "7. All Account Holder List"<< endl;
	cout << "0. Exit"<< endl;
	cout << "--- Choose any one option ---" << endl;
	cout << "Enter one option: ";

	int selectedOption = 0;
	cin >> selectedOption;
	cin.ignore ( std::numeric_limits<std::streamsize>::max(), '\n' ); 

	return selectedOption;
}


void depositAmount() {
	cout << "--- Deposit Amount ---" << endl;

	string str = getName(); // get account holder's name
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value list = getBankRecords();
	int len = list.size();

	for ( int i = 0; i < len; i++ )
    {
		string name = list[i]["name"].asString();
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name == str) {
			int depositAmount;
			cout << "Deposit Amount: ";
			cin >> depositAmount;
			
			string balance = list[i]["balance"].asString();
			int bal = stoi(balance);
			bal += depositAmount;
			
			list[i]["balance"] = std::to_string(bal);

			setBankRecords(list);
			cout << "Deposit Successful!"<< endl;
			cout << "Current Balance: " << bal << endl;
			return;
		}

    }

	cout << "Record Not Found." << endl;
}


void withdrawAmount() {
	cout << "--- Withdraw Amount ---" << endl;

	string str = getName(); // get account holder's name
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value list = getBankRecords();
	int len = list.size();

	for ( int i = 0; i < len; i++ )
    {
		string name = list[i]["name"].asString();
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name == str) {
			int withdrawAmount;
			cout << "Withdraw Amount: ";
			cin >> withdrawAmount;
			
			string balance = list[i]["balance"].asString();
			int bal = stoi(balance);

			if (bal < withdrawAmount) {
				cout << "Not enough balance." << endl;
				return;
			}

			bal -= withdrawAmount;
			
			list[i]["balance"] = std::to_string(bal);

			setBankRecords(list);

			cout << "Withdrawl Successful!" << endl;
			cout << "Current Balance: " << bal << endl;
			
			return;
		}

    }

	cout << "Record Not Found." << endl;
}


void checkBalance() {
	cout << "--- Check Balance ---" << endl;

	string str = getName(); // get account holder's name
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value list = getBankRecords();
	int len = list.size();

	for ( int i = 0; i < len; i++ )
    {
		string name = list[i]["name"].asString();
		string holder = name;
		transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (name == str) {
			string balance = list[i]["balance"].asString();
			string accNum = list[i]["accNumber"].asString();
			
			cout << "Account Holder: " << holder << endl;
			cout << "Account Number: " << accNum << endl;
			cout << "Current Balance: " << balance << endl;
			return;
		}

    }

	cout << "Record Not Found." << endl;
}


void addAccount() {
	string accountHolder, accountNumber, id, balance = "0";

	Json::Value records = getBankRecords();
	int numOfAccounts = records.size();
	id = std::to_string(numOfAccounts);
	accountNumber = "A100" + std::to_string(numOfAccounts);

	cout << endl << "--- Provide Account Details ---" << endl;
	cout << "Account Holder Name: ";
	getline(cin, accountHolder);

	Json::Value record;
	record["id"] = id;
	record["name"] = accountHolder;
	record["accNumber"] = accountNumber;
	record["balance"] = balance;

	records.append(record);
	setBankRecords(records);
	
   	cout << accountHolder << "'s Account added. " << endl;
   	cout << "Account Number: " << accountNumber << endl; 
}


void updateAccount() {
	cout << endl << "--- Update Account ---" << endl;
	string str = getName();
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getBankRecords();
	int len = records.size();

    for ( int i = 0; i < len; i++ )
    {
		string value = records[i]["name"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if(str == value) {
			string name;
			cout << "New Name: ";
			std::getline(std::cin, name);

			records[i]["name"] = name;

			setBankRecords(records);
			cout << "Updated" << endl;

			return;
		}
    }

	cout << "Record Not Found." << endl;
}


void DeleteAccount() {
	cout << endl << "--- Delete Account ---" << endl;
	string str = getName();
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);

	Json::Value records = getBankRecords();
	int len = records.size();

	Json::Value newRecords;
	bool accExist = false;

    for ( int i = 0; i < len; i++ )
    {
		string value = records[i]["name"].asString();
		// convert to lowercase for comparision;
		std::transform(value.begin(), value.end(), value.begin(), ::tolower);

        if(str == value) {
			accExist = true;
			continue;
		}
		newRecords.append(records[i]);
    }

	if(accExist) {
		setBankRecords(newRecords);
		cout << "Account Deleted";
	} else {
		cout << "Account does not exist";
	}
}


void showAcounts() {
	cout << "--- List of Account Holders ---" << endl;

	Json::Value list = getBankRecords();
	int len = list.size();

	cout << "No. | " << "Account Holder's Name | " << "Acc No. | " << "Balance " << endl;
	for ( int i = 0; i < len; i++ )
    {
		string name = list[i]["name"].asString();
		string num = list[i]["accNumber"].asString();
		string balance = list[i]["balance"].asString();

		int padding = 25 - name.length();
		if (padding > 0) {
			for(int j = 0; j < padding; j++) {
				name += " ";
			}
		}

		cout << i+1 << ". " << name << "  " << num << "    $" << balance << endl;
    }
}


void bankActions(int option) {
	switch(option) {
		case 1: 
			depositAmount();
			break;
		case 2: 
			withdrawAmount();
			break;
		case 3: 
			checkBalance();
			break;
		case 4: 
			addAccount();
			break;
		case 5: 
			updateAccount();
			break;
		case 6: 
			DeleteAccount();
			break;
		case 7: 
			showAcounts();
			break;
	}
}


void home () {
	int option = bankOptions();
	if (option != 0 && option <= 7) {
		bankActions(option);
	} else if (option > 7) {
		cout << endl << "!!! Enter Valid Option !!!" << endl;
		option = bankOptions();
	} else {
	 	exit(0);
	}
}


int main () {
	cout << "*** WELCOME ***";
	string yn;
	while(true) {
		cout << endl << "--- Bank Management System ---" << endl;
		home();
		cout << endl << "continue? (y/n) :";
		cin >> yn;
		if(yn != "y") break;
	}

	cout << "Good Bye!" << endl;
}