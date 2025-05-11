#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

template <typename T>
struct Node {
    T data;
    Node* next;

    Node(const T& data) : data(data), next(nullptr) {}
};

struct Mail {
    string senderAddress;
    string receiverAddress;
    string subject;
    string content;
    bool isSpam; 

    friend ostream& operator<<(ostream& os, const Mail& mail) {
        os << "From: " << mail.senderAddress << "\n"
           << "To: " << mail.receiverAddress << "\n"
           << "Subject: " << mail.subject << "\n"
           << "Content: " << mail.content << "\n"
           << "Spam: " << (mail.isSpam ? "Yes" : "No") << "\n";
        return os;
    }
};

struct Account {
    string username;
    string gmailAddress;
    string password;
    Node<Mail>* inbox;    // Linked list for inbox
    Node<Mail>* sent;     // Linked list for sent emails
    Node<Mail>* draft;    // Linked list for draft emails
    Node<Mail>* spam;     // Linked list for spam emails
    Node<Mail>* trash;    // Linked list for trash emails

    Account() : inbox(nullptr), sent(nullptr), draft(nullptr), spam(nullptr), trash(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;

public:
    LinkedList() : head(nullptr) {}
    ~LinkedList() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void insert(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (!head) {
            head = newNode;
        } else {
            Node<T>* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void display() const {
        Node<T>* temp = head;
        while (temp) {
            cout << temp->data << "\n";
            temp = temp->next;
        }
    }

    Node<T>* getHead() const {
        return head;
    }
};

void saveToFile(const LinkedList<Account>& userAccounts, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file for saving." << endl;
        return;
    }

    cout << "Saving data to file: " << filename << endl; // Debug statement

    Node<Account>* temp = userAccounts.getHead();
    while (temp) {
        const Account& account = temp->data;
        file << "Account:\n"; // Marker for the start of an account
        file << account.username << "\n"
             << account.gmailAddress << "\n"
             << account.password << "\n";

        // Save inbox
        file << "Inbox:\n";
        Node<Mail>* inbox = account.inbox;
        while (inbox) {
            file << inbox->data.senderAddress << "\n"
                 << inbox->data.receiverAddress << "\n"
                 << inbox->data.subject << "\n"
                 << inbox->data.content << "\n"
                 << inbox->data.isSpam << "\n";
            inbox = inbox->next;
        }
        file << "EndInbox\n";

        // Save sent
        file << "Sent:\n";
        Node<Mail>* sent = account.sent;
        while (sent) {
            file << sent->data.senderAddress << "\n"
                 << sent->data.receiverAddress << "\n"
                 << sent->data.subject << "\n"
                 << sent->data.content << "\n"
                 << sent->data.isSpam << "\n";
            sent = sent->next;
        }
        file << "EndSent\n";

        // Save draft
        file << "Draft:\n";
        Node<Mail>* draft = account.draft;
        while (draft) {
            file << draft->data.senderAddress << "\n"
                 << draft->data.receiverAddress << "\n"
                 << draft->data.subject << "\n"
                 << draft->data.content << "\n"
                 << draft->data.isSpam << "\n";
            draft = draft->next;
        }
        file << "EndDraft\n";

        // Save spam
        file << "Spam:\n";
        Node<Mail>* spam = account.spam;
        while (spam) {
            file << spam->data.senderAddress << "\n"
                 << spam->data.receiverAddress << "\n"
                 << spam->data.subject << "\n"
                 << spam->data.content << "\n"
                 << spam->data.isSpam << "\n";
            spam = spam->next;
        }
        file << "EndSpam\n";

        // Save trash
        file << "Trash:\n";
        Node<Mail>* trash = account.trash;
        while (trash) {
            file << trash->data.senderAddress << "\n"
                 << trash->data.receiverAddress << "\n"
                 << trash->data.subject << "\n"
                 << trash->data.content << "\n"
                 << trash->data.isSpam << "\n";
            trash = trash->next;
        }
        file << "EndTrash\n";

        temp = temp->next;
    }

    file.close();
    cout << "Data saved to file successfully!" << endl;
}

void loadFromFile(LinkedList<Account>& userAccounts, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file for loading." << endl;
        return;
    }

    cout << "Loading data from file: " << filename << endl; // Debug statement

    string line;
    Account* currentAccount = nullptr;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line == "Account:") {
            // Load account details
            Account account;
            getline(file, account.username);
            getline(file, account.gmailAddress);
            getline(file, account.password);

            userAccounts.insert(account);
            currentAccount = &userAccounts.getHead()->data;
        } else if (line == "Inbox:") {
            // Load inbox
            while (getline(file, line) && line != "EndInbox") {
                Mail mail;
                mail.senderAddress = line;
                getline(file, mail.receiverAddress);
                getline(file, mail.subject);
                getline(file, mail.content);
                file >> mail.isSpam;
                file.ignore(); // Ignore newline after bool

                if (!currentAccount->inbox) {
                    currentAccount->inbox = new Node<Mail>(mail);
                } else {
                    Node<Mail>* temp = currentAccount->inbox;
                    while (temp->next) {
                        temp = temp->next;
                    }
                    temp->next = new Node<Mail>(mail);
                }
            }
        } else if (line == "Sent:") {
            // Load sent
            while (getline(file, line) && line != "EndSent") {
                Mail mail;
                mail.senderAddress = line;
                getline(file, mail.receiverAddress);
                getline(file, mail.subject);
                getline(file, mail.content);
                file >> mail.isSpam;
                file.ignore(); // Ignore newline after bool

                if (!currentAccount->sent) {
                    currentAccount->sent = new Node<Mail>(mail);
                } else {
                    Node<Mail>* temp = currentAccount->sent;
                    while (temp->next) {
                        temp = temp->next;
                    }
                    temp->next = new Node<Mail>(mail);
                }
            }
        } else if (line == "Draft:") {
            // Load draft
            while (getline(file, line) && line != "EndDraft") {
                Mail mail;
                mail.senderAddress = line;
                getline(file, mail.receiverAddress);
                getline(file, mail.subject);
                getline(file, mail.content);
                file >> mail.isSpam;
                file.ignore(); // Ignore newline after bool

                if (!currentAccount->draft) {
                    currentAccount->draft = new Node<Mail>(mail);
                } else {
                    Node<Mail>* temp = currentAccount->draft;
                    while (temp->next) {
                        temp = temp->next;
                    }
                    temp->next = new Node<Mail>(mail);
                }
            }
        } else if (line == "Spam:") {
            // Load spam
            while (getline(file, line) && line != "EndSpam") {
                Mail mail;
                mail.senderAddress = line;
                getline(file, mail.receiverAddress);
                getline(file, mail.subject);
                getline(file, mail.content);
                file >> mail.isSpam;
                file.ignore(); // Ignore newline after bool

                if (!currentAccount->spam) {
                    currentAccount->spam = new Node<Mail>(mail);
                } else {
                    Node<Mail>* temp = currentAccount->spam;
                    while (temp->next) {
                        temp = temp->next;
                    }
                    temp->next = new Node<Mail>(mail);
                }
            }
        } else if (line == "Trash:") {
            // Load trash
            while (getline(file, line) && line != "EndTrash") {
                Mail mail;
                mail.senderAddress = line;
                getline(file, mail.receiverAddress);
                getline(file, mail.subject);
                getline(file, mail.content);
                file >> mail.isSpam;
                file.ignore(); // Ignore newline after bool

                if (!currentAccount->trash) {
                    currentAccount->trash = new Node<Mail>(mail);
                } else {
                    Node<Mail>* temp = currentAccount->trash;
                    while (temp->next) {
                        temp = temp->next;
                    }
                    temp->next = new Node<Mail>(mail);
                }
            }
        }
    }

    file.close();
    cout << "Data loaded from file successfully!" << endl;
}
void mainMenu();

void subMenu(Account* user, LinkedList<Account>& userAccounts);

void mainMenu() {
    LinkedList<Account> userAccounts; // Linked list to store all user accounts
    Account* currentUser = nullptr;  // Pointer to track the currently logged-in user

    // Load data from file at the start
    loadFromFile(userAccounts, "email_data.txt");

    while (true) {
        cout << "===== Main Menu =====" << endl;
        cout << "1- Create New Account" << endl;
        cout << "2- Login Account" << endl;
        cout << "3- Logout Account" << endl;
        cout << "4- Save the data in file and Exit System" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            // Create a new account
            Account newAccount;
            cout << "Enter username: ";
            cin >> newAccount.username;
            cout << "Enter Gmail address (must contain @lhr.nu.edu.pk): ";
            cin >> newAccount.gmailAddress;
            cout << "Enter password (minimum 6 characters): ";
            cin >> newAccount.password;

            // Validate the Gmail address and password
            if (newAccount.gmailAddress.find("@lhr.nu.edu.pk") == string::npos || newAccount.password.length() < 6) {
                cout << "Invalid Gmail address or password. Account creation failed." << endl;
            } else {
                userAccounts.insert(newAccount);
                cout << "Account created successfully!" << endl;
            }
        } else if (choice == 2) {
            // Login to an account
            if (currentUser) {
                cout << "You are already logged in as " << currentUser->username << ". Logout first." << endl;
                continue;
            }

            string username, password;
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;

            // Search for the account in the linked list
            Node<Account>* temp = userAccounts.getHead();
            while (temp) {
                if (temp->data.username == username && temp->data.password == password) {
                    currentUser = &temp->data;
                    cout << "Login successful! Welcome, " << currentUser->username << "." << endl;
                    subMenu(currentUser, userAccounts); // Go to the sub-menu
                    break;
                }
                temp = temp->next;
            }
            if (!currentUser) {
                cout << "Invalid username or password." << endl;
            }
        } else if (choice == 3) {
            // Logout from the current account
            if (currentUser) {
                cout << "Logged out successfully! Goodbye, " << currentUser->username << "." << endl;
                currentUser = nullptr;
            } else {
                cout << "No user is currently logged in." << endl;
            }
        } else if (choice == 4) {
            // Exit the system
            saveToFile(userAccounts, "email_data.txt"); // Save data to file before exiting
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Please try again. Exiting Program" << endl;
            break;
        }
    }
}

void subMenu(Account* user, LinkedList<Account>& userAccounts) {
    vector<string> spamWords = {"Winner", "Free", "Urgent", "Claim", "Bonus", "Limited", "Exclusive", "Gift", "Guaranteed", "Profit"};

    while (true) {
        cout << "===== Sub Menu =====" << endl;
        cout << "1- Create New Mail" << endl;
        cout << "2- Sent" << endl;
        cout << "3- Inbox" << endl;
        cout << "4- Draft" << endl;
        cout << "5- Spam" << endl;
        cout << "6- Trash" << endl;
        cout << "7- Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            // Create a new mail
            Mail newMail;
            newMail.senderAddress = user->gmailAddress;
            cout << "Enter recipient's email address: ";
            cin >> newMail.receiverAddress;

            // Check if the recipient's email is valid
            Node<Account>* temp = userAccounts.getHead();
            bool recipientFound = false;
            while (temp) {
                if (temp->data.gmailAddress == newMail.receiverAddress) {
                    recipientFound = true;
                    break;
                }
                temp = temp->next;
            }

            if (!recipientFound) {
                cout << "No user exists with such an address. Returning to Sub Menu." << endl;
                continue;
            }

            cout << "Enter subject: ";
            cin.ignore();
            getline(cin, newMail.subject);
            cout << "Enter content: ";
            getline(cin, newMail.content);

            // Check if the mail is spam
            newMail.isSpam = false;
            for (const string& word : spamWords) {
                if (newMail.subject.find(word) != string::npos || newMail.content.find(word) != string::npos) {
                    newMail.isSpam = true;
                    break;
                }
            }

            // Ask the user if they want to send the mail or save it as a draft
            cout << "1- Send Mail" << endl;
            cout << "2- Save as Draft" << endl;
            cout << "Enter your choice: ";
            int sendChoice;
            cin >> sendChoice;

            if (sendChoice == 1) {
                // Send the mail
                // Add the mail to the recipient's inbox or spam
                if (newMail.isSpam) {
                    if (!temp->data.spam) {
                        temp->data.spam = new Node<Mail>(newMail);
                    } else {
                        Node<Mail>* spamTemp = temp->data.spam;
                        while (spamTemp->next) {
                            spamTemp = spamTemp->next;
                        }
                        spamTemp->next = new Node<Mail>(newMail);
                    }
                } else {
                    if (!temp->data.inbox) {
                        temp->data.inbox = new Node<Mail>(newMail);
                    } else {
                        Node<Mail>* inboxTemp = temp->data.inbox;
                        while (inboxTemp->next) {
                            inboxTemp = inboxTemp->next;
                        }
                        inboxTemp->next = new Node<Mail>(newMail);
                    }
                }

                // Add the mail to the sender's sent list
                if (!user->sent) {
                    user->sent = new Node<Mail>(newMail);
                } else {
                    Node<Mail>* sentTemp = user->sent;
                    while (sentTemp->next) {
                        sentTemp = sentTemp->next;
                    }
                    sentTemp->next = new Node<Mail>(newMail);
                }

                cout << "Mail sent successfully!" << endl;
            } else if (sendChoice == 2) {
                // Save as draft
                if (!user->draft) {
                    user->draft = new Node<Mail>(newMail);
                } else {
                    Node<Mail>* draftTemp = user->draft;
                    while (draftTemp->next) {
                        draftTemp = draftTemp->next;
                    }
                    draftTemp->next = new Node<Mail>(newMail);
                }

                cout << "Mail saved as draft successfully!" << endl;
            } else {
                cout << "Invalid choice. Returning to Sub Menu." << endl;
            }
        } else if (choice == 2) {
            // View sent emails
            cout << "===== Sent Emails =====" << endl;
            if (user->sent) {
                Node<Mail>* temp = user->sent;
                int index = 1;
                while (temp) {
                    cout << index << "- " << temp->data.subject << " to " << temp->data.receiverAddress << endl;
                    temp = temp->next;
                    index++;
                }

                cout << "Enter the number of the email to view its content (or 0 to go back): ";
                int viewChoice;
                cin >> viewChoice;

                if (viewChoice > 0 && viewChoice < index) {
                    temp = user->sent;
                    for (int i = 1; i < viewChoice; i++) {
                        temp = temp->next;
                    }
                    cout << temp->data << endl;
                } else if (viewChoice != 0) {
                    cout << "Invalid choice. Returning to Sub Menu." << endl;
                }
            } else {
                cout << "No sent emails." << endl;
            }
        } else if (choice == 3) {
            // View inbox
            cout << "===== Inbox =====" << endl;
            if (user->inbox) {
                Node<Mail>* temp = user->inbox;
                int index = 1;
                while (temp) {
                    cout << index << "- " << temp->data.subject << " from " << temp->data.senderAddress << endl;
                    temp = temp->next;
                    index++;
                }

                cout << "Enter the number of the email to view its content (or 0 to go back): ";
                int viewChoice;
                cin >> viewChoice;

                if (viewChoice > 0 && viewChoice < index) {
                    temp = user->inbox;
                    for (int i = 1; i < viewChoice; i++) {
                        temp = temp->next;
                    }
                    cout << temp->data << endl;

                    // Option to move the email to trash
                    cout << "1- Move to Trash" << endl;
                    cout << "2- Go back" << endl;
                    cout << "Enter your choice: ";
                    int trashChoice;
                    cin >> trashChoice;

                    if (trashChoice == 1) {
                        // Move the email to trash
                        if (!user->trash) {
                            user->trash = new Node<Mail>(temp->data);
                        } else {
                            Node<Mail>* trashTemp = user->trash;
                            while (trashTemp->next) {
                                trashTemp = trashTemp->next;
                            }
                            trashTemp->next = new Node<Mail>(temp->data);
                        }

                        // Remove the email from the inbox
                        if (temp == user->inbox) {
                            user->inbox = temp->next;
                        } else {
                            Node<Mail>* prev = user->inbox;
                            while (prev->next != temp) {
                                prev = prev->next;
                            }
                            prev->next = temp->next;
                        }

                        delete temp;
                        cout << "Email moved to trash successfully!" << endl;
                    }
                } else if (viewChoice != 0) {
                    cout << "Invalid choice. Returning to Sub Menu." << endl;
                }
            } else {
                cout << "No emails in inbox." << endl;
            }
        } else if (choice == 4) {
            // View draft emails
            cout << "===== Draft Emails =====" << endl;
            if (user->draft) {
                Node<Mail>* temp = user->draft;
                int index = 1;
                while (temp) {
                    cout << index << "- " << temp->data.subject << " to " << temp->data.receiverAddress << endl;
                    temp = temp->next;
                    index++;
                }

                cout << "Enter the number of the email to view its content (or 0 to go back): ";
                int viewChoice;
                cin >> viewChoice;

                if (viewChoice > 0 && viewChoice < index) {
                    temp = user->draft;
                    for (int i = 1; i < viewChoice; i++) {
                        temp = temp->next;
                    }
                    cout << temp->data << endl;

                    // Option to send the draft
                    cout << "1- Send Draft" << endl;
                    cout << "2- Go back" << endl;
                    cout << "Enter your choice: ";
                    int sendChoice;
                    cin >> sendChoice;

                    if (sendChoice == 1) {
                        // Send the draft
                        // Find the recipient's account
                        Node<Account>* recipientTemp = userAccounts.getHead();
                        bool recipientFound = false;
                        while (recipientTemp) {
                            if (recipientTemp->data.gmailAddress == temp->data.receiverAddress) {
                                recipientFound = true;
                                break;
                            }
                            recipientTemp = recipientTemp->next;
                        }

                        if (!recipientFound) {
                            cout << "Recipient not found. Draft not sent." << endl;
                            continue;
                        }

                        // Add the mail to the recipient's inbox or spam
                        if (temp->data.isSpam) {
                            if (!recipientTemp->data.spam) {
                                recipientTemp->data.spam = new Node<Mail>(temp->data);
                            } else {
                                Node<Mail>* spamTemp = recipientTemp->data.spam;
                                while (spamTemp->next) {
                                    spamTemp = spamTemp->next;
                                }
                                spamTemp->next = new Node<Mail>(temp->data);
                            }
                        } else {
                            if (!recipientTemp->data.inbox) {
                                recipientTemp->data.inbox = new Node<Mail>(temp->data);
                            } else {
                                Node<Mail>* inboxTemp = recipientTemp->data.inbox;
                                while (inboxTemp->next) {
                                    inboxTemp = inboxTemp->next;
                                }
                                inboxTemp->next = new Node<Mail>(temp->data);
                            }
                        }

                        // Add the mail to the sender's sent list
                        if (!user->sent) {
                            user->sent = new Node<Mail>(temp->data);
                        } else {
                            Node<Mail>* sentTemp = user->sent;
                            while (sentTemp->next) {
                                sentTemp = sentTemp->next;
                            }
                            sentTemp->next = new Node<Mail>(temp->data);
                        }

                        // Remove the draft from the draft list
                        if (temp == user->draft) {
                            user->draft = temp->next;
                        } else {
                            Node<Mail>* prev = user->draft;
                            while (prev->next != temp) {
                                prev = prev->next;
                            }
                            prev->next = temp->next;
                        }

                        delete temp;
                        cout << "Draft sent successfully!" << endl;
                    }
                } else if (viewChoice != 0) {
                    cout << "Invalid choice. Returning to Sub Menu." << endl;
                }
            } else {
                cout << "No draft emails." << endl;
            }
        } else if (choice == 5) {
            // View spam emails
            cout << "===== Spam Emails =====" << endl;
            if (user->spam) {
                Node<Mail>* temp = user->spam;
                int index = 1;
                while (temp) {
                    cout << index << "- " << temp->data.subject << " from " << temp->data.senderAddress << endl;
                    temp = temp->next;
                    index++;
                }

                cout << "Enter the number of the email to view its content (or 0 to go back): ";
                int viewChoice;
                cin >> viewChoice;

                if (viewChoice > 0 && viewChoice < index) {
                    temp = user->spam;
                    for (int i = 1; i < viewChoice; i++) {
                        temp = temp->next;
                    }
                    cout << temp->data << endl;

                    // Option to move the email to trash
                    cout << "1- Move to Trash" << endl;
                    cout << "2- Go back" << endl;
                    cout << "Enter your choice: ";
                    int trashChoice;
                    cin >> trashChoice;

                    if (trashChoice == 1) {
                        // Move the email to trash
                        if (!user->trash) {
                            user->trash = new Node<Mail>(temp->data);
                        } else {
                            Node<Mail>* trashTemp = user->trash;
                            while (trashTemp->next) {
                                trashTemp = trashTemp->next;
                            }
                            trashTemp->next = new Node<Mail>(temp->data);
                        }

                        // Remove the email from the spam
                        if (temp == user->spam) {
                            user->spam = temp->next;
                        } else {
                            Node<Mail>* prev = user->spam;
                            while (prev->next != temp) {
                                prev = prev->next;
                            }
                            prev->next = temp->next;
                        }

                        delete temp;
                        cout << "Email moved to trash successfully!" << endl;
                    }
                } else if (viewChoice != 0) {
                    cout << "Invalid choice. Returning to Sub Menu." << endl;
                }
            } else {
                cout << "No spam emails." << endl;
            }
        } else if (choice == 6) {
            // View trash emails
            cout << "===== Trash Emails =====" << endl;
            if (user->trash) {
                Node<Mail>* temp = user->trash;
                int index = 1;
                while (temp) {
                    cout << index << "- " << temp->data.subject << " from " << temp->data.senderAddress << endl;
                    temp = temp->next;
                    index++;
                }

                cout << "Enter the number of the email to view its content (or 0 to go back): ";
                int viewChoice;
                cin >> viewChoice;

                if (viewChoice > 0 && viewChoice < index) {
                    temp = user->trash;
                    for (int i = 1; i < viewChoice; i++) {
                        temp = temp->next;
                    }
                    cout << temp->data << endl;
                } else if (viewChoice != 0) {
                    cout << "Invalid choice. Returning to Sub Menu." << endl;
                }
            } else {
                cout << "No trash emails." << endl;
            }
        } else if (choice == 7) {
            // Back to the main menu
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
}

int main() {
    mainMenu();
    return 0;
}