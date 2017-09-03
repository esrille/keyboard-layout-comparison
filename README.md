# 日本語のキーボードレイアウトの比較ツール

## 打鍵効率の目安

https://esrille.github.io/keyboard-layout-comparison/

いくつかの日本語入力方式の特徴を、実際に例文を入力してしらべられます。

## 打鍵時間の測定

 https://esrille.github.io/keyboard-layout-comparison/keytime.html

2つのキーをつづけておすのに要する時間を計測できます。

次のページはノートPCをつかって実際に計測をおこなった例です。

https://esrille.github.io/keyboard-layout-comparison/keytime.notepc.html

html2txt.pyをつかうと、htmlファイルからプログラムで処理しやすいテキスト形式のファイルを生成できます。

つかいかたの例:
```
$ ./html2txt.py keytime.notepc.html > keytime.notepc.txt
```

## かな配列の入力時間概算ツール

打鍵時間の測定結果をもとに、あたえられた、ひらがながきのテキストを入力するのに要する時間を概算することができます。

例:

```
$ ./toqwerty.py new-stickney.json mekuraniji.txt | ./keyingtime keytime.notepc.txt
```

toqwerty.pyは、ひらがながきのテキストをUS QWERTYキーボードでうったときのキーのアルファベットを出力します。

日本語のキーボードレイアウトは、new-stickney.json のようにJSONで定義したものを指定します:

```
{
    "normal": "けくすさせつにのをわ□たかしはてらうい゛な□ちきことよっん、。ね",
    "shift": "・ひふへ□□みもおえ□゜そやゆほれるりあま□□ゐ□ゑ□ろーぬむめ",
    "daku": "がぎぐげござじずぜぞだぢづでどばびぶべぼ",
    "handaku": "ぱぴぷぺぽ",
    "kogaki": "ぁぃぅぇぉゃゅょ"
}
```

normal と shift には、下記のキーにわりあてるかな文字、計32文字を1つの文字列で指定します。わりあてのないキーは□としておきます:
```
QWERTYUIOP[
ASDFGHJKL:'
ZXCVBNM,./
```

daku, handaku, kogakiは、それぞれ、゛, ゜, ゛キーとくみあわせて入力する文字を指定します。新JIS配列の小がきのように、直接キーボードから入力できる場合は、空文字列 "" を指定します。


keyingtime は、c++14のプログラムです。g++が利用できる環境であれば、

```
$ make
g++ -O3 -std=c++14 -Wall    keyingtime.cpp   -o keyingtime
```

でビルドできます。

打鍵時間の測定ページでは、最終的には、2つのキーをおせる状態にあらかじめ指を配置したあとで、どれだけ2キーをはやくうてるか、というデータがえられます。そこで、keyingtimeでは、各指ごとに再配置にかかる時間と、2つのキーをおす時間をつみかさねて概算するようにしてみています。
