#include "parse.h"
#include "config.h"

#include <fstream>
#include <sstream>

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
