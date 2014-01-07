#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include "Util.h"
#include "FileIndex.h"
#include "InvertedIndexGen.h"
#include "md5.h"

using namespace std;
extern Message_Digest Md5Hash;

InvertedIndexGen::InvertedIndexGen() {
    // Default constructor.
}

// The destructor iterates over each file index element deletes it.
InvertedIndexGen::~InvertedIndexGen() {
    map<string, FileIndex*>::iterator it = idx.begin();
    // iterate over each map element pair.
    while (it != idx.end()) {
        FileIndex* fi = it->second;
        // Now, delete each file index.
        while (fi != NULL) {
            FileIndex* p = fi;
            fi = fi->getNext();
            delete p;
        }
        it++;
    }
    // Next, delete all map entries.
    idx.clear();
}

// Takes a file name as an argument and builds the inverted index.
int InvertedIndexGen::build(const string& file) {
    vector<string> files;
	bool rebuild_index=true;
	if (loadIndexFile(files, file) == -1)
        return -1;
	Md5Hash.LoadIndex();
	if(Md5Hash.size()>0)
	{
		rebuild_index=!(Md5Hash.CompareMD5(files));//is there any need to rebuild
	}
	if(rebuild_index )
	{
		if (indexFiles(files) == -1)
			return -1;
		Md5Hash.CalculateMD5(files);
		Md5Hash.SaveMD5();
	}
	else
	{
		RestoreIndex("index.dat");//retore index from file
	}
    return 0;
}

// Looks up a word in the inverted index.
FileIndex* InvertedIndexGen::lookup(const string& word) {
    return idx[word];
}

// Loads the index file into the vector files.
int InvertedIndexGen::loadIndexFile(vector<string>& files, const string& idxfile) {
	ifstream infile(idxfile.c_str());
	if (infile) {
		string line;
		int lineno = 1;
		while (getline(infile, line)) {
			if (line == "")
				cerr << "[" << int_to_string(lineno)
				     << "] found blank line in input file. skipping." << endl;
			else
				files.push_back(line);
			lineno++;
		}
		return 0;
	}
	else {
		cerr << "can't open file " << idxfile << endl;
		return -1;
	}
}

// Indexes each file in the files vector.
int InvertedIndexGen::indexFiles(const vector<string>& files) {
    // Uncomment the following lines:
    vector<string> words; // Words in a file.
    set<string>    seen;  // Files we have "seen" (already indexed).
    int            fcnt;  // The file we are indexing.
    string         curr_file;
    string         curr_word;
    int status = 0;

    fcnt = 0;


    for (vector<string>::const_iterator file_it = files.begin(); file_it != files.end(); ++file_it){
        curr_file = *file_it;
        if(seen.find(curr_file) == seen.end()){
            seen.insert(curr_file);

            if(readWords(curr_file, words) != -1){
                for (vector<string>::iterator words_it = words.begin(); words_it != words.end(); ++words_it){
                    curr_word = *words_it;
                    insert(curr_word, fcnt,curr_file);
					curr_word.clear();
                }
				words.clear();
                fcnt++;
            }
            else {
                status = -1;
                fcnt++;
            }
        }
        else {
            status = -1;
            cout << "duplicate input file: " << curr_file << ". Skipping." << endl;
            fcnt++;
        }
    }
	PersistIndex();
	//RestoreIndex("index.dat");
    return status;
}

// Inserts a word into the inverted index.
void InvertedIndexGen::insert(const string& word, int fcnt,const string& fname) {
    if(idx.find(word) == idx.end()) {                // word not seen
        FileIndex* newEntry = new FileIndex(fcnt,fname);  // add new pair to idx map
        idx.insert(pair<string, FileIndex*>(word, newEntry) );
        return;
    }
    else {                                           // word has been seen
        FileIndex* curr = lookup(word);
        while(curr->getIndex() != fcnt && curr->getNext() != NULL){  // iterate through word's FileIndex objects
            curr = curr->getNext();
        }
        if((curr->getIndex() == fcnt)) {  // if there's an index match, do nothing
            return;
        }
        else {                        // if there's no match, add new FileIndex pointer to value list
            FileIndex* addIndex = new FileIndex(fcnt,fname);
            curr->setNext(addIndex);
        }
    }
}

// Returns true if c is an alpha character.
bool alpha(char c) {
	if((c >= -1 && c <= 255))
		return isalpha(c);
	return false;
}

// Returns true if c is not an alpha character.
bool not_alpha(char c) {
	if((c >= -1 && c <= 255))
		return !isalpha(c);
	else
		return false;
}

//
// This method splits the string str into a vector of strings.  That is, we
// split a sentence into words.
//
vector<string> split(const string& str) {
	vector<string> ret;
	string::const_iterator i = str.begin();
	while (i != str.end()) {
		// Ignore unrecognized characters (non-alpha):
		i = find_if(i, str.end(), alpha);

		// Find the next alpha word:
		string::const_iterator j = find_if(i, str.end(), not_alpha);

		// Copy the characters in [i, j)
		if (i != str.end())
			ret.push_back(string(i, j));

		i = j;
	}
	return ret;
}

//
// This method reads the words in the provided file into the vector v.
//
int InvertedIndexGen::readWords(const string& file, vector<string>& v) {
	ifstream infile(file.c_str());
	if (infile) {
		string line;
		while (getline(infile, line)) {
			vector<string> words = split(line);
			v.insert(v.end(), words.begin(), words.end());
		}
		return 0;
	}
	else {
		cerr << "can't open file " << file << endl;
		return -1;
	}
}

//
// This method converts the FileIndex list fi into a set of
// integers representing the file indexes.
//
void to_set(set<int>& s, FileIndex* fi) {
    FileIndex* p = fi;
    while (p != NULL) {
        s.insert(p->getIndex());
        p = p->getNext();
    }
}

//
// Returns the string representation of the inverted index.
//
string InvertedIndexGen::toString() {
    set<int> indexes;
    string   res = "";
    map<string, FileIndex*>::iterator it = idx.begin();
    
    while (it != idx.end()) {
        res += it->first + ": ";
        to_set(indexes, it->second);
        for (set<int>::iterator it2 = indexes.begin();
             it2 != indexes.end(); ++it2) {
            res += int_to_string(*it2) + " ";
        }    
        res += "\n";    
        indexes.clear();
        it++;
    }

    return res;
}

int InvertedIndexGen::numberOfWords() {
	return idx.size();
}

