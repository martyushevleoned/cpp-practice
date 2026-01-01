/*
    Given int array & target, return indices of 2 nums that add to target
    Ex. nums = [2,7,11,15] & target = 9 -> [0,1], 2 + 7 = 9

    At each num, calculate complement, if exists in hash map then return

    Time: O(n)
    Space: O(n)
*/

#include <map>
#include <vector>

using namespace std;

class Solution
{
public:
    vector<int> twoSum(vector<int>& nums, int target)
    {
        map<int, int> valueToIndexMap;

        for (int i = 0; i < nums.size(); i++)
        {
            int pair = target - nums.at(i);
            if (valueToIndexMap.find(pair) != valueToIndexMap.end())
                return {i, valueToIndexMap[pair]};
            valueToIndexMap.insert({nums.at(i), i});
        }

        return {};
    }
};