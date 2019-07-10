#include "HashMap.hpp"
#include "hashmap.h"
#include <string>

HashMap::HashMap(size_t size)
{
	this->_map = Create_Map(size);
	this->size = size;
}

HashMap::HashMap()
{
	this->_map = New_Map();
	this->size = DEFAULT_MAP_SIZE;
}

HashMap::~HashMap()
{
	Map_close(_map);
}

void HashMap::put(char *key, void* value)
{
	Map_put(_map, key, value);
}

void HashMap::put(const char *key, void* value)
{
	Map_put(_map, (char*) key, value);
}

void HashMap::put(std::string key, void* value)
{
	Map_put(_map, (char*)key.c_str(), value);
}

void* HashMap::get(char *key)
{
	return Map_get(_map, key);
}

void* HashMap::get(const char *key)
{
	return Map_get(_map, (char*) key);
}

void* HashMap::get(std::string key)
{
	return Map_get(_map, (char*)key.c_str());
}

void HashMap::resize(size_t size)
{
	this->size = size;
	Map_resize(&_map, size);
}