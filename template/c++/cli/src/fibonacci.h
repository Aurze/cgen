/*
 * Calculate the Fibonacci of value.
 *
 * @param n possition of the value in the fibonacci sequence.
 */
template <typename T> constexpr T fibonacci(T n) {
  return n < 1 ? 0
               : (n == 1 || n == 2 ? 1 : fibonacci(n - 1) + fibonacci(n - 2));
}
