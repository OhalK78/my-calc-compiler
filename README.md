# 計算機言語構成論 問題4.2

講義ページ→ https://ie.u-ryukyu.ac.jp/~kono/lecture/compiler/c4/lecture.html

問題4.2

>s-calc.c の若干の改良を試みる。あとでコンパイラに書き換えることを前提にいくつかの機能をつけ加えて見よう。
termとして8進数や16進数 (楽勝)
-1 や -a (楽勝)
AND(&)やOR(|)の計算 (楽勝)
<<,>> などの算術シフト (楽勝)
Cの三項演算子a?b:c (コンパイルは難しい)
制御構造 while, for 文など (難しい)
配列 (宣言を別にするのが簡単だろう)
手続き呼び出し (入力テキストをすべて取っておく必要がある)
浮動小数点 (全体的な改良が必要になる。すべの数を小数点で計算するのが楽だろう)
楽勝と書いてある部分は必須とする。残りのどれか、または、自分で考えた機能を実装し、my-calc.cを作って見よ。
実行結果と、変更した主要な部分を明示せよ。

---

# 追加した機能
* 必須
    * termとして8進数や16進数 
    * -1 や -a 
    * AND(&)やOR(|)の計算 
    * <<,>> などの算術シフト 
* その他 
    * ==や!=の演算
    * Cの三項演算子a?b:c

# 実行結果

input.txtを入力すると、次の結果が得られた。
```
% ./a.out < input.txt
1
 = 0x00000001 = 1
1+2			//  expr
 = 0x00000003 = 3
2*3			//  mexpr
 = 0x00000006 = 6
1+(2*3)			// term
 = 0x00000007 = 7
1+2*3			// expr order
 = 0x00000007 = 7
-1+0x10*010		// token
 = 0x0000007f = 127
(0xff&07)|0x100		// logical expression
 = 0x00000107 = 263
100/10
 = 0x0000000a = 10
a=1*3
 = 0x00000003 = 3
b=2*3
 = 0x00000006 = 6
a+b
 = 0x00000009 = 9
a=(b=3<<2)
 = 0x0000000c = 12
a>>1
 = 0x00000006 = 6
b
 = 0x0000000c = 12
1==1
 = 0x00000001 = 1
1==0x10
 = 0x00000000 = 0
b==a
 = 0x00000001 = 1
b==a?-a:b+1
 = 0xfffffff4 = -12
b!=a?-a:b+1
 = 0x0000000d = 13
!(b+1>a)?-a:b+1
 = 0x0000000d = 13
!(b+1<a)?-a:b+1
 = 0xfffffff4 = -12
```

# 変更した主要な部分

## termとして8進数や16進数
s-token.cのtoken()にて、8進数/16進数を入力できるようにした。
8進数入力は```0```で始まり、16進数入力は```0x```で始まるため、その入力を受けた際に、その通りに入力が可能なように修正を行う。

よって、s-token.cのtoken関数にて、数値を受け取る部分を以下のように書き換えた。
```
if('0'<=c && c<='9') {     /* Decimal or Octal or Hexadecimal*/
    if (c=='0' && *ptr){            /* Octal or Hexadecimal */
        c = *ptr;
        ptr++;
        if('0'<=c && c<='7'){                 /* Octal */
            d = 00 + c-'0';
            while((c= *ptr++)){
                if('0'<=c && c<='7'){
                    d = d*8 + (c-'0');
                }else{
                    break;
                }
            }
        }else if(c=='x'){           /* Hexadecimal */
            d = 0x0;
            while((c= *ptr++)){
                if('0'<=c && c<='9'){
                    d = d*16 + (c-'0');
                }else if('a'<=c && c<='f'){
                    d = d*16 + (c-'a');
                }else{
                    break;
                }
            }
        }
    }else{                  /* Decimal */
        d = c-'0';
        while((c= *ptr++)) {
            if('0'<=c && c<='9') {
                d = d*10 + (c - '0');
            } else {
                break;
            }
        }
    }
    if (c!=0) ptr--;
    value = d;
    last_token = '0';
    return last_token;
```

## -1 や -a 
-から始まるtermをtoken()した時に、それが-termであると読み込むことができれば良い。

よって、my-calc.cのterm関数における```switch(last_token)```の条件分岐で、次のcaseを追加した。
```
case '-':		/* -1や-a */
	d = -1*term();
	return d;
```

## AND(&)やOR(|)の計算 
既に実装されている*や+を参考に実装を行った。
AND演算は、my-calc.cのmexpr関数の条件分岐にて、次のcaseを追加した。
```
case '&':
	d &= term();
	break;
```

また、OR演算はmy-calc.cのaexpr関数の条件分岐にて、次のcaseを追加した。
```
case '|':
	d |= mexpr();
	break;
```

## <<,>> などの算術シフト 
<<や>>は、<や>と同じ記号であるので、それを考慮してtoken関数とmy-calc.cを変更した。

具体的には、'<'をtokenした時、次の文字を先読みし、'<'が続いていれば左シフト演算"<<"が入力されているため、'l'をlast_tokenに、別の文字であれば'<'だけなので、'<'をlast_tokenとして返すようにした。'>'も同じような処理で、">>"なら'r'、'>'なら'>'を返すようにした。
よってtoken関数には次の条件分岐を追加した。

```
} else if (c=='<'){    /* << or < */
        last_token = c;
        if(c== *ptr){       // < の後に<が来る→ <<が入力されている
            last_token = 'l';
            ptr++;
        }
        return last_token;
    } else if (c=='>'){    /* >> or > */
        last_token = c;
        if(c== *ptr){       // > の後に>が来る→ >>が入力されている
            last_token = 'r';
            ptr++;
        }
        return last_token;
}
```

また、my-calc.cのexpr関数にて、last_tokenが'l'(<<)や'r'(>>)の場合の処理と、未実装であった'<'の処理を追加した。
```
case '<':
	d = (d < aexpr());
	break;
case 'r':				// right shift (>>)
	d = (d >> aexpr());
	break;
case 'l':				// left shift (<<)
	d = (d << aexpr());
	break;
```

## ==や!=、!(term)の演算
==や!=も、=や!との処理を区別するため、次の文字を先読みして処理を変える必要があった。

よって、token関数に次の条件分岐を追加した。
```
}else if (c=='=') {
        last_token = c;
        if(c == *ptr){       // = の後に=が来る→ ==が入力されている
            last_token = 'e';
            ptr++; 
        }
        return last_token;
} else if (c=='!') {
        last_token = c;
        if(*ptr == '='){       // ! の後に=が来る→ !=が入力されている
            last_token = 'n';
            ptr++; 
        }
        return last_token;
}
```

ここで、last_tokenとして、```"=="```は'e'、```"!="```は'n'としている。
```"=="```や```"!="```の演算は、expr関数に次のcaseを追加した。
```
case 'e':				// == operation
	d = (d == aexpr());
	break;
case 'n':				// != operation
	d = (d != aexpr());
	break;
```

また、!(term)の演算は、term関数に次のcaseを追加した。
```
case '!':
	d = !(term());
	return d;
```

## Cの三項演算子a?b:c
三項演算子```a?b:c```は、aがtrueであればb、falseであればcを返す演算である。よって、'?'がlast_tokenで得られた時に、trueであればb、falseであればcをdに格納するように処理を追加する。この時、':'でbとcが区切られているので、':'を読み込んだ時に、それまでで得られている値をreturnするようにする。

以上より、expr関数に次のcaseを追加した。
```
case '?':				// ternary operator 
	if(d){
		d = expr();
		expr();
		return d;
	}else{
		expr();
		d = expr();
		return d;
	}
case ':':
	return d;
default:
```
 