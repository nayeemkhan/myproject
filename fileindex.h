#ifndef _FILE_INDEX_H
#define _FILE_INDEX_H
#include <cstddef>
#include <string>

//
// Represents an index to a file.
//
class FileIndex {
 public:
  FileIndex();
  FileIndex(int);
  FileIndex(int,std::string);
  FileIndex* getNext();
  int getIndex();
  std::string getfilename();
  void setNext(FileIndex*);
   template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
    {
        ar & index;
		ar & filename;
		ar & next;
    }
 private:
  int index;       // The index of the file a word was found in.
  std::string filename;
  FileIndex* next; // The next file index.
 
};

#endif
