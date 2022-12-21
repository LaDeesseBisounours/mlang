# Grammar for mlang

## basics
expresions are 5 + varlol
statements are 5 + varlol;
properties, starting with a # are used to speak classic code
macros, starting with a @ are used to manipulate properties

identifier = nameid :: nameid

## general purpose grammar
### keywords 
variable declaration : let, maybe just c style Type nameid
function declaration : fn, defun 
struct and class declaration: struct, class
namespace : namespace something {} 
const mutable keyword : const mut (default const)
return keyword: return
if blocks: if, elif, else
for blocks: for, while

nameid: [_\-a-zA-Z]
colon: :
semicolon: ;
parentheses: []
brackets: {}



### operator priorities 

from highest to lowest
- namspaced id: nameids::nameids
- primary expr : literals, namspaced id, parentheses of complete expression
- postfix expr : [0], (), (args), derefs, ++, -- 
- unary expr : &, *, +, -, ~,  NOT, abs,
- unary keywords:  move, copy, ref
- mult expr: %, *, /
- add expr: +, -
- chevron:  >>, << 
- bitwise ops (to do)
- bool ineq: gt lt gte lte
- bool and: and 
- bool or: or
- bool equal: eq ineq



### ranges
keyword : *range*
operators: union et intersection (peut etre ||, &&)
ex: range ] 0, 1000 ] union [ 2000, 3000 ] 


### properties and macros
properties written with # or #nameid
unamed property has to be a statement list
can be followed by 
- (function decl parameter) 
- { statement list }
- [ expr list ]

macros written @macroname
can be followed by
- (expr list)


# steps
## steps 1
compile functions, variables with integer type
*should compile and ret value 65*
fn main() -> Integer {
    let i : Integer#{ #value: range ]0; 1000]; } = 15;
    let result : Integer#{ #value: range ]0; 1000]; } = i + 5 * 10;
    return result;
} 

typedef Integer#{ #value: range ]0; 1000]; } as smalluint;
fn main() -> Integer {
    let i : smalluint = 15;
    let result : smalluint = i + 5 * 10;
    return result;
} 

*should not compile*
typedef Integer#{ #value: range ]- 1000; 0]; } as smalluint;
fn main() -> Integer {
    let i : smalluint = 15;
    let result : smalluint = i + 5 * 10;
    return result;
} 

**
