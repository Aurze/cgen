/*
 * Calculate the Fibonacci of value passed as T
 */
template <int T> struct Fibonacci {
  enum { value = (Fibonacci<T - 1>::value + Fibonacci<T - 2>::value) };
};

/*
 *Calculate the Fibonacci of 0
 */
template <> struct Fibonacci<0> {
  enum { value = 1 };
};

/*
 *Calculate the Fibonacci of 1
 */
template <> struct Fibonacci<1> {
  enum { value = 1 };
};

int main() {
  int x = Fibonacci<45>::value;
  cout << x << endl;
}
