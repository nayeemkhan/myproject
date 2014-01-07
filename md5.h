#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fstream> 
#include <map>
#include <vector>
#include <boost/serialization/map.hpp>
#include <boost/archive/binary_oarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp>
#include <openssl/md5.h>
using namespace std;

//Class for handing MD5 hash
class Message_Digest
{
private:
	std::map<std::string, string> Hashmap; //map to save md5 hash 
	
	
	
public:
	Message_Digest();
	Message_Digest(std::string &);
	bool CompareMD5(std::vector<std::string>& vec_filenames);
	void CalculateMD5(std::vector<std::string>& vec_filenames);
	void SaveMD5();
	bool LoadIndex();
	int size(){return Hashmap.size();}
	 friend bool operator== (Message_Digest &Lhs, Message_Digest &Rhs);
	//Message_Digest& operator=( Message_Digest& rhs );
};