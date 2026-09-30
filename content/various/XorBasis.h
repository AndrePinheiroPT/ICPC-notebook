/**
 * Author: Caio
 * Date: 2015-05-12
 * License: CC0
 * Source: own work
 * Description: Struct that computes the xor basis by adding values. Allows adding other basis too.
 * Status: tested
 */

struct Basis{
    vector<int> basis;
    Basis(){}
    Basis(int x){
        add(x);
    }
    Basis operator+(Basis other) const{
        Basis res;
        for(int x:basis) res.add(x);
        for(int x:other.basis) res.add(x);
        return res;
    }
    void add(int x){
        for(auto& i:basis) x=min(x, x^i);
        if(x) basis.push_back(x);
    }
}