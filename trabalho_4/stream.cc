#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print( int x ) {
	cout << x << endl;
}

template<typename T, int N, class Function>
void operator | ( const T(& array)[N], Function fn ) {
	vector<T> temp ( array, array + N );
	for_each( temp.begin(), temp.end(), fn );
} 

template<
	typename T,
	typename A,
	template<typename, typename> class Estrutura,
	class Function
>
void operator | ( Estrutura<T, A> vetor, Function fn ) {
	for_each( vetor.begin(), vetor.end(), fn );
} 

int main( void ) {
	int tab[10] = { 1, 2, 3, 2, 3, 4, 6, 0, 1, 8 };
	vector<int> v{ 2, 6, 8 };
	tab | []( int x ) { cout << x*x << endl; };
	cout << endl;
	tab | [ &v ]( int x ) { v.push_back( x ); };
	v | []( int x ) { cout << x*x << endl; };
	cout << endl;
	v | &print;
	cout << endl;
	return 0;
}
