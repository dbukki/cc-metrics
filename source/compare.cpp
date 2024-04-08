#include "compare.h"

#include <vector>

using namespace std;


size_t lcs(const string& a, const string& b)
{
    vector<size_t> buf(b.length() + 1, 0);
    size_t diag;
    for (size_t ai = 0; ai < a.length(); ++ai)
    {
        diag = 0;
        for (size_t bi = 0; bi < b.length(); ++bi)
        {
            size_t& ac = buf[bi];
            size_t& bc = buf[bi + 1];

            size_t cur;
            if (a[ai] == b[bi])
                cur = diag + 1;
            else if (ac > bc)
                cur = ac;
            else cur = bc;

            diag = bc;
            bc = cur;
        }
    }
    return buf[b.length()];
}

size_t dif(const string& a, const string& b)
{
    return a.length() + b.length() - 2 * lcs(a, b);
}
