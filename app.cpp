#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>
#include <regex>
#include <algorithm>

using namespace std;

// Structure to hold date information
struct Date
{
    int year;
    int month;
    int day;
};

// Structure to hold contact information
struct Person
{
    string fname;
    string lname;
    Date birthday;
    string phoneNumbers[10];
    string email;
};

// Function to display the menu and get user choice
int menu(void)
{
    cout << "Choose one option: \n";
    cout << "   1) Add contact\n";
    cout << "   2) Search contact\n";
    cout << "   3) Delete contact\n";
    cout << "   4) Change contact info\n";
    cout << "   5) Sort contacts\n";
    cout << "   6) Save to file\n";
    cout << "   7) Exit the program\n";

    int answer;
    cin >> answer;
    if(answer >=1 and answer <= 7)
        return answer;
    else
    {

        cout << "Invalid input format (1 - 7).\n";

        return 7;

    }
}

// Function to check if a phone number is valid
bool check_num(string phonenum)
{
    if (phonenum[0] == '0' && phonenum.length() == 11)
        return true;
    else
        return false;
}

// Function to check if an email is valid
bool check_mail(string email)
{
    const regex pattern("[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}");
    return regex_match(email, pattern);
}

// Function to check if a name is valid (at least 3 characters)
bool check_name(string name)
{
    if (name.length() < 3)
        return false;

    for (char c : name) {
        if (isdigit(c))
            return false;
    }
    return true;

}

// Function to check if a birth date is valid
bool check_birth(Date birth)
{

    if (birth.year < 1)
        return false;
    if (birth.month < 1 || birth.month > 12)
        return false;

    int days[] = { 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29 };

    // Check for KABISE year
    int a = birth.year % 33;
    bool kabise = a == 1 or a == 5 or a == 9 or a == 13 or a == 17 or a == 22 or a == 26 or a == 30;

    if (kabise && birth.month == 12)
        days[11] = 30;

    if (birth.day < 1 or birth.day > days[birth.month - 1])
        return false;

    return true;
}

// Function to check if a phone number is unique in the phonebook
bool is_phone_unique(const string& phone, const Person* phonebook, int counter)
{
    for (int i = 0; i < counter; ++i)
    {
        const string (&phoneNumbers)[10] = phonebook[i].phoneNumbers;

        for (int j = 0; j < 10; ++j)
        {
            if (phoneNumbers[j] == phone)
                return false;
        }

    }
    return true;
}

// Function to add a new person to the phonebook
void add_person(Person* phonebook, int& emptyIndex)
{
    if (emptyIndex >= 100)
    {
        cout << "Phonebook is full.\n";
        return;
    }

    Person p;


    // Get and validate first name
    cout << "Enter the first name: ";
    cin >> p.fname;
    while (!check_name(p.fname))
    {
        cout << "Invalid name. (at least 3 characters)\nEnter the first name: ";
        cin >> p.fname;
    }

    // Get and validate last name
    cout << "Enter the last name: ";
    cin >> p.lname;
    while (!check_name(p.lname))
    {
        cout << "Invalid name. (at least 3 characters)\nEnter the last name: ";
        cin >> p.lname;
    }

    // Get and validate birth date
    cout << "Enter birthday (yyyy/mm/dd): ";
    char slash;
    cin >> p.birthday.year >> slash >> p.birthday.month >> slash >> p.birthday.day;
    while (!check_birth(p.birthday))
    {
        cout << "Invalid date. Please enter again (yyyy/mm/dd): ";
        cin >> p.birthday.year >> slash >> p.birthday.month >> slash >> p.birthday.day;
    }

    // Get and validate phone numbers
    int phoneIndex = 0;
    set<string> uniquePhones;

    while (phoneIndex < 10)
    {
        bool allPhonesValid = true;

        cout << "Enter phone number (-1 to finish): ";
        string phone;
        cin >> phone;

        if (phone == "-1")
            break;

        if (check_num(phone))
        {
            if (uniquePhones.find(phone) != uniquePhones.end() || !is_phone_unique(phone, phonebook, emptyIndex))
            {
                cout << "Duplicate phone number: " << phone << ". Please enter a different number.\n";
                allPhonesValid = false;
            }
            else
            {
                p.phoneNumbers[phoneIndex] = phone;
                phoneIndex++;
                uniquePhones.insert(phone);
            }
        }
        else
        {
            cout << "Invalid phone number: " << phone << ". Please enter a valid number.\n";
            allPhonesValid = false;
        }
    }


    // Get and validate email if available
    cout << "Do you have an email? (y/n): ";
    char hasEmail;
    cin >> hasEmail;
    if (hasEmail == 'y' || hasEmail == 'Y')
    {
        cout << "Enter email: ";
        cin >> p.email;
        while (!check_mail(p.email))
        {
            cout << "Invalid email. Please enter again: ";
            cin >> p.email;
        }
    }
    else if(hasEmail == 'n' || hasEmail == 'N')
        p.email = "";
    else
        cout << "Wrong answer!\n";

    // Add the person to the phonebook
    phonebook[emptyIndex] = p;
    emptyIndex++;
    cout << "This person added to contacts!\n";
}

// Function to save the phonebook to a file
void save_contacts(const Person* phonebook, int counter, const string& filename)
{
    ofstream outFile(filename, ios::out);

    // Write each person's details to the file
    for (int i = 0; i < counter; ++i)
    {
        outFile << phonebook[i].fname << " " << phonebook[i].lname << "\n";
        outFile << phonebook[i].birthday.year << "/" << phonebook[i].birthday.month << "/" << phonebook[i].birthday.day << "\n";
        for (const auto& phone : phonebook[i].phoneNumbers)
        {
            if (!phone.empty())
                outFile << phone << ";";
        }
        outFile << "\n";
        outFile << phonebook[i].email << "\n";
        outFile << "------------------\n";
    }

    outFile.close();
    cout << "Phonebook saved to " << filename << endl;
}

// Function to load the phonebook from a file
void load_phonebook(Person* phonebook, int& counter, const string& filename)
{
    ifstream inFile(filename);

    counter = 0;
    string line;
    while (getline(inFile, line))
    {
        if (line == "------------------")
            continue;

        // ====== NAME ======
        Person p;
        istringstream iss(line);
        iss >> p.fname >> p.lname;

        // ====== DATE ======

        // Reads a line of text from the input stream "inFile" and stores it in the string "line"
        getline(inFile, line);
        // Reads formatted input from a string
        sscanf(line.c_str(), "%d/%d/%d", &p.birthday.year, &p.birthday.month, &p.birthday.day);

        // ====== PHONE NUMBER ======
        getline(inFile, line);
        stringstream phoneStream(line);
        string phone;
        int phoneIndex = 0;
        while (getline(phoneStream, phone, ';') && phoneIndex < 10)
        {
            if (!phone.empty())
            {
                p.phoneNumbers[phoneIndex] = phone;
                phoneIndex++;
            }
        }

        // ====== EMAIL ======
        getline(inFile, p.email);

        // Read the "------------------"
        getline(inFile, line);

        phonebook[counter] = p;
        counter++;
    }

    inFile.close();
    cout << "Phonebook loaded from " << filename << endl;
    cout << "Hello!,please choose one option " << endl;
}

// Function to delete a person from the phonebook
void delete_user(Person* phonebook, int& counter)
{
    cout << "Enter phone number of the person to delete (-1 for Back): ";
    string phoneInput;
    cin >> phoneInput;

    if (phoneInput == "-1")
        return;
    if (!check_num(phoneInput))
    {
        cout << "Invalid phone number format.\n";
        return;
    }

    int indexToDelete = -1;
    for (int i = 0; i < counter; ++i)
    {
        for (const auto& phone : phonebook[i].phoneNumbers)
        {
            if (phone == phoneInput)
            {
                indexToDelete = i;
                break;
            }
        }
        if (indexToDelete != -1)
            break;
    }

    if (indexToDelete == -1)
    {
        cout << "Person with this phone number not found.\n";
        return;
    }

    for (int i = indexToDelete; i < counter - 1; ++i)
        phonebook[i] = phonebook[i + 1];

    counter--;

    cout << "Person deleted from phonebook.\n";
}

// Function to search for a person by name, email, phone number
void search_user(const Person* phonebook, int counter)
{
    cout << "Choose search option:\n";
    cout << "   1) First name\n";
    cout << "   2) Last name\n";
    cout << "   3) Phone number\n";
    cout << "   4) Email\n";
    cout << "   5) Back\n";

    int option;
    cin >> option;

    if (option == 5)
        return;

    string searchTerm;
    cout << "Enter the search term: ";
    cin >> searchTerm;

    bool found = false;
    for (int i = 0; i < counter; ++i)
    {
        bool match = false;
        switch (option)
        {
            case 1:
                match = (phonebook[i].fname == searchTerm);
                break;
            case 2:
                match = (phonebook[i].lname == searchTerm);
                break;
            case 3:
                for (const auto& phone : phonebook[i].phoneNumbers)
                {
                    if (phone == searchTerm)
                    {
                        match = true;
                        break;
                    }
                }
                break;
            case 4:
                match = (phonebook[i].email == searchTerm);
                break;
            default:
                cout << "Invalid option.\n";
                return;
        }

        if (match) {
            found = true;
            cout << "First Name: " << phonebook[i].fname << "\n";
            cout << "Last Name: " << phonebook[i].lname << "\n";
            cout << "Birthday: " << phonebook[i].birthday.year << "/" << phonebook[i].birthday.month << "/" << phonebook[i].birthday.day << "\n";
            cout << "Phone Numbers: ";
            for (const auto& phone : phonebook[i].phoneNumbers)
            {
                if (!phone.empty())
                    cout << phone << " ";
            }
            cout << "\nEmail: " << phonebook[i].email << "\n";
            cout << "------------------\n";
        }
    }

    if (!found)
        cout << "No matching contact found.\n";
}

// Function to change a person's information in the phonebook
void change_info(Person* phonebook, int counter)
{
    cout << "Enter phone number of the person to change(-1 to Back): ";
    string phoneInput;
    cin >> phoneInput;

    if (phoneInput == "-1")
        return;
    int indexToChange = -1;
    for (int i = 0; i < counter; ++i)
    {
        for (const auto& phone : phonebook[i].phoneNumbers)
        {
            if (phone == phoneInput)
            {
                indexToChange = i;
                break;
            }
        }
        if (indexToChange != -1)
            break;
    }

    if (indexToChange == -1)
    {
        cout << "Phone number not found.\n";
        return;
    }


    Person& p = phonebook[indexToChange];
    cout << "Editing this contact:\n";
    cout << "First Name: " << p.fname << "\n";
    cout << "Last Name: " << p.lname << "\n";
    cout << "Email: " << p.email << "\n";
    cout << "Phone Numbers: ";
    for (const auto& phone : p.phoneNumbers) {
        if (!phone.empty()) {
            cout << phone << " ";
        }
    }
    cout<<endl;


    cout << "Choose one option: \n";
    cout << "   1) Add phone number\n";
    cout << "   2) Delete phone number\n";
    cout << "   3) Change email\n";
    cout << "   4) Back\n";


    int choice;
    cin >> choice;

    switch (choice)
    {
        case 1: {
            cout << "Enter new phone number: ";
            string newPhone;
            cin >> newPhone;
            if (!check_num(newPhone) || !is_phone_unique(newPhone, phonebook, counter))
            {
                cout << "Invalid or duplicate phone number.\n";
                return;
            }
            for (auto& phone : p.phoneNumbers)
            {
                if (phone.empty())
                {
                    phone = newPhone;
                    cout << "Phone number added.\n";
                    return;
                }
            }
            cout << "Cannot add more phone numbers.(Max = 10)\n";
            break;
        }
        case 2: {
            cout << "Enter phone number to delete: ";
            string delPhone;
            cin >> delPhone;
            bool found = false;
            int phoneCount = 0;
            for (const auto& phone : p.phoneNumbers)
            {
                if (!phone.empty())
                    phoneCount++;
            }
            if (phoneCount <= 1)
            {
                cout << "Cannot delete the last phone number.\n";
                return;
            }
            for (auto& phone : p.phoneNumbers)
            {
                if (phone == delPhone)
                {
                    phone.clear();
                    found = true;
                    break;
                }
            }
            if (found)
                cout << "Phone number deleted.\n";
            else
                cout << "Phone number not found.\n";
            break;
        }
        case 3: {
            cout << "Enter new email: ";
            string newEmail;
            cin >> newEmail;
            if (!check_mail(newEmail))
            {
                cout << "Invalid email format.\n";
                return;
            }
            p.email = newEmail;
            cout << "Email updated.\n";
            break;
        }
        case 4:
            return;
        default:
            cout << "Invalid option.\n";
            break;
    }
}

// Function to sort the phonebook by first name or last name
void sort_contacts(Person* phonebook, int counter)
{
    cout << "Choose sorting option:\n";
    cout << "   1) Last name\n";
    cout << "   2) Birth date\n";
    cout << "   3) Back\n";

    int option;
    cin >> option;

    switch (option)
    {
        case 1:
            sort(phonebook, phonebook + counter, [](const Person& a, const Person& b)
            {
                if (a.lname != b.lname)
                    return a.lname < b.lname;

                if (a.fname != b.fname)
                    return a.fname < b.fname;

                if (a.birthday.year != b.birthday.year)
                    return a.birthday.year < b.birthday.year;

                if (a.birthday.month != b.birthday.month)
                    return a.birthday.month < b.birthday.month;

                return a.birthday.day < b.birthday.day;

            });
            break;
        case 2:
            sort(phonebook, phonebook + counter, [](const Person& a, const Person& b) {
                if (a.birthday.year != b.birthday.year)
                    return a.birthday.year < b.birthday.year;
                if (a.birthday.month != b.birthday.month)
                    return a.birthday.month < b.birthday.month;
                if (a.birthday.day != b.birthday.day)
                    return a.birthday.day < b.birthday.day;
                if (a.lname != b.lname)
                    return a.lname < b.lname;
                return a.fname < b.fname;
            });
            break;
        case 3:
            return;
        default:
            cout << "Invalid option.\n";
            break;
    }

    ofstream outFile("phonebook.txt", ios::out); // Open file in out mode to overwrite

    for (int i = 0; i < counter; ++i) {
        outFile << phonebook[i].fname << " " << phonebook[i].lname << "\n";
        outFile << phonebook[i].birthday.year << "/" << phonebook[i].birthday.month << "/" << phonebook[i].birthday.day << "\n";
        for (const auto& phone : phonebook[i].phoneNumbers)
        {
            if (!phone.empty())
                outFile << phone << ";";
        }

        outFile << "\n";
        outFile << phonebook[i].email << "\n";
        outFile << "------------------\n";
    }

    outFile.close();
    cout << "Contacts saved to phonebook.txt" << endl;
}



int main() {
    Person phonebook[100];
    int emptyIndex = 0;

    // Load phonebook from file
    load_phonebook(phonebook, emptyIndex, "phonebook.txt");

    int choice = 0;
    while (choice != 7) {
        choice = menu();

        switch (choice) {
            case 1:
                add_person(phonebook, emptyIndex);
                break;
            case 2:
                search_user(phonebook, emptyIndex);
                break;
            case 3:
                delete_user(phonebook, emptyIndex);
                break;
            case 4:
                change_info(phonebook, emptyIndex);
                break;
            case 5:
                sort_contacts(phonebook, emptyIndex);
                break;
            case 6:
                save_contacts(phonebook, emptyIndex, "phonebook.txt");
                break;
            case 7:
                cout << "Have a nice day!.\n";
                break;
            default:
                cout << "Invalid option. Please choose again.\n";
        }
    }

    return 0;
}
