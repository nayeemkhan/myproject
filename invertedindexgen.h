#ifndef _INVERTED_INDEX_GEN_H
#define _INVERTED_INDEX_GEN_H
#include <boost/serialization/map.hpp>
#include <boost/archive/binary_oarchive.hpp> 
#include <boost/archive/binary_iarchive.hpp> 
#include <map>
#include <vector>
#include <string>
#include "FileIndex.h"
#include <fstream> 
// The class definition for the inverted index generator.
class InvertedIndexGen {
 public:
    InvertedIndexGen();
    ~InvertedIndexGen();
    int build(const std::string&);
    FileIndex* lookup(const std::string&);
    std::string toString();
    int numberOfWords();
	
 private:
    std::map<std::string, FileIndex*> idx;
    
    // utility routines
    int loadIndexFile(std::vector<std::string>&, const std::string&);
    int indexFiles(const std::vector<std::string>&);
    int readWords(const std::string&, std::vector<std::string>&);
    void insert(const std::string&, int,const std::string&);
	friend class boost::serialization::access; 
	void PersistIndex(){
		std::ofstream indexFile("index.dat", std::ios_base::binary); 
		if(indexFile)
		{
			boost::archive::binary_oarchive oa(indexFile); 
			oa<<idx;
			indexFile.close();
		}
	}

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(idx);
    }
	void DeleteIndex(){idx.clear();}
	void RestoreIndex(const std::string& filename )
	{
		std::ifstream ifs(filename, std::ios::binary); 
		if(ifs)
		{
			boost::archive::binary_iarchive ia(ifs); 
			idx.clear();
			ia>>idx;
			ifs.close();
		}
	}
};

#endif
