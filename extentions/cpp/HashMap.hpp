#pragma once

#include "hashmap.h"
#include <cstddef>
#include <string>

class HashMap
{
private:
	size_t size;
	Map* _map;

public:
	HashMap();
	HashMap(std::size_t);
	~HashMap();

	void put(char*, void*);
	void put(const char*, void*);
	void put(std::string, void*);

	void* get(char*);
	void* get(const char*);
	void* get(std::string);

	void del(char*);
	void del(const char*);
	void del(std::string);

	void resize(std::size_t);
};