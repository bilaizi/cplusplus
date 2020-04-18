#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

struct Part {
};

struct Whole {
    Part p1, p2;
};
struct Asset {
    std::string ticker;
    //...more fields
};

struct Basket {
    Basket(const std::vector<std::string>& tickers) {
        //Load data from database and create assets
        for(auto& t : tickers)
            assets.push_back({t /*,more fields*/});
    }
    std::vector<Asset> assets;
};

void getBasketAssets(const std::vector<std::string>& tickers,
    std::vector<std::shared_ptr<Asset>>& assets) {
    //Create a basket that loads assets
    auto bp = std::make_shared<Basket>(tickers);
    for(auto& a : bp->assets) {
        //Aliasing constructor
        assets.push_back(std::shared_ptr<Asset>(bp, &a));
    }
}

int main() {
    //Create an alias for readability
    using PartPtr = std::shared_ptr<Part>;
    auto wp = std::make_shared<Whole>();
    //Create std::shared_ptr<Part> using aliasing
    auto pp1 = PartPtr(wp, &wp->p1);
    auto pp2 = PartPtr(wp, &wp->p2);
    /*Shows that pp1 and pp2 are compared as equal 
        using std::owner_less but not with std::less*/
    std::cout << std::boolalpha
                << std::less<PartPtr>()(pp1, pp2) //true 
                << std::less<PartPtr>()(pp2, pp1) //false 
                << std::owner_less<PartPtr>()(pp1, pp2) //false
                << std::owner_less<PartPtr>()(pp2, pp1); //false

    //create a set with default std::less comparator
    std::set<PartPtr> pset;
    //create a set with std::owner_less comparator
    std::set<PartPtr,std::owner_less<PartPtr>> pownerset;

    pset.insert(pp1); //inserts
    pset.insert(pp2); //inserts

    pownerset.insert(pp1); //inserts
    pownerset.insert(pp2); //returns existing

    std::cout << pset.size(); //2
    std::cout << pownerset.size(); //1

    std::vector<std::shared_ptr<Asset>> allAssets;

    getBasketAssets({"KO","PEP"},allAssets); // 2 Assets
    getBasketAssets({"MSFT","AAPL","FB","AMZN"},allAssets); // 4 Assets
    getBasketAssets({"SBUX","MCD","CMG"},allAssets); // 3 Assets
    getBasketAssets({"BD"},allAssets); //1 Assets
    using Comparator = std::owner_less<std::shared_ptr<Asset>>; 
    std::map<std::shared_ptr<Asset>, size_t, Comparator> countMap;
    for(auto& ap : allAssets)
        countMap[ap]++;
    for(auto& kvp : countMap)
        std::cout << kvp.second << " ";
    return 0;
 }
