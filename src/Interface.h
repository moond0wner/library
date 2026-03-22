#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "Library.h" 

void printHeader(const std::string& title);
void mainInterface();
void bookInterface();
void readerInterface();
void menuBooks(Library& lib);
void readerMenu(Library& lib);
void borrowBookInterface(Library& lib);
void returnBookInterface(Library& lib);
void overdueInterface(Library& lib);
bool hasNoData(const Library& lib);

#endif
