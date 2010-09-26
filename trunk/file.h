#include <fstream>
#include <string>
#include <sstream>

class datFile {
public:
datFile(const char *filename);
~datFile();
ofstream dFile;
void open(const char *filename);
void close();
void init(string data);
void append(string data);
};

datFile::datFile(const char *filename)
{
    dFile.open(filename); // create file to store fitness data   
}

datFile::~datFile()
{
    dFile.close();
}

void datFile::open(const char *filename)
{
    dFile.open(filename); // create file to store fitness data   
}

void datFile::close()
{
    dFile.close();
}

void datFile::init(string data)
{
    dFile << data;
}

void datFile::append(string data)
{
    dFile << data << endl;
}


