#include <set>
#include <string>

using namespace std;

class Solution
{
public:
    int lengthOfLongestSubstring(string s)
    {
        int leftInclusive = 0, rightExclusive = 0, maxSize = 0;
        set<char> charSet;

        while (rightExclusive < s.size())
        {
            while (charSet.find(s[rightExclusive]) != charSet.end())
            {
                charSet.extract(s[leftInclusive]);
                leftInclusive++;
            }

            charSet.insert(s[rightExclusive]);
            rightExclusive++;

            if (maxSize < charSet.size())
                maxSize = charSet.size();
        }

        return maxSize;
    }
};