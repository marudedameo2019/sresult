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
template <typename T>
struct myvec : public std::vector<T> {
  using base = std::vector<T>;
  myvec() { std::cerr << "myvec()" << std::endl; }
  myvec(const myvec& r) : base{r} {
    std::cerr << "myvec(const myvec&)" << std::endl;
  }
  myvec(myvec&& r) : base{std::move(r)} {
    std::cerr << "myvec(myvec&&)" << std::endl;
  }
  ~myvec() { std::cerr << "~myvec()" << std::endl; }
};

int main() {
  enum class localerror {
    something_error,
  };
  using namespace std;
#if defined(USE_SRESULT)
  using namespace sresult;
  using sresult::unexpected;
#endif
  setlocale(LC_ALL, "");
  auto create_vec = [](auto n) -> expected<myvec<int>, localerror> {
    std::cerr << "create_vec(auto)" << std::endl;
    myvec<int> v;
    v.reserve(n);
    for (int i = 0; i < n; ++i) v.push_back(i);
    return v;
  };
  auto nop = [](auto&& v) {
    std::cerr << "nop(auto)" << std::endl;
    return v;
  };
  auto err2str = [](auto e) -> wstring {
    std::cerr << "err2str(auto)" << std::endl;
    return L"不明エラー";
  };
  {
    cout << "[one variable]" << endl;
    auto r = expected<int, localerror>{3}
                 .and_then(create_vec)
                 .transform(nop)
                 .transform_error(err2str);
    cout << r.value().size() << endl;
  }
  cout << endl;
  {
    cout << "[not const]" << endl;
    auto r1 = expected<int, localerror>{3};
    auto r2 = r1.and_then(create_vec);
    auto r3 = r2.transform(nop);
    auto r4 = r3.transform_error(err2str);
    cout << r2.value().size() << endl;
    cout << r3.value().size() << endl;
    cout << r4.value().size() << endl;
  }
  cout << endl;
  {
    cout << "[const]" << endl;
    const auto r1 = expected<int, localerror>{3};
    const auto r2 = r1.and_then(create_vec);
    const auto r3 = r2.transform(nop);
    const auto r4 = r3.transform_error(err2str);
    cout << r2.value().size() << endl;
    cout << r3.value().size() << endl;
    cout << r4.value().size() << endl;
  }
}