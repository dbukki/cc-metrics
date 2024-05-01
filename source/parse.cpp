#include "parse.h"

#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;


istream& get_until(istream& in, string& token, char stop)
{
    token.clear();
    int ch;
    while ((ch = in.get()) >= 0 && ch != stop)
        if (token.length() > 0 || !isspace(ch))
            token += static_cast<char>(ch);
    return in;
}

string get_all(const string& file)
{
    ifstream in(file);
    ostringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

string unescape(const string& s)
{
    string res;
    for (size_t i = 0; i < s.length(); ++i)
    {
        if (s[i] == '\\')
        {
            ++i;
            switch (s[i])
            {
                case '0': res += '\0'; break;
                case 'a': res += '\a'; break;
                case 'b': res += '\b'; break;
                case 'f': res += '\f'; break;
                case 'n': res += '\n'; break;
                case 'r': res += '\r'; break;
                case 't': res += '\t'; break;
                case 'v': res += '\v'; break;
                case '\\': res += '\\'; break;
                case '\'': res += '\''; break;
                case '\"': res += '\"'; break;
            }
        }
        else res += s[i];
    }
    return res;
}

vector<string> split(const string& s, const string& delimiter)
{
    vector<string> chunks;
    size_t start = 0;
    size_t end;

    while ((end = s.find(delimiter, start)) != string::npos)
    {
        chunks.emplace_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }

    chunks.emplace_back(s.substr(start));
    return chunks;
}

double parse_double(const char* str)
{
    char* end;
    double d = strtod(str, &end);
    if (end == str)
        d = NAN;
    return d;
}
