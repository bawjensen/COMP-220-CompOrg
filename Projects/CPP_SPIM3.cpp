#include <iostream>

using namespace std;

int fib(int n, int v1, int v2) {
	if (n > 0) {
		int val = fib(n-1, v2, v1+v2);
		cout << "v1 " << n << ": " << v1 << endl;

		return val + v1;
	}
	else {
		return 0;
	}
}

int main() {
	int sum = fib(44, 1, 1);

	cout << "Sum: " << sum << endl;
}