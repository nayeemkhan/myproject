#include "FileIndex.h"
#include <cstddef>

//
FileIndex::FileIndex() {
  
}


// Creates a new file index given the index i.
//
FileIndex::FileIndex(int i) {
  index = i;
  next  = NULL;
}

FileIndex::FileIndex(int i,std::string fname) {
  index = i;
  filename=fname;
  next  = NULL;
}

//
// Returns the next file index in this list.
//
FileIndex* FileIndex::getNext() {
  return next;
}

std::string FileIndex::getfilename() {
  return filename;
}
//
// Sets the next file index in this list.
//
void FileIndex::setNext(FileIndex* n) {
  next = n;
}

//
// Returns the index.
//
int FileIndex::getIndex() {
  return index;
}
