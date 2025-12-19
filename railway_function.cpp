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

// エラーを表すenum(classなのは要素を参照する際に型名を付けたいので)
enum class localerror {
  read_error,
  invalid_argument,
  out_of_range,
  something_error,
};

using namespace std;
#if __cplusplus < 202302L
using namespace sresult;
#endif

expected<string, localerror> getline_from_stdin(nullptr_t) {
#if defined(USE_SRESULT)
  // deprecatedされたexceptionのunexpected関数との衝突回避
  using sresult::unexpected;
#endif
  // 標準入力から1行読んで返す
  string s;
  if (getline(cin, s)) {
    // 1行分の文字列を返す
    return {s};
  } else {
    // エラーを返す
    return unexpected{localerror::read_error};
  }
}

expected<int, localerror> string_to_int(const string& s) {
#if defined(USE_SRESULT)
  // deprecatedされたexceptionのunexpected関数との衝突回避
  using sresult::unexpected;
#endif
  try {
    // 文字列sをintにして返す
    return {stoi(s)};
  } catch (invalid_argument e) {
    return unexpected{localerror::invalid_argument};
  } catch (out_of_range e) {
    return unexpected{localerror::out_of_range};
  }
}

expected<vector<int>, localerror> input_numbers(int n) {
#if defined(USE_SRESULT)
  // deprecatedされたexceptionのunexpected関数との衝突回避
  using sresult::unexpected;
#endif
  // 整数値をn回入力した結果を返す関数
  vector<int> v;
  v.reserve(n);
  for (int i = 0; i < n; ++i) {
    auto r = expected<nullptr_t, localerror>(nullptr)
                 .and_then(getline_from_stdin)
                 .and_then(string_to_int);
    if (r) {
      // vectorに読み込んだint値を追加
      v.push_back(r.value());
    } else {
      // エラーを返す
      return unexpected{r.error()};
    }
  }
  // n回分の読み込んだint値を返す
  return v;
}

int sum(const vector<int>& v) {
  // int値ベクタを合計して返す
  int sum = 0;
  for (auto e : v) sum += e;
  return sum;
}

wstring error_to_wstring(localerror e) {
  // enumエラー値をエラー文字列に変換
  switch (e) {
    case localerror::read_error:
      return L"読み込みエラー!";
    case localerror::invalid_argument:
      return L"不正な引数!";
    case localerror::out_of_range:
      return L"範囲外!";
    default:
      return L"不明エラー";
  }
}

int main() {
  setlocale(LC_ALL, "");
  auto r = expected<int, localerror>{3}  // 正常値3のexpectedを作成
               .and_then(input_numbers)
               .transform(sum)
               .transform_error(error_to_wstring);
  // 結果を表示
  wcout << (r ? (L"合計: " + to_wstring(r.value())) : (L"エラー: " + r.error()))
        << endl;
}