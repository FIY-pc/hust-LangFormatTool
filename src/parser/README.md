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
                | LineComment
                | BlockComment
    // 外部声明：包括函数定义、函数声明、变量声明、注释
```

---

# 2. 变量声明

```
var_decl        → type_spec IDENT array_decl_opt assign_opt SEMI

array_decl_opt  → LB array_dim RB array_decl_opt
                | ε

array_dim       → INT_CONST
                | IDENT
                | ε

assign_opt      → ASSIGN expr
                | ε
    // 变量声明支持多维数组（如 int a[10][b];），维度可为常量或标识符，也可为空（如 int a[];）
```

---

# 3. 局部变量声明

```
local_var_decl  → type_spec IDENT array_decl_opt assign_opt SEMI
    // 局部变量声明，出现在复合语句块内，语法与外部变量声明一致
```

---

# 4. 函数声明与定义

```
function_decl   → type_spec IDENT LP param_list_opt RP SEMI
    // 函数声明：类型+函数名+参数列表+分号（只声明不实现）

function_def    → type_spec IDENT LP param_list_opt RP compound_stmt
    // 函数定义：类型+函数名+参数列表+函数体（实现）

param_list_opt  → param_list
                | ε

param_list      → param param_list_tail

param_list_tail → COMMA param param_list_tail
                | ε

param           → type_spec IDENT array_decl_opt
    // 参数支持数组类型（如 int a[], float b[10]），维度可为空
```

---

# 5. 复合语句与语句列表

```
compound_stmt   → LBRACE var_decl_list_opt stmt_list_opt RBRACE
    // 复合语句块内先是局部变量声明列表，再是语句列表

var_decl_list_opt → var_decl_list
                  | ε

var_decl_list   → local_var_decl var_decl_list
                | ε

stmt_list_opt   → stmt_list
                | ε

stmt_list       → stmt stmt_list
                | ε
```

---

# 6. 语句类型

```
stmt            → expr_stmt
                | if_stmt
                | while_stmt
                | for_stmt
                | return_stmt
                | break_stmt
                | continue_stmt
                | compound_stmt
                | local_var_decl
                | LineComment
                | BlockComment
    // 支持的语句类型包括注释节点
```

---

# 7. 各类语句详细规则

## 7.1 表达式语句

```
expr_stmt       → expr SEMI
                | SEMI
    // 表达式语句，以分号结尾的表达式或单独一个分号（空语句）
```

## 7.2 if语句

```
if_stmt         → IF LP expr RP stmt
                | IF LP expr RP stmt ELSE stmt
```

## 7.3 while语句

```
while_stmt      → WHILE LP expr RP stmt
```

## 7.4 for语句

```
for_stmt        → FOR LP expr_stmt expr_stmt expr RP stmt
```

## 7.5 return语句

```
return_stmt     → RETURN expr SEMI
                | RETURN SEMI
```

## 7.6 break/continue语句

```
break_stmt      → BREAK SEMI
continue_stmt   → CONTINUE SEMI
```

---

# 8. 表达式

```
expr            → assign_expr

assign_expr     → logical_or_expr
                | IDENT ASSIGN assign_expr

logical_or_expr → logical_and_expr { OR logical_and_expr }

logical_and_expr→ equality_expr { AND equality_expr }

equality_expr   → relational_expr { (EQ | NEQ) relational_expr }

relational_expr → additive_expr { (LT | GT | LE | GE) additive_expr }

additive_expr   → multiplicative_expr { (PLUS | MINUS) multiplicative_expr }

multiplicative_expr
                → unary_expr { (MUL | DIV | MOD) unary_expr }

unary_expr      → (PLUS | MINUS | NOT) unary_expr
                | postfix_expr

postfix_expr    → primary_expr
                | IDENT LP arg_list_opt RP
                | postfix_expr LB expr RB
    // 支持数组访问（如 a[1][b]），支持函数调用（如 foo(a, b)）

arg_list_opt    → arg_list
                | ε

arg_list        → expr { COMMA expr }
                | ε

primary_expr    → IDENT
                | INT_CONST
                | LONG_CONST
                | FLOAT_CONST
                | CHAR_CONST
                | STRING_CONST
                | LP expr RP
```

---

# 9. 类型说明符

```
type_spec       → INT
                | FLOAT
                | CHAR
                | VOID
```

---

# 10. 注释

```
LineComment     → // 注释内容
BlockComment    → /* 注释内容 */
    // 注释可作为顶层节点或语句节点
```
