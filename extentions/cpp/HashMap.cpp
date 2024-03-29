#include "HashMap.hpp"
#include "hashmap.h"
#include <iostream>
#include <string>

HashMap::HashMap(size_t size)
{
    this->_map = create_map(size);
    this->size = size;
}

HashMap::HashMap()
{
    this->_map = new_map();
    this->size = DEFAULT_MAP_SIZE;
}

HashMap::~HashMap()
{
    map_close(_map);
}

size_t HashMap::item_count()
{
    return _map->item_count;
}

void HashMap::put(char* key, void* value)
{
    map_put(_map, key, value);
}

void HashMap::put(const char* key, void* value)
{
    map_put(_map, (char*)key, value);
}

void HashMap::put(std::string key, void* value)
{
    map_put(_map, (char*)key.c_str(), value);
}

void* HashMap::get(char* key)
{
    return map_get(_map, key);
}

void* HashMap::get(const char* key)
{
    return map_get(_map, (char*)key);
}

void* HashMap::get(std::string key)
{
    return map_get(_map, (char*)key.c_str());
}

void HashMap::resize(size_t size)
{
    this->size = size;
    map_resize(&_map, size);
}

std::vector<std::string> HashMap::keys()
{
    char* skeys[_map->item_count];
    std::vector<std::string> vkeys;
    map_keys(_map, skeys);
    for (size_t i = 0; i < _map->item_count; i++)
        vkeys.push_back(skeys[i]);

    return vkeys;
}

void HashMap::keys(char** keys)
{
    map_keys(_map, keys);
}