#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#if __cplusplus < 202302L
#define USE_SRESULT
#endif
#if defined(USE_SRESULT)
#include "sresult.h"
#else
#include <expected>
#endif
int main() {
  enum class localerror {
    something_error,
  };
  using namespace std;
  using namespace chrono;
#if defined(USE_SRESULT)
  using namespace sresult;
  using sresult::unexpected;
#endif
  setlocale(LC_ALL, "");
  system("free -h");
  auto s = high_resolution_clock::now();
  auto print_time = [s] {
    auto e = high_resolution_clock::now();
    cout << "time: " << duration_cast<milliseconds>(e - s).count() / 1000.
         << "\n";
  };
  constexpr size_t N = 1024 * 1024 * 1024 / sizeof(int);
  auto recursive_call = [&print_time](auto& self, auto&& obj, auto fn,
                           auto n) -> expected<vector<int>, localerror> {
    if (n > 0) {
      print_time();
      return self(self, std::move(obj).transform_error(fn), fn, n - 1);
    }
    return obj;
  };
  auto r = recursive_call(
      recursive_call, expected<int, localerror>{N}.transform([](auto n) {
        vector<int> v;
        v.reserve(n);
        for (int i = 0; i < n; ++i) v.push_back(i);
        return v;
      }),
      [](auto e) { return e; }, 10);
  print_time();
  cout.flush();
  system("free -h");
  wcout << (r ? (L"size: " + to_wstring(r.value().size()))
              : (L"エラー: 何かのエラー"))
        << endl;
}