#pragma once
#include <iostream>
#include <string>
#include <vector>


std::istream& get_until(std::istream& in, std::string& token, char stop);

std::string get_all(const std::string& file);

std::string unescape(const std::string& s);

std::vector<std::string> split(const std::string& s, const std::string& delimiter);
