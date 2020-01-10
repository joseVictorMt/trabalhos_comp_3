#include <vector>
#include <iostream>
#include <math.h>
#include <string>
#include <map>
#include <utility>

#include "stream.cc"

using namespace std;

int main( int argc, char* argv[]) {
    // Teste 1
    vector<int> v1 = { 2, 9, 8, 8, 7, 4 };
    auto result = v1 | []( int x ) { return x % 2 == 0; };
    for( auto x : result )
        cout << x << " ";
    cout << endl;

    // Teste 2
    v1 = { 2, 9, 8, 8, 7, 4 };
    v1 | []( int x ) { return x % 2 == 0; } | [] ( int x ) { cout << x << " "; };
    cout << endl;

    // Teste 3
    int v2[] = { 2, 9, 8, 8, 7, 4 };
    auto result2 = v2 | []( int x ) { return x % 2 == 0; };
    for( auto x : result2 )
        cout << x << " ";
    cout << endl;

    // Teste 4
    int v3[] = { 2, 9, 8, 8, 7, 4 };
    v3 | []( int x ) { return x % 2 == 0; } | [] ( int x ) { cout << x << " "; };
    cout << endl;

    // Teste 5
    map<string,string> v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" } };
    v | []( auto x ){ return pair{ x.first, stod( x.second ) }; } | []( auto p ) { cout << p.second + 1.1 << " "; };
    cout << endl;

    // Teste 6
    v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" }, { "f", "6" } };
    auto par = []( auto x ){ return stoi( x.second ) % 2 == 0; };
    auto upper_first = []( auto x ){ string st = x.first; transform(st.begin(), st.end(), st.begin(), ::toupper); return st; };
    v | par | upper_first | []( auto st ) { cout << st + "! "; };
    cout << endl;

    // Teste 7
    v = { { "a", "1" }, { "b", "2" }, { "c", "3" }, { "d", "4" }, { "e", "5" }, { "F", "6" } };
    //par = []( auto x ){ return stoi( x.second ) % 2 == 0; };
    auto first = []( pair<string,string> p ) { return p.first; };
    auto upper = []( string st ){ transform(st.begin(), st.end(), st.begin(), ::toupper); return st; };
    vector<string> result3 = v | par | first | upper;
    result3 | []( auto st ) { cout << st + "! "; };
    cout << endl;

    // Teste 8
    vector<string> v4 = { "janeiro", "fevereiro", "março", "abril", "maio" };
    v4 | &string::length | [] ( int x ) { cout << x << " "; };
    cout << endl;
}