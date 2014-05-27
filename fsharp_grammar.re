= F# の文法

//lead{
本章では、本書を読むために必要な F# の文法について、簡単に解説を行います。
//}

== 簡単な演算とコメント

まずは、簡単な演算から行ってみましょう。

//emlist{
(1 + 2) * 3 // equal 9
//}

スラッシュを続けて二つ書いた場合は、それ以降が行コメントとして解釈されます。

別の例を試してみましょう。

//emlist{
2 * 3.0 // コンパイルエラー!!
//}

F# は強い静的型付き言語なので、上記コードはコンパイルエラーになります。
このコードを動くように修正したい場合、どちらかの値を明示的に型変換して型を合わせる必要があります。
今回は、整数型に合わせることにしましょう。

//emlist{
2 * (int 3.0) // equal 6
//}

今度は実行できました。

== 値束縛

様々な計算を行うにあたり、計算結果をどこかに記憶しておけると便利ですね。
F# では、 @<kw>{let} キーワードを使って値と識別子を関連付けることができます。
これを、値束縛と呼びます。

//emlist{
let 識別子 : 型名 = 式
//}

型推論@<fn>{tapl}という機能により、型の記述を省略できます。
本書では理解のための補助として型を明示する時以外は、型の記述を省略します。

//footnote[tapl][オーム社出版 "型システム入門 - プログラミング言語と型の理論"にて詳しく解説されています。]

では、演算結果を束縛してみましょう。

//emlist{
let x : int = (1 + 2) * 3 // x equal 9
//}

型を省略する場合は、次のようになります。

//emlist{
let x = (1 + 2) * 3
//}

識別子と式を別々の行に分けることもできます。

//emlist{
let y =
  1 + 2 * 3 - 4
//}

束縛した値が他の束縛にも利用できることを確認しましょう。

//emlist{
let x = 1
let y = 2 * 3
let z = x + y - 4
//}

また、束縛は入れ子にすることもできます。

//emlist{
let z =
  let x = 1
  let y = 2 * 3
  x + y - 4
//}

F# ではインデントが意味をもつため、各行のインデントは揃えてください。
内部で定義した x と y には、z の式以外からアクセスすることはできません。

単純に識別子、と記述すると混乱してしまう可能性があるため、以降では束縛により値と関連付けられた識別子を変数と呼ぶことにします。

== if 式

F# で条件による分岐を行う方法の一つに、if 式があります。

//emlist{
if 条件式 then 条件式がtrueの場合の式
else 条件式がfalseの場合の式
//}

then に続く式と else に続く式は、同じ型である必要があります。
具体的な例で確認してみましょう。

//emlist{
if true then 1 else 0
//}

この式は問題なくコンパイルできます。
しかし、次の式は 1 の型が int であり、 2.3 の型が float であるため、コンパイルエラーになります。

//emlist{
// コンパイルエラー
if false then 1 else 2.3
//}


条件式は、 bool 型の値である true もしくは false を表す式である必要があります。
比較演算子である = (等値)、> (より大きい)、 < (未満)、 >= (以上)、 <= (以下)、 <> (等しくない)などが条件式として使えます。

//emlist{
if x < 0 then "not natural number" else "natural number"
//}

== 関数 (函数)

"識別子は、値が確定していないパラメータ x と関連付けて定義できる"と仮定すると、x が確定した時に x に依存する値や式も結果が確定します。
この"パラメータと関連付けられた値や式"を関数 (函数) と呼びます。
F# では関数という表記が一般的なため、以降は関数に表記を統一します。

F# での関数定義には、変数でも用いた @<kw>{let} キーワードを使います。

//emlist{
let 識別子 1つ以上のパラメータ : 戻り値型 = 式
//}

変数と同様に、戻り値型は式から推論可能なことが多いため、本書では必要と判断した場合を除き、戻り値型を省略します。

実際に関数を定義してみましょう。

//emlist{
let add x y : int = x + y
//}

パラメータは空白で区切ることで複数定義できます。
この定義から戻り値型を省略すると、次の形になります。

//emlist{
let add x y = x + y
//}

パラメータにも明示的な型の注釈を加えられます。

//emlist{
let add (x : int) (y : int) = x + y
//}

パラメータの型は、型推論可能な場合と型を明示する必要がある場合があります。
しかし、本書で扱う型は基本的に型推論可能なので、型を明示する必要がある場合についての説明は割愛します。

関数を呼び出すには、関数名の後に適用する各パラメータを空白区切りで指定します。
@<hd>{簡単な演算とコメント} で、コンパイルエラーを避けるために int による型変換を行いましたが、これは関数呼び出しの一例です。

//emlist{
int 3.0 // 3.0 をパラメータとして int 関数を呼び出す
//}

コメント部分に記述した説明は、"int 関数に3を適用する"とも表現できます。
本書では関数適用と関数呼び出しを同じ意図で使用していますが、以降は関数適用に用語を統一して説明を行います。

関数の他に、演算子も定義できます。
演算子を定義する場合は、@<kw>{let} キーワードを用いる際に演算子を丸括弧で囲みます。

//emlist{
// Haskell形式の不一致の比較演算子を定義
let (/=) x y = x <> y
//}

定義した演算子を使ってみましょう。

//emlist{
1 /= 2 // equal true
//}

また、演算子を丸括弧で囲むと、値の演算子への適用を関数適用と同様の順序で記述できます。

//emlist{
(=) 1 2 // equal false
//}

== リスト・シーケンス

== レコード

何らかのデータをグループ化して定義したい場合は、レコードを使います。
レコードは、波括弧の中に識別子名と型のペアを一つ以上記述して定義します。

//emlist{
type 型名 = {
  識別子1 : 識別子1の型
  識別子2 : 識別子2の型
  ... }
//}

以降は、レコードに属する識別子をフィールドと呼ぶことにします。

例として、分数を定義してみましょう。

//emlist{
type Rational = {
  Numerator : int
  Denominator : int }
//}

定義したレコードの値を作成する時は、各フィールドに値を設定します。

//emlist{
// 1/2を表す
let x = {
  Numerator = 1
  Denominator = 2 }
//}

フィールドはセミコロンで区切ることで、同じ行に連続して記述できます。

//emlist{
let x = { Numerator = 1; Denominator = 2 }
//}

フィールドの参照にはドットを用います。

//emlist{
// 分数の加算
let add x y = {
  Numerator = x.Numerator * y.Denominator + y.Numerator * x.Denominator
  Denominator = x.Denominator * y.Denominator }
//}

パラメータ x と y の型を記述していない点に注目してください。
これは、レコードが型推論と連携できることを示しています。

レコードの一部のフィールドの値はコピーし、一部の値は変更して新たな値を作成するには、 @<kw>{with} キーワードを用います。

//emlist{
// 分数を2倍する
let twice x = { x with Numerator = x.Numerator * 2 }
//}

この例では、分母はそのままコピーし、分子は2倍して設定するレコードを新たに生成して返しています。

== タプル

== 判別共用

== クラス

=== コンストラクタ

=== メソッド

== ジェネリック


ジェネリック型@<fn>{generics}のレコードを定義したい場合は、型名の後ろの山括弧の中に型パラメータを記述します。
型パラメータは、カンマ区切りで複数記述できます。

//footnote[generics][他の言語では、多相型や総称型と呼ばれることもあります。]

//emlist{
type Pair<'T, 'U> = { Key : 'T; Value : 'U }
//}

