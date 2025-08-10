#include <algorithm>
#include <vector>

using namespace std;

class Solution
{
public:
    double findMedianSortedArrays(const vector<int>& nums1, const vector<int>& nums2)
    {
        vector<int> mergedVector(nums1.begin(), nums1.end());
        mergedVector.insert(mergedVector.end(), nums2.begin(), nums2.end());
        stable_sort(mergedVector.begin(), mergedVector.end());
        return findMedianSortedArray(mergedVector);
    }
    double findMedianSortedArray(vector<int>& nums)
    {
        return nums.size() % 2 == 1 ? nums[nums.size() / 2]
                                    : (nums[nums.size() / 2 - 1] + nums[nums.size() / 2]) / 2.;
    }
};