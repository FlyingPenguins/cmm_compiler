// Sample program for C++ subset
// Computes the GCD of two integr

/* asdg */
/* asfdg
asdf
*/
#include <iostream>

int x, y;	// The two values whose GCD is to be computed

// In the subset, a and b are passed by reference, but in
// real C++, they are passed by value.  It doesn't matter
// in this program.
int gcd ( int a, int b )
{
	if ( b == 0 ) return a;
	else return gcd ( b, a % b );
}

int main()
{
	cout << "asfg";
    cin >>  x; /* Not reading ">>" properly */
	cout << x << gcd ( x, y ) << endl;
    
}
