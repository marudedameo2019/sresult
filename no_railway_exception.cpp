#include <iostream>
#include <string>
#include <vector>

// エラーを表すenum(classなのは要素を参照する際に型名を付けたいので)
enum class localerror {
  read_error,
  something_error,
};

using namespace std;

string getline_from_stdin() noexcept(false) {
  // 標準入力から1行読んで返す
  string s;
  if (getline(cin, s))
    return s;
  else
    throw localerror::read_error;
}

vector<int> input_numbers(int n) noexcept(false) {
  // 整数値をn回入力した結果を返す関数
  vector<int> v;
  v.reserve(n);
  for (int i = 0; i < n; ++i) {
    v.push_back(stoi(getline_from_stdin()));
  }
  // n回分の読み込んだint値を返す
  return v;
}

int sum(const vector<int>& v) noexcept {
  // int値ベクタを合計して返す
  int sum = 0;
  for (auto e : v) sum += e;
  return sum;
}

int main() {
  setlocale(LC_ALL, "");
  try {
    auto r = sum(input_numbers(3));
    wcout << L"合計: " << r << endl;
  } catch (localerror e) {
    wcout << wstring(L"エラー: ") + (e == localerror::read_error
                                         ? L"読み込みエラー!"
                                         : L"不明エラー")
          << endl;
  } catch (invalid_argument) {
    wcout << (L"エラー: "
              L"不正な引数!")
          << endl;
  } catch (out_of_range) {
    wcout << (L"エラー: "
              L"範囲外!")
          << endl;
  } catch (exception) {
    wcout << (L"エラー: "
              L"不明エラー")
          << endl;
  }
}