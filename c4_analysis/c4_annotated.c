/*
 * next() - Tokenizer function that advances to the next token in the source code.
 *
 * What the code does:
 * - Reads characters from the source code and groups them into tokens.
 * - Identifies keywords, identifiers, numbers, operators, and special symbols.
 * - Handles comments, string literals, and numeric literals.
 *
 * Why it is written in this way:
 * - Uses a loop to efficiently iterate through characters in the source code.
 * - Implements direct handling of different token types for performance and simplicity.
 * - Uses a global token (tk) and source pointer (p) to maintain state.
 *
 * How it fits into the compiler:
 * - This function is essential for lexical analysis, converting raw source code into tokens.
 * - Feeds parsed tokens into the parser and expression evaluator.
 * - Works closely with the symbol table and expression parser.
 */

//A void function doesn't return anything.
//This function is a lexical analyzer. It reads and tokenizes the next input symbol
void next()
{
  char *pp; // A temporary pointer for identifiers and string literals

  while (tk = *p) { // Read characters from the source code until a token is found
    ++p; // Move to the next character
    
    if (tk == '\n') { // Handle newlines
      if (src) { // If source debugging is enabled, print the source line and emitted instructions
        printf("%d: %.*s", line, p - lp, lp);
        lp = p;
        while (le < e) { // Print emitted assembly instructions if the condition is met
          printf("%8.4s", &"LEA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,"
                           "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
                           "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,"[*++le * 5]);
          if (*le <= ADJ) printf(" %d\n", *++le); else printf("\n");
        }
      }
      ++line; // Increment line number
    }
    else if (tk == '#') { // Handle preprocessor directives by skipping to the next line
      while (*p != 0 && *p != '\n') ++p;
    }
    else if ((tk >= 'a' && tk <= 'z') || (tk >= 'A' && tk <= 'Z') || tk == '_') { // Handle identifiers
      pp = p - 1; // Mark the start of the identifier
      while ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_')
        tk = tk * 147 + *p++; // Compute a hash value for the identifier
      tk = (tk << 6) + (p - pp); // Unique identifier hash
      id = sym; // Start searching the symbol table
      while (id[Tk]) { // Check if the identifier is already in the symbol table
        if (tk == id[Hash] && !memcmp((char *)id[Name], pp, p - pp)) { tk = id[Tk]; return; }
        id = id + Idsz;
      }
      id[Name] = (int)pp; // Store identifier name
      id[Hash] = tk; // Store hash value
      tk = id[Tk] = Id; // Mark it as an identifier token
      return;
    }
    else if (tk >= '0' && tk <= '9') { // Handle numeric literals
      if (ival = tk - '0') { while (*p >= '0' && *p <= '9') ival = ival * 10 + *p++ - '0'; } // Decimal numbers
      else if (*p == 'x' || *p == 'X') { // Hexadecimal numbers
        while ((tk = *++p) && ((tk >= '0' && tk <= '9') || (tk >= 'a' && tk <= 'f') || (tk >= 'A' && tk <= 'F')))
          ival = ival * 16 + (tk & 15) + (tk >= 'A' ? 9 : 0);
      }
      else { while (*p >= '0' && *p <= '7') ival = ival * 8 + *p++ - '0'; } // Octal numbers
      tk = Num;
      return;
    }
    else if (tk == '/') { // Handle comments and division operator
      if (*p == '/') { // Line comment
        ++p;
        while (*p != 0 && *p != '\n') ++p;
      }
      else {
        tk = Div; // Division operator
        return;
      }
    }
    else if (tk == '\'' || tk == '"') { // Handle character and string literals
      pp = data;
      while (*p != 0 && *p != tk) {
        if ((ival = *p++) == '\\') {
          if ((ival = *p++) == 'n') ival = '\n'; // Handle escape sequences
        }
        if (tk == '"') *data++ = ival; // Store string characters
      }
      ++p;
      if (tk == '"') ival = (int)pp; else tk = Num; // String literals are stored, char literals are numbers
      return;
    }

    // Handle assignment, comparison, and arithmetic operators
    else if (tk == '=') { 
        // The '=' operator can either be an assignment ('=') or a comparison ('==').
        // If the next character is also '=', it's the equality operator (==).
        if (*p == '=') { ++p; tk = Eq; } else tk = Assign; 
        return; 
    }
    else if (tk == '+') { 
        // The '+' operator can either be addition ('+') or increment ('++').
        // If the next character is also '+', it's an increment operator.
        if (*p == '+') { ++p; tk = Inc; } else tk = Add; 
        return; 
    }
    else if (tk == '-') { 
        // The '-' operator can either be subtraction ('-') or decrement ('--').
        if (*p == '-') { ++p; tk = Dec; } else tk = Sub; 
        return; 
    }
    else if (tk == '!') { 
        // The '!' operator can either be logical NOT ('!') or '!=' (not equal).
        if (*p == '=') { ++p; tk = Ne; } 
        return; 
    }

    // Handle relational (comparison) and bitwise shift operators
    else if (tk == '<') { 
        // '<' can be:
        // - '<=' (less than or equal)
        // - '<<' (left bitwise shift)
        // - '<' (less than)
        if (*p == '=') { ++p; tk = Le; } 
        else if (*p == '<') { ++p; tk = Shl; } 
        else tk = Lt; 
        return; 
    }
    else if (tk == '>') { 
        // '>' can be:
        // - '>=' (greater than or equal)
        // - '>>' (right bitwise shift)
        // - '>' (greater than)
        if (*p == '=') { ++p; tk = Ge; } 
        else if (*p == '>') { ++p; tk = Shr; } 
        else tk = Gt; 
        return; 
    }

    // Handle logical and bitwise operators
    else if (tk == '|') { 
        // '|' can be:
        // - '||' (logical OR for conditions)
        // - '|' (bitwise OR)
        if (*p == '|') { ++p; tk = Lor; } else tk = Or; 
        return; 
    }
    else if (tk == '&') { 
        // '&' can be:
        // - '&&' (logical AND for conditions)
        // - '&' (bitwise AND)
        if (*p == '&') { ++p; tk = Lan; } else tk = And; 
        return; 
    }
    else if (tk == '^') { 
        // '^' is always a bitwise XOR.
        tk = Xor; 
        return; 
    }
    else if (tk == '%') { 
        // '%' is always a modulus operator.
        tk = Mod; 
        return; 
    }
    else if (tk == '*') { 
        // '*' is always a multiplication operator.
        tk = Mul; 
        return; 
    }

    // Handle brackets, conditional, and special characters
    else if (tk == '[') { 
        // '[' is used for array indexing.
        tk = Brak; 
        return; 
    }  
    else if (tk == '?') { 
        // '?' is the conditional (ternary) operator.
        tk = Cond; 
        return; 
    }  
    else if (tk == '~' || tk == ';' || tk == '{' || tk == '}' || 
             tk == '(' || tk == ')' || tk == ']' || tk == ',' || tk == ':') { 
        // These characters are returned as tokens directly without further processing.
        return;
    }
}




/*
 * expr(int lev) - Parses and generates code for expressions.
 *
 * What the code does:
 * - Parses expressions and converts them into intermediate machine code.
 * - Supports various expression types, including literals, identifiers, function calls, unary and binary operations.
 * - Implements precedence climbing to handle operator precedence.
 * - Generates appropriate instructions for each expression type.
 *
 * Why it is written in this way:
 * - Uses a recursive structure to handle nested expressions.
 * - Implements a "precedence climbing" approach for efficient parsing of binary operators.
 * - Uses a global token (tk) to keep track of the current token and progress the parser.
 *
 * How it fits into the compiler:
 * - This function is central to parsing and compiling expressions.
 * - Generates intermediate code for expressions, which is later used in code generation.
 * - Works with the tokenizer (next()) and symbol table (id) to process variables and function calls.
 */

void expr(int lev) 
{
    int t, *d; // Temporary variables to store type and identifier pointer

    // Handle different types of expressions
    if (!tk) { // Check for end of file (unexpected termination)
        printf("%d: unexpected eof in expression\n", line);
        exit(-1);
    }
    else if (tk == Num) { // Numeric literal
        *++e = IMM; // Load immediate value into generated code
        *++e = ival; // Store the numeric value
        next(); // Move to the next token
        ty = INT; // Set type to integer
    }
    else if (tk == '"') { // String literal
        *++e = IMM; // Load immediate value (address of string)
        *++e = ival;
        next(); // Move to the next token
        while (tk == '"') next(); // Handle concatenated string literals
        data = (char *)((int)data + sizeof(int) & -sizeof(int)); // Align data pointer
        ty = PTR; // Set type to pointer (string is an array of chars)
    }
    else if (tk == Sizeof) { // sizeof operator
        next();
        if (tk == '(') next(); else { // Expect '(' after sizeof
            printf("%d: open paren expected in sizeof\n", line);
            exit(-1);
        }
        ty = INT; // Default type is integer
        if (tk == Int) next();
        else if (tk == Char) { next(); ty = CHAR; }
        while (tk == Mul) { next(); ty = ty + PTR; } // Handle pointer types
        if (tk == ')') next(); else { // Expect ')'
            printf("%d: close paren expected in sizeof\n", line);
            exit(-1);
        }
        *++e = IMM; // Load size as immediate value
        *++e = (ty == CHAR) ? sizeof(char) : sizeof(int);
        ty = INT; // sizeof always returns an integer
    }
    else if (tk == Id) { // Identifier (variable or function call)
        d = id; // Store pointer to identifier in symbol table
        next(); // Move to next token
        if (tk == '(') { // Function call
            next();
            t = 0;
            while (tk != ')') { // Parse function arguments
                expr(Assign);
                *++e = PSH; // Push argument onto stack
                ++t;
                if (tk == ',') next();
            }
            next();
            if (d[Class] == Sys) *++e = d[Val]; // System call
            else if (d[Class] == Fun) { // User-defined function
                *++e = JSR; // Jump to subroutine
                *++e = d[Val];
            }
            else { // Error: not a function
                printf("%d: bad function call\n", line);
                exit(-1);
            }
            if (t) { *++e = ADJ; *++e = t; } // Adjust stack pointer for arguments
            ty = d[Type]; // Set type to function return type
        }
        else if (d[Class] == Num) { // Constant variable
            *++e = IMM; *++e = d[Val];
            ty = INT;
        }
        else { // Variable
            if (d[Class] == Loc) { // Local variable
                *++e = LEA; // Load effective address
                *++e = loc - d[Val];
            }
            else if (d[Class] == Glo) { // Global variable
                *++e = IMM; // Load global address
                *++e = d[Val];
            }
            else { // Undefined variable error
                printf("%d: undefined variable\n", line);
                exit(-1);
            }
            *++e = ((ty = d[Type]) == CHAR) ? LC : LI; // Load value (char or int)
        }
    }
    
    // Handle parenthesized expressions and type casts
    else if (tk == '(') {
        next(); // Consume '('
        
        // Handle type casting (e.g., (int)x or (char)x)
        if (tk == Int || tk == Char) {
            t = (tk == Int) ? INT : CHAR; // Determine the type (int or char)
            next(); // Move past type keyword
            
            // Handle pointer types (e.g., (int*)x or (char**)x)
            while (tk == Mul) {
                next();
                t = t + PTR; // Increase pointer level
            }
            
            // Ensure closing parenthesis after type specifier
            if (tk == ')') next();
            else { printf("%d: bad cast\n", line); exit(-1); }
            
            expr(Inc); // Parse the expression being cast
            ty = t; // Set resulting type
        }
        
        // Handle normal parenthesized expressions (e.g., (a + b))
        else {
            expr(Assign); // Parse the inner expression
            
            // Ensure closing parenthesis after expression
            if (tk == ')') next();
            else { printf("%d: close paren expected\n", line); exit(-1); }
        }
    }
    
    // Handle pointer dereference '*ptr'
    else if (tk == Mul) {
        next(); // Consume '*'
        expr(Inc); // Parse operand being dereferenced
        
        // Ensure operand is a valid pointer
        if (ty > INT) ty = ty - PTR; // Reduce pointer level (e.g., int* → int)
        else { printf("%d: bad dereference\n", line); exit(-1); }
        
        // Load value from memory
        *++e = (ty == CHAR) ? LC : LI; // Load char or integer from pointer
    }
}

    // Handle address-of operator '&'
    else if (tk == And) {
        next(); expr(Inc); // Parse the operand
        if (*e == LC || *e == LI) --e; // If operand is a variable, keep address
        else { printf("%d: bad address-of\n", line); exit(-1); }
        ty = ty + PTR; // Convert to pointer type
    }
    
    // Handle logical NOT '!' operator
    else if (tk == '!') {
        next(); expr(Inc);
        *++e = PSH; *++e = IMM; *++e = 0; *++e = EQ; // Compare with zero
        ty = INT; // Logical NOT always results in integer type
    }
    
    // Handle bitwise NOT '~' operator
    else if (tk == '~') {
        next(); expr(Inc);
        *++e = PSH; *++e = IMM; *++e = -1; *++e = XOR; // Bitwise NOT
        ty = INT;
    }
    
    // Handle unary plus '+' operator
    else if (tk == Add) {
        next(); expr(Inc);
        ty = INT; // Has no effect but ensures integer type
    }
    
    // Handle unary minus '-' operator
    else if (tk == Sub) {
        next(); *++e = IMM;
        if (tk == Num) {
            *++e = -ival; // Directly negate number
            next();
        } else {
            *++e = -1; *++e = PSH; expr(Inc); *++e = MUL; // Multiply by -1
        }
        ty = INT;
    }
    
    // Handle pre-increment and pre-decrement '++' and '--'
    else if (tk == Inc || tk == Dec) {
        t = tk; next(); expr(Inc);
        if (*e == LC) { *e = PSH; *++e = LC; } // Load variable value
        else if (*e == LI) { *e = PSH; *++e = LI; }
        else { printf("%d: bad lvalue in pre-increment\n", line); exit(-1); }
        *++e = PSH;
        *++e = IMM; *++e = (ty > PTR) ? sizeof(int) : sizeof(char); // Increment size
        *++e = (t == Inc) ? ADD : SUB; // Add or subtract 1
        *++e = (ty == CHAR) ? SC : SI; // Store result back
    }
    
    else { printf("%d: bad expression\n", line); exit(-1); }

    // Handle operator precedence using precedence climbing (Top Down Operator Precedence)
    while (tk >= lev) { // Process operators based on precedence
        t = ty; // Save the current expression type
        
        // Handle assignment operator '='
        if (tk == Assign) {
            next(); // Move past '='
            
            // Ensure that the left-hand side of assignment is a valid lvalue
            if (*e == LC || *e == LI) *e = PSH; // Convert value to a modifiable location
            else { printf("%d: bad lvalue in assignment\n", line); exit(-1); } // Error if not a valid lvalue
            
            expr(Assign); // Recursively process the right-hand side expression
            
            // Generate code to store the value back
            *++e = ((ty = t) == CHAR) ? SC : SI; // Use SC for char, SI for int
        }
        
        // Handle ternary conditional operator '?:'
        else if (tk == Cond) {
            next(); // Move past '?'
            
            // Generate branch instruction for false case
            *++e = BZ; d = ++e; // Store jump address
            
            expr(Assign); // Parse true expression
            
            // Expect a colon ':' to separate true and false expressions
            if (tk == ':') next();
            else { printf("%d: conditional missing colon\n", line); exit(-1); } // Error if missing ':'
            
            // Patch false jump and generate jump over false expression
            *d = (int)(e + 3);
            *++e = JMP; d = ++e;
            
            expr(Cond); // Parse false expression
            
            // Patch jump instruction to continue after false expression
            *d = (int)(e + 1);
        }
    }
        
// Logical OR '||' - Short-circuit evaluation
        else if (tk == Lor) {
            next();
            *++e = BNZ; d = ++e; // Branch if true (short-circuit logic)
            expr(Lan); // Evaluate the next operand
            *d = (int)(e + 1); // Patch jump location
            ty = INT; // Logical operations return an integer
        }
        
        // Logical AND '&&' - Short-circuit evaluation
        else if (tk == Lan) {
            next();
            *++e = BZ; d = ++e; // Branch if false (short-circuit logic)
            expr(Or); // Evaluate the next operand
            *d = (int)(e + 1); // Patch jump location
            ty = INT;
        }
        
        // Bitwise OR '|'
        else if (tk == Or) {
            next();
            *++e = PSH; expr(Xor); // Push first operand, evaluate second
            *++e = OR; // Perform bitwise OR
            ty = INT;
        }
        
        // Bitwise XOR '^'
        else if (tk == Xor) {
            next();
            *++e = PSH; expr(And); // Push first operand, evaluate second
            *++e = XOR; // Perform bitwise XOR
            ty = INT;
        }
        
        // Bitwise AND '&'
        else if (tk == And) {
            next();
            *++e = PSH; expr(Eq); // Push first operand, evaluate second
            *++e = AND; // Perform bitwise AND
            ty = INT;
        }
        
        // Equality and relational operators
        // These compare two values and return 1 (true) or 0 (false)
        else if (tk == Eq) {
            next();
            *++e = PSH; expr(Lt); // Push first operand, evaluate second
            *++e = EQ; // Check if equal
            ty = INT;
        }
        else if (tk == Ne) {
            next();
            *++e = PSH; expr(Lt);
            *++e = NE; // Check if not equal
            ty = INT;
        }
        else if (tk == Lt) {
            next();
            *++e = PSH; expr(Shl);
            *++e = LT; // Check if less than
            ty = INT;
        }
        else if (tk == Gt) {
            next();
            *++e = PSH; expr(Shl);
            *++e = GT; // Check if greater than
            ty = INT;
        }
        else if (tk == Le) {
            next();
            *++e = PSH; expr(Shl);
            *++e = LE; // Check if less than or equal
            ty = INT;
        }
        else if (tk == Ge) {
            next();
            *++e = PSH; expr(Shl);
            *++e = GE; // Check if greater than or equal
            ty = INT;
        }
// Handle shift operators
    else if (tk == Shl) { // Left shift '<<'
        next();
        *++e = PSH; expr(Add); // Push first operand and parse the second
        *++e = SHL; // Generate left shift instruction
        ty = INT; // Shift operations return integers
    }
    else if (tk == Shr) { // Right shift '>>'
        next();
        *++e = PSH; expr(Add);
        *++e = SHR; // Generate right shift instruction
        ty = INT;
    }
    
    // Handle addition '+' operator
    else if (tk == Add) {
        next();
        *++e = PSH; expr(Mul); // Push first operand and evaluate second
        
        // Handle pointer arithmetic: multiply by sizeof(int) if adding pointer
        if ((ty = t) > PTR) {
            *++e = PSH; *++e = IMM; *++e = sizeof(int); *++e = MUL;
        }
        *++e = ADD; // Perform addition
    }
    
    // Handle subtraction '-' operator
    else if (tk == Sub) {
        next();
        *++e = PSH; expr(Mul);
        
        // Special case: Pointer difference should be divided by sizeof(int)
        if (t > PTR && t == ty) {
            *++e = SUB; *++e = PSH; *++e = IMM; *++e = sizeof(int); *++e = DIV;
            ty = INT; // Result is an integer
        }
        // Pointer arithmetic (e.g., ptr - offset)
        else if ((ty = t) > PTR) {
            *++e = PSH; *++e = IMM; *++e = sizeof(int); *++e = MUL;
            *++e = SUB;
        }
        else {
            *++e = SUB; // Regular subtraction
        }
    }
    
    // Handle multiplication '*'
    else if (tk == Mul) {
        next();
        *++e = PSH; expr(Inc); // Push first operand and evaluate second
        *++e = MUL; // Generate multiplication instruction
        ty = INT;
    }
    
    // Handle division '/'
    else if (tk == Div) {
        next();
        *++e = PSH; expr(Inc);
        *++e = DIV; // Generate division instruction
        ty = INT;
    }
    
    // Handle modulo '%'
    else if (tk == Mod) {
        next();
        *++e = PSH; expr(Inc);
        *++e = MOD; // Generate modulo instruction
        ty = INT;
    }
    
    // Handle post-increment 'x++' and post-decrement 'x--'
    else if (tk == Inc || tk == Dec) {
        if (*e == LC) { *e = PSH; *++e = LC; } // Load char variable
        else if (*e == LI) { *e = PSH; *++e = LI; } // Load int variable
        else { printf("%d: bad lvalue in post-increment\n", line); exit(-1); }
        
        // Store original value before modification
        *++e = PSH;
        *++e = IMM; *++e = (ty > PTR) ? sizeof(int) : sizeof(char);
        *++e = (tk == Inc) ? ADD : SUB; // Perform increment or decrement
        *++e = (ty == CHAR) ? SC : SI; // Store back to memory
        
        // Ensure correct return value by undoing modification in register
        *++e = PSH; *++e = IMM; *++e = (ty > PTR) ? sizeof(int) : sizeof(char);
        *++e = (tk == Inc) ? SUB : ADD;
        next();
    }
    
    // Handle array indexing (subscript operator '[]')
    else if (tk == Brak) {
        next();
        *++e = PSH; expr(Assign); // Push base address and evaluate index
        
        // Ensure closing bracket ']'
        if (tk == ']') next();
        else { printf("%d: close bracket expected\n", line); exit(-1); }
        
        // Ensure the base expression is a pointer
        if (t > PTR) {
            *++e = PSH; *++e = IMM; *++e = sizeof(int); *++e = MUL; // Multiply index by sizeof(int)
        }
        else if (t < PTR) {
            printf("%d: pointer type expected\n", line); exit(-1); // Error if not a pointer
        }
        
        *++e = ADD; // Compute final memory address
        *++e = ((ty = t - PTR) == CHAR) ? LC : LI; // Load value from array
    }
    
    // Handle unknown tokens
    else {
        printf("%d: compiler error tk=%d\n", line, tk);
        exit(-1);
    }
}

}






/*
 * stmt() - Parses and generates code for statements.
 *
 * What the code does:
 * - Recognizes and processes control flow statements (if, while, return, blocks).
 * - Generates intermediate machine code for execution.
 * - Calls expr() to handle expressions within statements.
 *
 * Why it is written in this way:
 * - Uses a recursive structure to handle nested statements.
 * - Directly translates structured code into low-level operations.
 * - Uses a global token (tk) and program counter (e) for efficiency.
 *
 * How it fits into the compiler:
 * - This function forms the backbone of statement processing.
 * - Works closely with expr() to evaluate expressions inside statements.
 * - Generates jump instructions for control flow constructs.
 */

void stmt()
{
    int *a, *b; // Temporary variables for jump addresses

    if (tk == If) { // Handle 'if' statement
        next();
        if (tk == '(') next(); else { printf("%d: open paren expected\n", line); exit(-1); }
        expr(Assign); // Evaluate the condition
        if (tk == ')') next(); else { printf("%d: close paren expected\n", line); exit(-1); }
        *++e = BZ; // Branch if zero (false condition)
        b = ++e;
        stmt(); // Parse the true branch
        if (tk == Else) { // Handle 'else' clause
            *b = (int)(e + 3);
            *++e = JMP; // Unconditional jump
            b = ++e;
            next();
            stmt(); // Parse the false branch
        }
        *b = (int)(e + 1); // Patch jump address
    }
    else if (tk == While) { // Handle 'while' loop
        next();
        a = e + 1; // Store loop start address
        if (tk == '(') next(); else { printf("%d: open paren expected\n", line); exit(-1); }
        expr(Assign); // Evaluate the loop condition
        if (tk == ')') next(); else { printf("%d: close paren expected\n", line); exit(-1); }
        *++e = BZ; // Branch if condition is false
        b = ++e;
        stmt(); // Parse loop body
        *++e = JMP; // Unconditional jump to loop start
        *++e = (int)a;
        *b = (int)(e + 1); // Patch jump address
    }
    else if (tk == Return) { // Handle 'return' statement
        next();
        if (tk != ';') expr(Assign); // Evaluate return expression
        *++e = LEV; // Generate return instruction
        if (tk == ';') next(); else { printf("%d: semicolon expected\n", line); exit(-1); }
    }
    else if (tk == '{') { // Handle block statement
        next();
        while (tk != '}') stmt(); // Parse inner statements
        next();
    }
    else if (tk == ';') { // Handle empty statement
        next();
    }
    else { // Handle expression statement
        expr(Assign);
        if (tk == ';') next(); else { printf("%d: semicolon expected\n", line); exit(-1); }
    }
}







// /*
//  * main(int argc, char **argv) - Entry point for the C4 compiler.
//  *
//  * What the code does:
//  * - Parses command-line arguments to enable debugging or source printing.
//  * - Allocates memory for different compiler components (symbol table, text area, stack, etc.).
//  * - Reads and tokenizes the input source code.
//  * - Initializes keyword and library function tables.
//  * - Prepares the source code for parsing.
//  *
//  * Why it is written in this way:
//  * - Implements a simple compiler structure that combines parsing and execution.
//  * - Uses a single-pass approach to scan, parse, and execute in one process.
//  * - Uses a small virtual machine to interpret the compiled code.
//  *
//  * How it fits into the compiler:
//  * - Acts as the main driver for the entire compilation and execution process.
//  * - Calls tokenizer, parser, and virtual machine components to process input code.
//  * - Handles errors and resource management (memory allocation and deallocation).
//  */

int main(int argc, char **argv)
{
  // Variable declarations
  int fd, bt, ty, poolsz, *idmain; // File descriptor, types, memory pool size, and pointer to main function identifier
  int *pc, *sp, *bp, a, cycle; // Virtual machine registers: program counter, stack pointer, base pointer, accumulator, and execution cycle counter
  int i, *t; // Temporary variables used throughout execution

  // Process command-line arguments to check for options
  --argc; ++argv; // Move past the program name
  if (argc > 0 && **argv == '-' && (*argv)[1] == 's') { // Check for '-s' flag (source output mode)
    src = 1; 
    --argc; 
    ++argv; 
  } 
  if (argc > 0 && **argv == '-' && (*argv)[1] == 'd') { // Check for '-d' flag (debug mode)
    debug = 1; 
    --argc; 
    ++argv; 
  } 
  if (argc < 1) { // Ensure a file argument is provided
    printf("usage: c4 [-s] [-d] file ...\n"); 
    return -1; 
  }

  // Open the source file for reading
  if ((fd = open(*argv, 0)) < 0) { 
    printf("could not open(%s)\n", *argv); // Print an error message if file opening fails
    return -1; 
  }

  // Allocate memory pools for different components of the compiler
  poolsz = 256*1024; // Set memory pool size to 256 KB (arbitrary but sufficient size)
  if (!(sym = malloc(poolsz))) { // Allocate memory for the symbol table (stores variable and function names)
    printf("could not malloc(%d) symbol area\n", poolsz); 
    return -1; 
  }
  if (!(le = e = malloc(poolsz))) { // Allocate memory for the text area (stores compiled code)
    printf("could not malloc(%d) text area\n", poolsz); 
    return -1; 
  }
  if (!(data = malloc(poolsz))) { // Allocate memory for the data area (global variables and static storage)
    printf("could not malloc(%d) data area\n", poolsz); 
    return -1; 
  }
  if (!(sp = malloc(poolsz))) { // Allocate memory for the stack (stores function calls and local variables)
    printf("could not malloc(%d) stack area\n", poolsz); 
    return -1; 
  }

  // Initialize allocated memory to zero to prevent undefined behavior
  memset(sym,  0, poolsz);
  memset(e,    0, poolsz);
  memset(data, 0, poolsz);

  // Initialize keywords and standard library functions into the symbol table
  p = "char else enum if int return sizeof while "
      "open read close printf malloc free memset memcmp exit void main"; // List of reserved words and built-in functions
  i = Char; 
  while (i <= While) { // Assign token values to keywords and store them in the symbol table
    next(); 
    id[Tk] = i++; 
  }
  i = OPEN; 
  while (i <= EXIT) { // Assign token values to built-in functions (e.g., file handling, memory management)
    next(); 
    id[Class] = Sys; // Mark as system functions
    id[Type] = INT; // Assign integer return type
    id[Val] = i++; 
  }
  next(); id[Tk] = Char; // Handle 'void' type explicitly (often represented as 'char' internally)
  next(); idmain = id; // Keep track of 'main' function identifier for execution entry point

  // Read source file into allocated memory buffer
  if (!(lp = p = malloc(poolsz))) { // Allocate memory for storing source code
    printf("could not malloc(%d) source area\n", poolsz); 
    return -1; 
  }
  if ((i = read(fd, p, poolsz-1)) <= 0) { // Read file contents into memory buffer
    printf("read() returned %d\n", i); // Print error message if reading fails
    return -1; 
  }
  p[i] = 0; // Null-terminate the source file content to ensure proper string handling
  close(fd); // Close the file descriptor to free system resources
// Parse global declarations and function definitions
line = 1; // Initialize line counter
next(); // Read the first token
while (tk) { // Loop through tokens until the end of the file
  bt = INT; // Default base type is INT
  
  // Check for type specifier (int, char, or enum)
  if (tk == Int) next(); // If 'int' is found, move to the next token
  else if (tk == Char) { next(); bt = CHAR; } // If 'char' is found, set base type to CHAR
  else if (tk == Enum) { // If 'enum' is found, handle enumeration type
    next();
    if (tk != '{') next(); // Skip identifier if present
    if (tk == '{') { // If an enumeration block is found
      next();
      i = 0; // Start enumeration value from 0
      while (tk != '}') { // Parse enum entries
        if (tk != Id) { // Ensure valid identifier
          printf("%d: bad enum identifier %d\n", line, tk); 
          return -1; 
        }
        next(); // Move to next token
        if (tk == Assign) { // Handle explicit value assignment
          next();
          if (tk != Num) { // Ensure a valid numeric value is assigned
            printf("%d: bad enum initializer\n", line); 
            return -1; 
          }
          i = ival; // Set current enumeration value
          next();
        }
        id[Class] = Num; id[Type] = INT; id[Val] = i++; // Store enum identifier and increment value
        if (tk == ',') next(); // Move to next enum entry
      }
      next(); // Move past '}'
    }
  }
  
  // Parse global variables and function declarations
  while (tk != ';' && tk != '}') { 
    ty = bt; // Start with base type
    while (tk == Mul) { next(); ty = ty + PTR; } // Handle pointer types
    
    if (tk != Id) { // Ensure valid identifier
      printf("%d: bad global declaration\n", line); 
      return -1; 
    }
    if (id[Class]) { // Check for duplicate definitions
      printf("%d: duplicate global definition\n", line); 
      return -1; 
    }
    next(); // Move to next token
    id[Type] = ty; // Store type information
    
    if (tk == '(') { // If '(' follows, it's a function declaration
      id[Class] = Fun; // Mark as function
      id[Val] = (int)(e + 1); // Store function entry point
      next(); 
      i = 0; // Parameter count
      while (tk != ')') { // Parse function parameters
        ty = INT; // Default parameter type is INT
        if (tk == Int) next(); // If 'int' is specified, move on
        else if (tk == Char) { next(); ty = CHAR; } // Handle 'char' type
        while (tk == Mul) { next(); ty = ty + PTR; } // Handle pointer parameters
        
        if (tk != Id) { // Ensure valid parameter name
          printf("%d: bad parameter declaration\n", line); 
          return -1; 
        }
        if (id[Class] == Loc) { // Check for duplicate parameter definitions
          printf("%d: duplicate parameter definition\n", line); 
          return -1; 
        }
        
        // Store parameter information
        id[HClass] = id[Class]; id[Class] = Loc;
        id[HType]  = id[Type];  id[Type] = ty;
        id[HVal]   = id[Val];   id[Val] = i++;
        
        next(); // Move to next token
        if (tk == ',') next(); // Handle multiple parameters
      }
// Process the function definition when a '{' is encountered, signaling the start of a function body.
next();
if (tk != '{') { 
    // If the token is not '{', it's an invalid function definition, so print an error and return -1.
    printf("%d: bad function definition\n", line); 
    return -1; 
}

// Set 'loc' to the index after the function token and call 'next()' to move to the next token.
loc = ++i;
next();

// This loop processes local variable declarations, checking for Int or Char types.
while (tk == Int || tk == Char) {
    // Assign the appropriate base type (Int or Char) to 'bt'.
    bt = (tk == Int) ? INT : CHAR;
    next();

    // The inner while loop processes declarations of local variables within the function body.
    while (tk != ';') {
        ty = bt;  // Set the type of the variable to the base type (Int or Char).

        // Check for pointer types by detecting '*' and adjusting the type accordingly.
        while (tk == Mul) { 
            next(); 
            ty = ty + PTR;  // Add pointer to the type.
        }

        // If the token is not an identifier (variable), print an error and return -1.
        if (tk != Id) { 
            printf("%d: bad local declaration\n", line); 
            return -1; 
        }

        // Check for duplicate local variable definitions in the current scope.
        if (id[Class] == Loc) { 
            printf("%d: duplicate local definition\n", line); 
            return -1; 
        }

        // Set the symbol table fields for the local variable: class, type, and value.
        id[HClass] = id[Class]; id[Class] = Loc;
        id[HType]  = id[Type];  id[Type] = ty;
        id[HVal]   = id[Val];   id[Val] = ++i;

        next(); // Move to the next token.

        // If a comma is encountered, move to the next token to process another variable declaration.
        if (tk == ',') next();
    }
    next(); // Move to the next token after processing all local variables.
}

// Create an entry for the function in the stack (adjust the stack pointer).
*++e = ENT; *++e = i - loc;

// Process statements inside the function body until '}' is encountered.
while (tk != '}') stmt();

// Mark the end of the function with a LEV (level) indicator on the stack.
*++e = LEV;

// Unwind the symbol table for local variables by restoring previous entries.
id = sym; // Start from the base symbol table.
while (id[Tk]) {
    if (id[Class] == Loc) {
        id[Class] = id[HClass];  // Restore class to previous state.
        id[Type] = id[HType];    // Restore type.
        id[Val] = id[HVal];      // Restore value.
    }
    id = id + Idsz; // Move to the next symbol in the table.
}

// If the token is not a '{', it is a global declaration.
else {
    // Set the class of the symbol to 'Glo' (global) and assign a value (memory address).
    id[Class] = Glo;
    id[Val] = (int)data;
    data = data + sizeof(int);  // Update the data pointer to allocate space for the global variable.
}

// If there is a comma, continue to process the next variable.
if (tk == ',') next();

next();  // Move to the next token.
}

// Check if the 'main' function is defined. If not, print an error and return -1.
if (!(pc = (int *)idmain[Val])) { 
    printf("main() not defined\n"); 
    return -1; 
}

// If the source is set, return 0 to indicate successful processing.
if (src) return 0;

// Setup the stack for execution: adjust the stack pointer 'bp' and 'sp'.
bp = sp = (int *)((int)sp + poolsz);

// Initialize the stack for the function call by pushing the exit function (EXIT).
*--sp = EXIT;  // If main returns, exit the program.

// Set up the stack for argument passing: argc and argv.
*--sp = PSH; 
t = sp;
*--sp = argc;  // Push argument count.
*--sp = (int)argv;  // Push argument values.
*--sp = (int)t;  // Push the stack pointer as a reference.
// Start the execution cycle. Initialize cycle counter to 0.
cycle = 0;
while (1) {
    // Fetch the next instruction (i) from the program counter (pc).
    i = *pc++; 
    ++cycle;  // Increment the cycle counter.

    // If debugging is enabled, print the instruction and its arguments for tracing.
    if (debug) {
        printf("%d> %.4s", cycle,
            &"LEA ,IMM ,JMP ,JSR ,BZ  ,BNZ ,ENT ,ADJ ,LEV ,LI  ,LC  ,SI  ,SC  ,PSH ,"
             "OR  ,XOR ,AND ,EQ  ,NE  ,LT  ,GT  ,LE  ,GE  ,SHL ,SHR ,ADD ,SUB ,MUL ,DIV ,MOD ,"
             "OPEN,READ,CLOS,PRTF,MALC,FREE,MSET,MCMP,EXIT,"[i * 5]);  // Print instruction name.

        // If the instruction requires an argument (e.g., ADJ), print it.
        if (i <= ADJ) 
            printf(" %d\n", *pc); 
        else 
            printf("\n");
    }

    // Execute the instruction based on the value of 'i'.
    if (i == LEA) 
        a = (int)(bp + *pc++);  // Load local address into 'a'.
    else if (i == IMM) 
        a = *pc++;  // Load global address or immediate value into 'a'.
    else if (i == JMP) 
        pc = (int *)*pc;  // Jump to the address specified by the current value of 'pc'.
    else if (i == JSR) { 
        *--sp = (int)(pc + 1);  // Save the return address on the stack.
        pc = (int *)*pc;  // Jump to subroutine.
    }
    else if (i == BZ)  
        pc = a ? pc + 1 : (int *)*pc;  // Branch if zero condition (a == 0).
    else if (i == BNZ) 
        pc = a ? (int *)*pc : pc + 1;  // Branch if not zero condition (a != 0).
    else if (i == ENT) { 
        *--sp = (int)bp;  // Save the old base pointer (bp) on the stack.
        bp = sp;  // Update the base pointer to the current stack pointer.
        sp = sp - *pc++;  // Adjust stack pointer based on the local variables for the subroutine.
    }
    else if (i == ADJ) 
        sp = sp + *pc++;  // Adjust the stack pointer (e.g., allocating space for local variables).
    else if (i == LEV) { 
        sp = bp;  // Restore the stack pointer from the base pointer.
        bp = (int *)*sp++;  // Restore the previous base pointer.
        pc = (int *)*sp++;  // Restore the return address (next instruction after the subroutine).
    }
    else if (i == LI)  
        a = *(int *)a;  // Dereference the pointer in 'a' to load an integer value.
    else if (i == LC)  
        a = *(char *)a;  // Dereference the pointer in 'a' to load a char value.
    else if (i == SI)  
        *(int *)*sp++ = a;  // Store integer 'a' at the memory location pointed by 'sp'.
    else if (i == SC)  
        a = *(char *)*sp++ = a;  // Store char 'a' at the memory location pointed by 'sp'.
    else if (i == PSH) 
        *--sp = a;  // Push the value of 'a' onto the stack.

    // Perform bitwise or, xor, and, and other logical operations using the stack.
    else if (i == OR)  
        a = *sp++ |  a;  // Perform OR operation between top of stack and 'a'.
    else if (i == XOR) 
        a = *sp++ ^  a;  // Perform XOR operation.
    else if (i == AND) 
        a = *sp++ &  a;  // Perform AND operation.
    else if (i == EQ)  
        a = *sp++ == a;  // Check equality.
    else if (i == NE)  
        a = *sp++ != a;  // Check inequality.
    else if (i == LT)  
        a = *sp++ <  a;  // Check if less than.
    else if (i == GT)  
        a = *sp++ >  a;  // Check if greater than.
    else if (i == LE)  
        a = *sp++ <= a;  // Check if less than or equal.
    else if (i == GE)  
        a = *sp++ >= a;  // Check if greater than or equal.
    else if (i == SHL) 
        a = *sp++ << a;  // Perform left shift operation.
    else if (i == SHR) 
        a = *sp++ >> a;  // Perform right shift operation.
    else if (i == ADD) 
        a = *sp++ +  a;  // Perform addition.
    else if (i == SUB) 
        a = *sp++ -  a;  // Perform subtraction.
    else if (i == MUL) 
        a = *sp++ *  a;  // Perform multiplication.
    else if (i == DIV) 
        a = *sp++ /  a;  // Perform division.
    else if (i == MOD) 
        a = *sp++ %  a;  // Perform modulo operation.

    // Handle system calls for file and memory operations.
    else if (i == OPEN) 
        a = open((char *)sp[1], *sp);  // Open a file.
    else if (i == READ) 
        a = read(sp[2], (char *)sp[1], *sp);  // Read from file.
    else if (i == CLOS) 
        a = close(*sp);  // Close the file.
    else if (i == PRTF) { 
        t = sp + pc[1]; 
        a = printf((char *)t[-1], t[-2], t[-3], t[-4], t[-5], t[-6]); 
    }
    else if (i == MALC) 
        a = (int)malloc(*sp);  // Allocate memory.
    else if (i == FREE) 
        free((void *)*sp);  // Free allocated memory.
    else if (i == MSET) 
        a = (int)memset((char *)sp[2], sp[1], *sp);  // Set memory.
    else if (i == MCMP) 
        a = memcmp((char *)sp[2], (char *)sp[1], *sp);  // Compare memory regions.
    else if (i == EXIT) { 
        printf("exit(%d) cycle = %d\n", *sp, cycle);  // Print exit message with return value and cycle count.
        return *sp;  // Exit the program with the value of 'sp'.
    }

    // Handle any unknown instructions.
    else { 
        printf("unknown instruction = %d! cycle = %d\n", i, cycle);  // Print error message for unknown instruction.
        return -1;  // Return -1 to indicate an error.
    }
}
