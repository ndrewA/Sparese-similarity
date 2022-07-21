#include <iostream>

#include <vector>
#include <unordered_set>
#include <unordered_map>

struct PairHash {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);

        if (hash1 != hash2) {
            return hash1 ^ hash2;
        }

        return hash1;
    }
};

std::unordered_map<std::pair<int, int>, float, PairHash> getSimilarity(std::unordered_map<int, std::vector<int>>& documents)
{
    std::unordered_map<int, std::unordered_set<int>> docNums;
    std::unordered_map<std::pair<int, int>, int, PairHash> inters;

    for(const auto& doc : documents) {
        const int newId = doc.first;
        for(const auto& num : doc.second) {
            for(auto oldId : docNums[num])
                ++inters[{oldId, newId}];
            docNums[num].insert(newId);
        }
    }

    std::unordered_map<std::pair<int, int>, float, PairHash> similarity;

    for(const auto& docInters : inters) {
        int intersection = docInters.second;
        int id1 = docInters.first.first;
        int id2 = docInters.first.second;
        size_t size1 = documents[id1].size();
        size_t size2 = documents[id2].size();
        float curSim = (float)intersection/(size1+size2-intersection);
        if(curSim > 0) similarity[docInters.first] = curSim;
    }
    return similarity;
}

int main()
{
    std::unordered_map<int, std::vector<int>> documents = {{13, {14, 15, 100, 9, 3}},
                                                           {16, {32, 1, 9, 3, 5}},
                                                           {19, {15, 29, 2, 6, 8, 7}},
                                                           {24, {7, 10}}};
    auto res = getSimilarity(documents);
    for(auto i : res)
        std::cout << i.first.first << ' ' << i.first.second << " : " << i.second << '\n';
    return 0;
}
