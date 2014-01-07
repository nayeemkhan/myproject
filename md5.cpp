#include "md5.h"
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <iomanip> 
using namespace std;
unsigned char result[MD5_DIGEST_LENGTH];

Message_Digest::Message_Digest()
{
	//default constructor
}

Message_Digest::Message_Digest(string & IndxFilename)
{
		std::ifstream ifs(IndxFilename, std::ios::binary); 
		if(ifs)
		{
			boost::archive::binary_iarchive ia(ifs); 
			Hashmap.clear();
			ia>>Hashmap;
		}
}



void Message_Digest::CalculateMD5(std::vector<std::string>& vec_filenames)
{

	std::string curr_file;

	
	Hashmap.clear();
	for (std::vector<std::string>::const_iterator file_it = vec_filenames.begin(); file_it != vec_filenames.end(); ++file_it)
	{
		MD5_CTX mdContext;
		curr_file = *file_it;
		FILE *inFile;
		fopen_s(&inFile,curr_file.c_str(), "rb");
		int bytes;
		unsigned char data[1024];
		if (inFile == NULL) {
			cout<<curr_file<<" can't be opened.\n";
			return ;
		 }

		MD5_Init (&mdContext);
		while ((bytes = fread (data, 1, 1024, inFile)) != 0)
			MD5_Update (&mdContext, data, bytes);

		MD5_Final (result,&mdContext);
		stringstream buf;
		for(int i = 0; i < MD5_DIGEST_LENGTH; i++)
		{
			buf << hex << setfill('0') << setw(2) << static_cast<int>(static_cast<unsigned char>(result[i])); 
		}
		Hashmap.insert(pair<string, string>(curr_file, buf.str()) );
	}
	
   
}

void Message_Digest::SaveMD5()
{
		std::ofstream indexFile("Hash.dat", std::ios_base::binary); 
		if(indexFile)
		{
			boost::archive::binary_oarchive oa(indexFile); 
			oa<<Hashmap;
			indexFile.close();
		}
}

bool Message_Digest::LoadIndex()
{
	std::ifstream ifs("Hash.dat", std::ios::binary); 
	if(ifs)
	{
        boost::archive::binary_iarchive ia(ifs); 
		Hashmap.clear();
		ia>>Hashmap;
		ifs.close();
		return true;
	}
	return false;
}



bool Message_Digest::CompareMD5(std::vector<std::string>& vec_filenames)
{
	/*string indxfile="Hash.dat";
	Message_Digest Current(indxfile);*/
	Message_Digest NewList;
	NewList.CalculateMD5(vec_filenames);
	if(Hashmap.size() != NewList.Hashmap.size())
		return false;
	map<string, string>::iterator it_lhs = Hashmap.begin();
	map<string, string>::iterator it_rhs = NewList.Hashmap.begin();

	 for (; it_lhs!=Hashmap.end() && it_rhs != NewList.Hashmap.end(); ++it_lhs,++it_rhs)
	 {
		 if(it_lhs->first == it_rhs->first && it_lhs->second == it_rhs->second)
			 continue;
		 else
			 return false;
	 }
	 return true;
}


bool operator==( const Message_Digest& lhs, const Message_Digest& rhs ) {
     // map<string, string>::iterator it_lhs = lhs.Hashmap.begin();

	return true;
  }