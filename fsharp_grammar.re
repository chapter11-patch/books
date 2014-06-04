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

演算結果を束縛してみましょう。

//emlist{
let x : int = (1 + 2) * 3 // x equal 9
//}

F#では型推論@<fn>{tapl}という機能により、型の記述を省略できます。
本書では理解のための補助として型を明示する時以外は、型の記述を省略します。

//footnote[tapl][オーム社出版 "型システム入門 - プログラミング言語と型の理論"にて詳しく解説されています。]

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

===[column] let の語源
@<kw>{let} というキーワードはあまり聞きなれない方もいるかもしれませんが、
これは数学から来た表記です。
例えば、

//emlist{
let x = 10, ...
//}

とあると、「x が 10 だとすると、...」という意味になります。
変数の定義には @<kw>{let} を使うんだ、と覚えてしまってもいいですが、
語源を知っていた方が覚えやすいと思うので、頭の片隅にでも置いておくといいでしょう。

===[column] 2 つの let と軽量構文
@<kw>{let} により変数が定義できることを本文で説明しましたが、
実は変数の定義は書く場所によって 2 つに分類できます。

 1. モジュールの中などに記述した場合の @<kw>{let} による@<strong>{値定義}
 2. 定義の中や関数内などに記述した場合の @<strong>{let 式}

一つ目の @<kw>{let} による値定義は、本文で説明した以下のような構文です。

//emlist{
let 識別子 : 型名 = 式
//}

それに対して、二つ目の let 式は本来、以下のような構文です。

//emlist{
let 識別子 : 型名 = 式1 in 式2
//}

このように、@<kw>{in} に続けて式を記述するのが本来の let 式です。
この構文は、式2 の部分で識別子 (変数名) が現れた場合にそれを式1 の値に置き換えることを意味しています。

本文で紹介した @<kw>{let} を入れ子にしたコードは、z が値定義だとすると本来の構文を使うと以下のように書きます。

//emlist{
let z =
  let x = 1 in
  let y = 2 * 3 in
  x + y - 4
//}

F# は @<kw>{軽量構文} という構文をサポートしており、let 式の @<kw>{in} を改行とインデントで代用することができます。
これによって、上のコードは本文中に載せたような書き方ができるようになり、
@<kw>{let} による値定義も let 式も同じ表記で書けることになります。
そのため、「変数を定義する場合は @<kw>{let} を使う」と考えてもらってほとんど問題はありません。

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

F#では関数も値です。
そのため、関数をパラメータとして渡したり、戻り値として扱えます。

関数の型は @<kw>{->} で表せます。 @<kw>{->} を使うと、パラメータに関数の型を明示できます。

//emlist{
f : パラメータ1の型 -> パラメータ2の型 ... -> 戻り値の型
//}

例として、整数値を受け取り整数値を返す f と、整数値 x、 y を受け取って計算結果を返す関数を定義してみましょう。

//emlist{
// f に加算結果を適用して返す関数
let addF (f : int -> int) x y = f (x + y)
//}

addF 関数に、別に定義した関数を適用します。

//emlist{
let square x = x * x
addF square 1 2 // equal 9
//}

関数をパラメータとして受け取ったり、関数を返す関数は高階関数と呼ばれます。

その場限りしか使用しない関数値を渡したい場合は、ラムダ式を使います。
ラムダ式は、 @<kw>{fun} と @<kw>{->} で表現します。

//emlist{
fun 一つ以上のパラメータ -> 式
//}

addF にラムダ式を適用してみましょう。

//emlist{
addF (fun x -> x + 3) 1 2 // equal 6
//}

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

同一の型を持つ値の集まりを表現する型をコレクションと呼びます。
コレクションのうち、F#では単方向連結リストのことをリストと呼びます。

空のリストは、角括弧を使って表現します。

//emlist{
[] // 空のリスト
//}

要素を一つ持たせたい場合は、角括弧の間に要素の初期化式を記述します。

//emlist{
[ 要素の初期化式 ]
//}

要素を二つ以上持たせたい場合は、セミコロン区切りで要素を指定します。

//emlist{
[ 要素の初期化式1; ... ]
//}

それでは実際に、リストを定義してみましょう。

//emlist{
[ 1 + 2 ] // equal [ 3 ]
[ 3; 4; 5 - 6 ] // equal [ 3; 4; -1 ]
//}

要素ごとに改行してインデントを揃えた場合、セミコロンは省略できます。

//emlist{
// [ 1; 1 + 2 ] と同じ結果になる
[ 1
  1 + 2 ]
//}

本書では、要素の初期化式が長くなる場合に改行スタイルを利用します。

リストの各要素は同じ型である必要があります。
例えば、整数と文字列は同じリストの要素として構成できません。

//emlist{
// コンパイルエラー
[ 1; "文字" ]
//}

F# のリストでは、要素の値は変更できません。
常に新たなリストを生成することで、追加や削除、更新を表現します。

リストの先頭に新しい要素を追加するには、 @<code>{::} 演算子を用います。
この場合も、元のリストは変更されず、新しいリストが生成されます。

//emlist{
1 :: [ 2 ] // equal [ 1; 2 ]
//}

[1; 2; 3] は、 1 :: 2 :: 3 :: [] の略記です。

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

2つ以上の任意の型をグループ化したいが、名前をつけるまでには至らない場合は、タプルを使用します。
タプルは、丸括弧とカンマを使って表現します。

//emlist{
(element1, elemnt2, ...)
//}

各要素の型は異なる場合もあります。

例として、第一要素にアイテム名、第二要素に個数を持つタプルを作ってみましょう。

//emlist{
// アイテム名が"薬草"で、個数は2
let item = ("薬草", 2)
//}

単にタプルと呼ぶと何要素なのか判別しづらいため、要素数が n のタプルを n-タプル と呼びます。
item は要素数が 2 なので、2-タプルです。

タプルの型を明示したい場合は、アスタリスクを使用します。

//emlist{
let item : string * int = ("薬草", 2)
//}

2-タプルの場合は、標準で提供されている fst 、 snd 関数でそれぞれ第一要素、第二要素を取得できます。

//emlist{
fst item // equal "薬草"
snd item // equal 2
//}

3-タプルや4-タプルなど、2-タプル以外のタプルでは fst 関数や snd 関数は使えないので注意してください。
fst 関数や snd 関数を使わないタプルの要素の取り出し方もありますが、それについてはパターンマッチで説明します。

== 判別共用体

"いくつかの状態の中の一つ"を型で表したい場合、判別共用体を使います。

//emlist{
type 型名 =
  | ケース識別子1
  | ケース識別子2
  ...
//}

ある状態をケース識別子で表し、ケース識別子を一つ以上定義することで型を構成します。

次のコードは、判別共用体を用いて順序付けを表したものです。

//emlist{
type Order =
  | LessThan
  | EqualTo
  | GreaterThan
//}

判別共用体の定義は、一行にまとめることもできます。

//emlist{
type Order = | LessThan | EqualTo | GreaterThan
//}

また、先頭のバーティカルバーは省略可能です。

//emlist{
type Order = LessThan | EqualTo | GreaterThan
//}

本書では、一行で記述するか複数行にわけて記述するかは、型定義の見易さに応じて使い分けることにします。
先頭のバーティカルバーの省略は、判別共用体を一行で記述する時のみ使用します。

ケース識別子を指定することで、判別共用体の値を返すことができます。

//emlist{
// 大小比較
let compare x y =
  if x < y then LessThan
  else
    if x = y then EqualTo
    else GreaterThan
//}

各ケース識別子には、一つ以上のフィールドを関連付けられます。
フィールド定義には、 @<kw>{of} キーワードを使います。

//emlist{
type 型名 =
  | ケース識別子 of 型
  ...
//}

フィールドを複数持たせたい場合は、アスタリスクで型を区切ります。
アスタリスクで区切ることからタプル型を連想するかもしれませんが、タプルそのものではありません。

//emlist{
type 型名 =
  | ケース識別子 of 型1 * 型2 * ...
//}

フィールドを持つ例として、 特定の図形を表す Shape 型を定義してみましょう。

//emlist{
type Shape =
  | Circle of float
  | Rectangle of float * float
//}

F# 3.1 からは、各フィールドに名前をつけられるようになりました。
フィールド名をつける場合には、コロンを用います。

//emlist{
type Shape =
  | Circle of radius : float
  | Rectangle of width : float * length : float
//}

実際に円や長方形を生成してみましょう。

//emlist{
Circle 2.0 // 半径2の円
Rectangle (3.0, 4.0) // 幅3、長さ4の長方形
//}

ケース識別子によって計算を分岐する方法や、フィールドの値を取り出す方法については、パターンマッチで説明します。

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

