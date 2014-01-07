#include "FileManager.h"


FileManager::FileManager()
{
	AFM = new AudioFileManager;

}

void FileManagerBase::GetFileInfo(string &filename)
{
	uintmax_t filesize=file_size(filename.c_str());
}

void FileManager::ProcessShareFolder(string& Dir,bool subfolders)
{
	recursive_directory_iterator rdi(Dir);  
	recursive_directory_iterator end_rdi;
	string curr_file;

	DirFileList.empty();
	filemap.clear();
	if(_DEBUG)
	cout<<"\n";
	for (; rdi != end_rdi; rdi++)
	{
		if(_DEBUG)
		{
			cout << (*rdi).path().string();
			cout << " <----- " << (*rdi).path().extension() << endl;
		}
		DirFileList.push_back((*rdi).path().string());
		filemap.insert(pair<string, boost::filesystem::path>((*rdi).path().string(),(*rdi).path()));
	}

	

	for (std::vector<std::string>::const_iterator file_it = DirFileList.begin();
		file_it != DirFileList.end(); 
		++file_it)
	{
		curr_file = *file_it;
		boost::filesystem::path curr_path;
		map<string, boost::filesystem::path>::iterator filemap_itr = filemap.begin();
		for(;
			filemap_itr !=filemap.end();
			++filemap_itr)
		{
			if(filemap_itr->first == curr_file)
			{
				curr_path= filemap_itr->second;
				break;
			}
		}
		
		string ext = curr_path.extension().string();

		if((ext) == ".mp3") 
		{
			//AudioFileManager *Audiofile = new AudioFileManager();
			AFM->GetFileProperties(curr_file);
		}
		if(ext == ".txt")
		{
			FileManagerBase *Txtfile = new FileManagerBase();
			Txtfile->GetFileProperties(curr_file);
		}
		if(ext == ".pdf")
		{

		}
		if(ext == ".mp4")
		{

		}
		if(ext == ".doc")
		{

		}
		ext.clear();
		
	}


}

//void FileManager::GetFileProperties(string & filename)
//{
//
//}

void AudioFileManager::GetFileProperties(string &filename)
{
	
	TAGdata test;
	ifstream mp3File;

	mp3File.open(filename.c_str(),ios::binary);
	mp3File.seekg (-128, ios::end);
	mp3File.read(reinterpret_cast <char *>(&test), 128);
	mp3File.close();

	string txtfilename= filename+".txt";
	std::ofstream txtFile(txtfilename);
	if(txtFile)
	{
		for(int i=0;i<30;i++)
		{
			if(test.album[i]!=NULL)
			txtFile<<test.album[i];
		}
		txtFile<<" ";

		for(int i=0;i<30;i++)
		{
			if(test.title[i]!=NULL)
			txtFile<<test.title[i];
		}
		txtFile<<" ";

		for(int i=0;i<30;i++)
		{
			if(test.artist[i]!=NULL)
			txtFile<<test.artist[i];
		}
		txtFile<<" ";

		for(int i=0;i<30;i++)
		{
			if(test.comment[i]!=NULL)
			txtFile<<test.comment[i];
		}
		txtFile<<" ";
		
		for(int i=0;i<4;i++)
			txtFile<<test.year[i];
		txtFile<<" ";

		for(int i=0;i<3;i++)
			txtFile<<test.tag[i];
		txtFile<<" ";

		
		txtFile<<test.genre;
		txtFile<<" ";
		
	}
	Filemap.insert(pair<string,string>(filename,txtfilename));
	txtFile.close();
if(_DEBUG)
	cout << test.tag << endl << test.title << endl << test.artist << endl << test.album << endl << test.year << endl << test.comment << endl;
}

void FileManager::IndexFiles()
{
	vector<string> txtFileVec;

	map<string, boost::filesystem::path>::iterator filemap_itr = filemap.begin();
	for(;
		filemap_itr !=filemap.end();
		++filemap_itr)
	{
		if((filemap_itr->second).extension().string() == ".txt")
			txtFileVec.push_back(filemap_itr->first);
	}
	std::ofstream txtFile("index.txt");

	for (std::vector<std::string>::const_iterator file_it = txtFileVec.begin();
		file_it != txtFileVec.end(); 
		++file_it)
	{
		txtFile << *file_it;
		txtFile <<"\n";
	}
	txtFile.close();
	ivgen.build("index.txt");
}

string AudioFileManager::GetFileMapping(string &filename)
{
	map<string, string>::iterator filemap_itr = Filemap.begin();
	for(;
		filemap_itr !=Filemap.end();
		++filemap_itr)
	{
		if(filemap_itr->second == filename)
			return filemap_itr->first;
	}

}
bool FileManager::SearchFiles(string& searchstring,vector<std::string> &files)
{
	FileIndex *Fdx=(ivgen.lookup(searchstring));
	cout<<"File Index for "<<searchstring;
	vector<string> tempList;
	while(Fdx)
	{
		cout<< Fdx->getfilename();
		cout<<",";
		tempList.push_back(Fdx->getfilename());
		Fdx=Fdx->getNext();
	}

	if(tempList.size()>0)
	{
		for (std::vector<std::string>::const_iterator file_it = tempList.begin();
			file_it != tempList.end(); 
			++file_it)
		{
			string tstring = *file_it;
			if(tstring.find("mp3") != string::npos)
			{
				string newfile = AFM->GetFileMapping(tstring);
				files.push_back(newfile);
			}
			else
			{
				files.push_back(tstring);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}