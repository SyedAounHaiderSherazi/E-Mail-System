# 📧 E-Mail Management System

A console-based email system in C++ that allows users to create accounts, send emails, manage spam, drafts, trash, and persist data using file I/O. Designed to demonstrate core concepts of data structures, file handling, and string processing in C++.

## 📁 File Structure

- `E-Mail System.cpp`: Full source code implementing the system
- `email_data.txt`: File for persistent storage of accounts and emails

## 🔑 Features

### 👤 Account Management
- Create new accounts using institutional email
- Login/Logout securely with password validation

### ✉️ Mail Management
- Compose and send emails
- Save drafts for later
- View sent, inbox, spam, and trash
- Move emails to trash manually

### 🧠 Spam Detection
- Detects spam based on content and subject
- Flags spam automatically based on keywords
- Winner, Free, Urgent, Claim, Bonus, Limited, Exclusive, Gift, Guaranteed, Profit


### 💾 File Handling
- Automatically loads from `email_data.txt` on startup
- Saves all emails and account data back to the file on exit

## 🧱 Data Structures

- Custom **Singly Linked List** for:
  - Users
  - Inbox, Sent, Draft, Spam, Trash folders
- `Mail` struct: Defines an email
- `Account` struct: Defines a user profile

## ⚙️ How to Run

1. Compile using g++ or any C++ compiler:
   ```bash
   g++ "E-Mail System.cpp" -o EmailSystem
