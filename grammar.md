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
- unary expr : *, +, -,   NOT, abs,
- unary ref: ref
- unary keywords: move, copy
- mult expr: %, *, /
- add expr: +, -
- chevron:  >>, << 
- bitwise ops (to do)
- bool ineq: gt lt gte lte
- bool and: and 
- bool or: or
- bool equal: eq ineq


### functions
keyword: fn
can be fn functionName(paramList) -> returnType { /*body*/}
can also be
fn functionName
fn functionName(paramList) { /*body*/}
fn functionName#{
    #params(paramList)
    #returns: type
    #precondition{}
    #body{}
    #postcondition{}
    #errors[fileNotFound, flagWrong]
}

??? how to handle return type + #properties
fn functionName#{
    #params(paramList);
    #precondition{};
    #body{};
    #postcondition{};
    #errors[fileNotFound, flagWrong];
} -> returnType



### ranges
keyword : *range*
operators: union et intersection (peut etre ||, &&)
ex: range ] 0, 1000 ] union [ 2000, 3000 ] 

simple_range : [ 0; 1000 ]
intersect
union


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
fn Integer main() {
    let i : Integer#{ #value: range ]0; 1000]; } = 15;
    let result : Integer#{ #value: range ]0; 1000]; } = i + 5 * 10;
    return result;
} 

typedef Integer#{ #value: range ]0; 1000]; } as smalluint;
fn Integer main()  {
    let i : smalluint = 15;
    let result : smalluint = i + 5 * 10;
    return result;
} 


*should not compile*
typedef Integer#{ #value: range ]- 1000; 0]; } as smalluint;
fn Integer main()  {
    let i : smalluint = 15;
    let result : smalluint = i + 5 * 10;
    return result;
}


## step 2
add strings, range upadate and calls to print
//============================
fn Integer#value: range [1;[ fibo(n: Integer#value: range [0; [ )  {
    if (n == 0) {
        return 1;
    } 
    if (n == 1) {
        return 1;
    }
    //allows to work on range updating for variables
    return fibo(n - 1) + fibo(n - 2)
}

fn Integer main() {
    let Integer result = fibo(10);
    std::print("Fibo result is equal to " + result.toString());
    return 0;
} 
