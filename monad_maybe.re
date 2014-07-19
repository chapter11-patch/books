= 第一歩: Maybeモナド

//lead{
"I call it my billion-dollar mistake. It was the invention of the null reference in 1965." Charles Antony Richard Hoare
//}

== 値が存在しない可能性を型で表す

値が存在するかしないかを型で表してみましょう。

//emlist{
type Maybe<'T> = Just of 'T | Nothing
//}

@<code>{Maybe} 型は、任意の値 を持つ @<code>{Just} と、値を持たない @<code>{Nothing} という2つのケース識別子から成ります。

//emlist{
let db = [ ("x", 1); ("y", 2); ("z", 3) ]
//}

ここで、 @<code>{db} から指定したキーに対応する値を取得する操作について考えてみましょう。
指定したキーが @<code>{db} に登録されていないときは、返す値がありません。
そこで、 @<code>{Maybe} 型の出番です。

//emlist{
let rec tryFind key db =
  match db with
  | [] -> Nothing
  | (k, v) :: xs when k = key -> Just v
  | _ :: xs -> tryFind key xs
//}

指定したキーに一致する値が見つからない際に @<code>{Nothing} を返すことで、値が存在しないことを伝えられるようになりました。

//emlist{
tryFind "y" db // equal (Just 2)
tryFind "w" db // equal Nothing
//}

@<code>{db} から二つの値を取得してその合計を返す関数を定義してみましょう。

//emlist{
let sum a b db =
  match tryFind a db with
  | Just x ->
    match tryFind b db with
    | Just y -> Just (x + y)
    | Nothing -> Nothing
  | Nothing -> Nothing

sum "x" "z" db // equal (Just 4)
sum "x" "w" db // equal Nothing
sum "v" "y" db // equal Nothing
//}

パターンマッチを使うことで、安全に値を取得して計算できるようになりました。

しかし、不満な点もあります。
パターンマッチがネストしてしまう点と、 @<code>{Nothing} にマッチした際に @<code>{Nothing} を返す、という定型的なコードが増えてしまう点です。

== 理想1: 定型的なコードを減らす

定型的なコードは、関数として解決できそうです。
ここで共通する操作は、 @<code>{Just} なら何かしらの計算を適用して @<code>{Maybe} 型を返し、 @<code>{Nothing} ならば @<code>{Nothing} を返す部分です。
今回は、演算子として抽出します。

//emlist{
// Maybe<'T> -> ('T -> Maybe<'U>) -> Maybe<'U>)
let (>>=) x f =
  match x with
  | Just v -> f v
  | Nothing -> Nothing
//

@<code>{>>=} 演算子を用いて、 @<code>{sum} 関数を書き換えてみましょう。

//emlist{
let sum a b db =
  tryFind a db >>= fun x ->
  tryFind b db >>= fun y ->
  Just (x + y)
//}

不満点が解消されました。

== 理想2: 手続き的な書式で記述する

@<code>{let} と同等のキーワードが利用できれば、もっとわかりやすくなることでしょう。
F# では、コンピュテーション式でこれを実現します。

//emlist{
// コンピュテーション式用のビルダークラス
type MaybeBuilder () =
  member this.Return(x) = Just x
  member this.ReturnFrom(x: Maube<_>) = x
  member this.Bind(x, f) = x >>= f

// ビルダーインスタンス
let maybe = MaybeBuilder()
//}

@<code>{>>=} に対応する @<code>{Bind} メソッドによって @<code>{let!} 、値を @<code>{Just} で包んで返す @<code>{Return} メソッドによって @<code>{return} 、 @<code>{Maybe} 型の値をそのまま返す @<code>{ReturnFrom} メソッドによって @<code>{return!} キーワードがコンピュテーション式内で利用可能になります。

準備が整ったので、 @<code>{sum} 関数を書き換えてみましょう。

//emlist{
let sum a b db =
  maybe {
    let! x = tryFind a db
    let! y = tryFind b db
    return x + y
  }
//}

コンピュテーション式の導入によって、ネストを平坦化することができました。

== 標準にあります

ここまで、 @<code>{Maybe} 型を独自に定義して Maybe モナドについて説明してきました。
しかし、この型を毎回定義するのは面倒くさいですね。

幸いなことに、 F# には @<code>{Maybe} 型と全く同じ動作をする @<code>{Option} 型が標準搭載されています。
@<code>{>>=} 演算子についても、 @<code>{bind} という関数名で定義されています。

というわけで、独自定義していた関数を組み込み関数で置き換えましょう。

//emlist{
let tryFind key db =
  List.tryFind (fst >> ((=) key)) db
  |> Option.map snd

let (>>=) x f = Option.bind f x

type OptionBuilder() =
  member this.Return(x) = Some x
  member this.ReturnFrom(x) = x
  member this.Bind(x, f) = x >>= f

let option = OptionBuilder()

let sum a b db =
  option {
    let! x = tryFind a db
    let! y = tryFind b db
    return x + y
  }

sum "x" "z" db // equal (Some 4)
sum "x" "w" db // equal None
sum "v" "y" db // equal None
//}

## 例題: ピエールの綱渡り

例題として、"すごい Haskell 楽しく学ぼう"に登場する例題"ピエールの綱渡り"を実装していきましょう。
以下に仕様を示します。

 1. ピエールが持つバランス棒の左右には鳥がとまることがある
 2. バランス棒の左右にとまっている鳥の差が 3 以下ならば、ピエールはバランスがとれる
 3. 差が 4 以上ならば、ピエールは安全ネットに落っこちる

まず、バランス棒にとまっている鳥の数を型で表しましょう。

//emlist{
type Birds = int
type Pole = Birds * Birds
//}

2-タプルの 第一要素が左側にとまっている鳥の数、第二要素が右側にとまっている鳥の数を表します。

次に、鳥の数をとって、バランス棒の左右に鳥をとまらせる関数を作ります。

//emlist{
// Birds -> Pole -> Pole
let landLeft n (left, right) =
  if abs ((left + n) - right) < 4 then Some (left + n, right)
  else None

// Birds -> Pole -> Pole
let landRight n (left, right) =
  if abs (left - (right + n)) < 4 then Some (left, right + n)
  else None
//}

鳥が飛び去る操作は、鳥の数に負数を渡すことで表現します。

それでは、ピエールに綱渡りさせてみましょう。

//emlist{
Some (0, 0) >>= landRight 2 >>= landLeft 2 >>= landRight 2 // equal (Some (2, 4))
//}

もう一回渡ってもらいましょう。

//emlist{
Some (0, 0) >>= landLeft 1 >>= landRight 4 >>= landLeft (-1) // equal None
//}

鳥が右側に集中してしまったことで、ピエールが落ちてしまいました。
落ちた状態で右側の鳥を減らしてみたらどうなるでしょう?

//emlist{
Some (0, 0) >>= landLeft 1 >>= landRight 4 >>= landLeft (-1) >>= landRight (-2) // equal None
//}

ピエールが落下してしまっていることが、後続の操作にも伝播していることがわかります。

最後に、コンピュテーション式を使って綱渡りをシミュレーションしてみましょう。

//emlist{
// routine equal (Some (3, 2))
let routine =
  option {
    let! start = Some (0, 0)
    let! first = landLeft 2 start
    let! second = landRight 2 first
    return! landLeft 1 second
  }
//}

