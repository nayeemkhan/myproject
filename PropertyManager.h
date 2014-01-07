//#include "FileManager.h"
#include <string>

using namespace std;

class FileManager;
class PropertyManager
{
	friend class FileManager;
	string ShareFolder;
public:
	PropertyManager(){ShareFolder="d:\\test";}
};