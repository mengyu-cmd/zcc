# zcc
> **zcc**是一个用C++写的C99的编译器.
<br><br>

## 一 、当前效果
        C语言例子：`simple/hello.c`<br>
        中间代码输出:`build/hello.qd`
        AT&T汇编输出:`build/hello.s`

## 二、 支持语法
### 2.1 数据类型
        字符串常量、int、short、char、long 
### 2.2 运算
```
        + - += -= <br>
        < > >= <= == !=<br>
        || &&<br>
```
### 2.3 支持函数调用
### 2.4 支持参数传递
### 2.5 控制流
```
        支持 if...else..
        支持 while
        支持 do..while..
        支持 for
        支持 goto
        支持 switch case
        支持 break
        支持 continue
        支持 default
```

## 三、 实现方法

### 3.1 预处理器
未实现
<br><br>

### 3.2 词法分析器
        正则语言，自动机;<br>
        还需要完成确定符号在文件中的位置
<br><br>

### 3.3 语法分析器
        大部分完成，上下文无关文法，递归子程序
<br><br>

### 3.4 AST
        部分完成
<br><br>

### 3.5 符号表
        大部分完成
<br><br>

### 3.6 中间代码生成
```
        + - * / % & | ^ && || 
        > < >= <= == != <<= >>=
        += *= /= %= -= |= ^= &= 
        if..else..
        while
        do..while
        goto label
 ```
<br><br>

### 3.7 生成汇编
        部分完成

