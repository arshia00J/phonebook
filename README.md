# Contact Management System

A simple command-line contact management system implemented in C++. This application allows users to manage their contacts by adding, searching, deleting, updating, sorting, and saving contact information.

## Features

- **Add Contact**: Add new contacts with details including first name, last name, birthday, phone numbers, and email.
- **Search Contact**: Search for contacts by first name, last name, phone number, or email.
- **Delete Contact**: Remove contacts by specifying a phone number.
- **Update Contact**: Update contact details including adding/deleting phone numbers and changing the email.
- **Sort Contacts**: Sort contacts by last name or birth date.
- **Save to File**: Save all contacts to a file.
- **Load from File**: Load contacts from a file when the application starts.

## Data Structures

- **Date Structure**: Stores date information (year, month, day).
- **Person Structure**: Stores contact information (first name, last name, birthday, phone numbers, and email).

## Functions

- **menu()**: Displays a menu to the user and returns their choice.
- **check_num(string phonenum)**: Validates a phone number.
- **check_mail(string email)**: Validates an email address.
- **check_name(string name)**: Validates a name (at least 3 characters, no digits).
- **check_birth(Date birth)**: Validates a birth date.
- **is_phone_unique(const string& phone, const Person* phonebook, int counter)**: Checks if a phone number is unique.
- **add_person(Person* phonebook, int& emptyIndex)**: Adds a new person to the phonebook.
- **save_contacts(const Person* phonebook, int counter, const string& filename)**: Saves the phonebook to a file.
- **load_phonebook(Person* phonebook, int& counter, const string& filename)**: Loads the phonebook from a file.
- **delete_user(Person* phonebook, int& counter)**: Deletes a person from the phonebook.
- **search_user(const Person* phonebook, int counter)**: Searches for a person by name, phone number, or email.
- **change_info(Person* phonebook, int counter)**: Changes a person's information in the phonebook.
- **sort_contacts(Person* phonebook, int counter)**: Sorts the phonebook by last name or birth date.

## How to Run

1. Clone the repository.
2. Compile the program using a C++ compiler.
3. Run the executable.

## Example Usage

```
Choose one option: 
   1) Add contact
   2) Search contact
   3) Delete contact
   4) Change contact info
   5) Sort contacts
   6) Save to file
   7) Exit the program
```

---

Feel free to customize this description further based on your specific needs or additional features.
