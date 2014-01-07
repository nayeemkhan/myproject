#include <iostream>
#include "InvertedIndexGen.h"
#include "md5.h"
#include "FileManager.h"

using namespace std;

Message_Digest Md5Hash;

// Main program entry point.
int main(int argc, char* argv[]) {
	// Check the program arguments.
	if (argc != 2) {
		cerr << "usage: Indexer file" << endl;
		return 1;
	}
	cout << "Test" << endl;

	//InvertedIndexGen ivgen;    	// Create the inverted index generator.
	//ivgen.build(argv[1]);		// Build the index.
	//cout << ivgen.toString();	// Return the a string representation of the index.
	//cout << ivgen.numberOfWords() << " words" << endl;

	//FileIndex *Fdx=(ivgen.lookup("nayeem"));
	//cout<<"File Index for "<<"nayeem = ";
	//while(Fdx)
	//{
	//	cout<< Fdx->getfilename();
	//	cout<<",";
	//	Fdx=Fdx->getNext();
	//}
	
	FileManager F1;
	string path="D:\\test";
	F1.ProcessShareFolder(path,true);
	F1.IndexFiles();
	vector<string> files;
	F1.SearchFiles((string)"Shreya",files);
	return 0;
}
