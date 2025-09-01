# 文法规范

# 1. 顶层文法结构

```
program         → external_decl_list
    // 整个程序入口，由一系列外部声明/定义组成

external_decl_list
                → external_decl external_decl_list
                | ε
    // 外部声明/定义的列表，可以有多个，也可以为空

external_decl   → function_def
                | function_decl
                | var_decl
    // 外部声明：包括函数定义、函数声明、变量声明
```

---

# 2. 变量声明与说明

```
var_decl        → type_spec IDENT SEMI
                | type_spec IDENT ASSIGN expr SEMI
    // 变量声明：类型+变量名+分号，或类型+变量名+赋值+分号
    // 例如 int a;   或   int a = 5;
```

---

# 3. 函数声明与定义

```
function_decl   → type_spec IDENT LP param_list RP SEMI
    // 函数声明：类型+函数名+参数列表+分号（只声明不实现）
    // 例如 int foo(int x);

function_def    → type_spec IDENT LP param_list RP compound_stmt
    // 函数定义：类型+函数名+参数列表+函数体（实现）
    // 例如 int foo(int x) { ... }

param_list      → param param_list_tail
                | ε
    // 参数列表：可以有参数，也可以没有

param_list_tail → COMMA param param_list_tail
                | ε
    // 参数列表的后续部分：逗号分隔的多个参数

param           → type_spec IDENT
    // 单个参数：类型+参数名
    // 例如 int x
```

---

# 4. 复合语句与语句列表

```
compound_stmt   → LBRACE stmt_list RBRACE
    // 复合语句（代码块）：大括号包裹的语句列表
    // 例如 { stmt1; stmt2; }

stmt_list       → stmt stmt_list
                | ε
    // 语句列表：可以有多个语句，也可以没有
```

---

# 5. 语句类型

```
stmt            → expr_stmt
                | if_stmt
                | while_stmt
                | for_stmt
                | return_stmt
                | break_stmt
                | continue_stmt
                | compound_stmt
                | var_decl
    // 支持的语句类型：
    // 1. 表达式语句
    // 2. if语句
    // 3. while循环
    // 4. for循环
    // 5. return语句
    // 6. break语句
    // 7. continue语句
    // 8. 复合语句（代码块）
    // 9. 局部变量声明
```

---

# 6. 各类语句详细规则

## 6.1 表达式语句

```
expr_stmt       → expr SEMI
                | SEMI
    // 表达式语句，以分号结尾的表达式或单独一个分号（空语句）
    // 例如 a = 5;   或   ;
```

## 6.2 if语句（含嵌套）

```
if_stmt         → IF LP expr RP stmt
                | IF LP expr RP stmt ELSE stmt
    // if语句和if-else语句，条件表达式后跟语句体
    // 例如 if (a > 0) x = 1;
    //      if (a > 0) x = 1; else x = 2;
```

## 6.3 while语句（可嵌套）

```
while_stmt      → WHILE LP expr RP stmt
    // while循环，条件表达式后跟循环体
    // 例如 while (a < 10) a = a + 1;
```

## 6.4 for语句（可嵌套）

```
for_stmt        → FOR LP expr_stmt expr_stmt expr RP stmt
    // for循环，括号内为初始化、条件、递增，后跟循环体
    // 例如 for (i = 0; i < 10; i = i + 1) { ... }
```

## 6.5 return语句

```
return_stmt     → RETURN expr SEMI
                | RETURN SEMI
    // return语句，可以有返回值也可以没有
    // 例如 return a;   或   return;
```

## 6.6 break/continue语句

```
break_stmt      → BREAK SEMI
    // break语句，跳出循环
continue_stmt   → CONTINUE SEMI
    // continue语句，跳过本次循环剩余部分
```

---

# 7. 表达式（支持所有运算符）

```
expr            → assign_expr
    // 表达式的入口

assign_expr     → logical_or_expr
                | IDENT ASSIGN assign_expr
    // 赋值表达式：变量赋值，或逻辑或表达式

logical_or_expr → logical_and_expr { OR logical_and_expr }
    // 逻辑或：a || b || c

logical_and_expr→ equality_expr { AND equality_expr }
    // 逻辑与：a && b && c

equality_expr   → relational_expr { (EQ | NEQ) relational_expr }
    // 相等/不等表达式：a == b, a != b

relational_expr → additive_expr { (LT | GT | LE | GE) additive_expr }
    // 关系表达式：a < b, a > b, a <= b, a >= b

additive_expr   → multiplicative_expr { (PLUS | MINUS) multiplicative_expr }
    // 加减表达式：a + b, a - b

multiplicative_expr
                → unary_expr { (MUL | DIV | MOD) unary_expr }
    // 乘除模表达式：a * b, a / b, a % b

unary_expr      → (PLUS | MINUS | NOT) unary_expr
                | postfix_expr
    // 一元运算：+a, -a, !a，或后缀表达式

postfix_expr    → primary_expr
                | IDENT LP arg_list RP
    // 后缀表达式：基本表达式，或函数调用
    // 例如 foo(a, b)

arg_list        → expr { COMMA expr }
                | ε
    // 函数调用参数列表，逗号分隔，可以为空

primary_expr    → IDENT
                | INT_CONST
                | FLOAT_CONST
                | CHAR_CONST
                | STRING_CONST
                | LP expr RP
    // 基本表达式：变量、常量、括号表达式
```

---

# 8. 类型说明符

```
type_spec       → INT
                | FLOAT
                | CHAR
                | VOID
    // 类型关键字：int、float、char、void
```