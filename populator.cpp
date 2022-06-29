#include <iostream>
#include <iomanip>
#include <map>
#include <random>
#include <initializer_list>
int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> p;
    p.push_back(40);
    p.push_back(10);
    p.push_back(10);
    p.push_back(40);
    std::discrete_distribution<> d(p.begin(), p.end());

    std::map<int, int> map;
    for(int n=0; n<10000; ++n) {
        ++map[d(gen)];
    }
    for(const auto [num, count] : map) {
        std::cout << num << " generated " << std::setw(4) << count << " times\n";
    }
}