#include <iostream>

using namespace std;

int fibRecur(int n, int* array) {
	// cout << "Recursing with " << n << " and " << array[4] << endl;
	if (array[n] == -1) {
		array[n] = fibRecur(n-1, array) + fibRecur(n-2, array);
	}

	return array[n];
}

int fib(int n, int* array) {
	array[0] = 0;
	array[1] = 1;

	fibRecur(n, array);

	int sum = 0;
	for (int i = n; i > 0; i--) {
		sum += array[i];

		cout << i;

		if (i < 10) {
			cout << ":  ";
		}
		else {
			cout << ": ";
		}

		cout << array[i] << endl;
	}

	return sum;
}

int main() {
	int memoArray[45] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
						  -1, -1, -1, -1, -1 };

	int ans = fib(44, memoArray);
	cout << "The sum of the fibonacci numbers is " << ans << endl;

	cout << "\nFinished, terminating." << endl;
}