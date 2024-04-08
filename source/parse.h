#pragma once
#include <iostream>
#include <string>


std::istream& get_until(std::istream& in, std::string& token, char stop);

std::string get_all(const std::string& file);
