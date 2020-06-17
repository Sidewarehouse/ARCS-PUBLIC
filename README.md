# ARCS-PUBLIC
Advanced Robot Control System (Public Version)

コーディング規約
- 関数名・変数名はアッパーキャメルケース(パスカル)で命名すること。
- 例外的に，数式ライクにした方が見栄えが良い関数名はスネークケースも認める。
- タブインデントは ./lib/ClassBase.hh/cc, ./lib/ClassTemplate.hh/cc, ./lib/FunctionBase.hh/cc を踏襲すること。
- 一時的にコメントアウトしたコードは最終的に消去すること。
- using namespace std; は例外なく使用禁止。
- 定数値は #define でも const ではなく constexpr を使用すること。マクロは使って良い。
- グローバル変数の使用は極力回避すること。
- goto文の使用は極力回避すること。
- new/delete, shared_ptrの使用は極力回避し，unique_ptrをなるべく使用すること。
- Cの古典的ポインタの使用は極力回避し，C++の現代的「参照」をなるべく使用すること。
- コメント文は母国語の「口語的表現」でなるべく付けて，コードとの相違がないようにすること。
- プロトタイプ宣言の引数変数は省略せずに書くこと。
- Efficient C++, More Efficient C++ にできるだけ準拠すること。
- コードを書いたらCppcheck等々により静的解析をすること。
- 物理定数，物理変数はすべてSI単位系で記述すること。
