#include "PropertyManager.h"
#include <string>
#include <iostream>
#include <boost/filesystem.hpp>
#include <map>
#include <id3/tag.h>
#include <tag.h>
#include "InvertedIndexGen.h"
#include <vector>
using namespace boost::filesystem;
using namespace std;

struct TAGdata
{
	char tag[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];
	char genre;
};

struct FileInfo
{
	string name;
	string cdate;
	string mdate;
	uintmax_t size;

};
class AudioFileManager;
class VideoFileManager;
class PDFFileManager;
class FileManager
{
private:
	static PropertyManager pMgr;
	enum type {TEXT,PDF,DOC,MP3,AVI,MPEG,DAT,MP4};
	std::map<std::string,FileInfo> FileInfomap; //mapping of file to FileInfo
	vector<string> DirFileList ; //List of files in shared directory
	vector<string> DirFileExtList;
//	vector<FileManagerBase*> FMBptr;
	AudioFileManager *AFM;
	VideoFileManager *VFM;
	PDFFileManager *PDFM;

	map<string,boost::filesystem::path> filemap;
	InvertedIndexGen ivgen;    	// Create the inverted index generator.
public:
	void ProcessShareFolder(string& path,bool subfolders);
	//virtual void GetFileProperties(string & filename);
	void GetFileInfo(string &filename); //filename should be specified with complete path
	void IndexFiles(); //index txt files only
	bool SearchFiles(string& string,vector<std::string> &files);
	FileManager();
};

class FileManagerBase
{
private:
	static PropertyManager pMgr;
	enum type {TEXT,PDF,DOC,MP3,AVI,MPEG,DAT,MP4};
	
	
public:
	std::map<std::string, string> Filemap; //mapping for text to actual filename
	virtual void GetFileProperties(string & filename){};//this retrieves file metadata which is file type specific
	virtual void ConvertFileToText(string & filename){};
	void GetFileInfo(string &filename); //filename should be specified with complete path
};


class PDFFileManager:public FileManagerBase
{
private:

public:
	void GetFileProperties(string &filename);

};

class AudioFileManager:public FileManagerBase
{
private:
	
public:
	void GetFileProperties(string &filename);
	string GetFileMapping(string &filename);
};

class VideoFileManager:public FileManagerBase
{
private:
	
public:
	void GetFileProperties(string &filename);
};

class DocFileManager:public FileManagerBase
{
private:

public:
	void GetFileProperties(string &filename);
};

