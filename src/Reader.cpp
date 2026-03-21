#include "Reader.h"
#include "Library.h"
#include "Utils.h"

using namespace std;

Reader::Reader(string name, string contact) {
	static int nextId = 1;
	this->id = nextId++;

	this->name = name;
	this->contact = contact;
	this->registrationDate = getCurrentDate();
}
int Reader::getId() const { return id; }
string Reader::getNameReader() const { return name; }
string Reader::getContactReader() const { return contact; }
vector<int> Reader::getBorrowedBooks() const { return borrowedBooks; }
vector<int> Reader::getHistory() const { return history; }
time_t Reader::getRegistrationDate() const { return registrationDate; }

void Reader::borrowBook(int bookId) {
	if (borrowedCount() == Library::BORROW_LIMIT) return;
	borrowedBooks.push_back(bookId);
}

void Reader::returnBook(int bookId) {
	for (size_t i = 0; i < borrowedBooks.size(); i++) {
		if (borrowedBooks[i] == bookId) {
			borrowedBooks.erase(borrowedBooks.begin() + i);
			history.push_back(bookId);
			return;
		}
	}
}

bool Reader::canBorrow() const {
	return borrowedCount() < Library::BORROW_LIMIT;
}

int Reader::borrowedCount() const {
	return (int)borrowedBooks.size();
}