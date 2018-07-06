#include <cassert>
#include "leveldb/db.h"
#include <fstream>
#include <string>

std::string readAFile(std::string name) {
    std::ifstream ifs(name);
  	std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
    return content;
}

int main(){
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, "../chaindata", &db);
	assert(status.ok());

	// 
	std::string file1 = readAFile("key_bodyKey");
	std::string file2 = readAFile("val_bodyKey");
	db->Put(leveldb::WriteOptions(), file1, file2);

	std::string file3 = readAFile("key_hashKey");
	std::string file4 = readAFile("val_hashKey");
	db->Put(leveldb::WriteOptions(), file3, file4);

	std::string file5 = readAFile("key_headerKey");
	std::string file6 = readAFile("val_headerKey");
	db->Put(leveldb::WriteOptions(), file5, file6);

	std::string file7 = readAFile("key_numberKey");
	std::string file8 = readAFile("val_numberKey");
	db->Put(leveldb::WriteOptions(), file7, file8);

	std::string file9 = readAFile("key_lookupKey");
	std::string file10 = readAFile("val_lookupKey");
	db->Put(leveldb::WriteOptions(), file9, file10);

	std::string file11 = readAFile("key_receiptsKey");
	std::string file12 = readAFile("val_receiptsKey");
	db->Put(leveldb::WriteOptions(), file11, file12);

	delete db;
}
