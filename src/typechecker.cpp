#include "typechecker.h"
#include <iostream>

TypeError::TypeError(const std::string &msg, std::string filename, const int &line)
{
    if (filename.empty())
    {
        this->msg = "\033[31mType Error\033[0m: " + msg;
    }
    else
    {
        this->msg = filename + ":" + std::to_string(line) + ": \033[31mType Error\033[0m: " + msg;
    }
}

TypeError::TypeError(const std::string &msg) : msg("\033[31mType Error\033[0m: " + msg) {}

const char *TypeError::what() const noexcept { return this->msg.c_str(); }

TypeChecker::TypeChecker(SyntaxTreeNode *root) : root(root), symbolTable(SymbolTable::empty()) {}

void TypeChecker::check()
{
    try
    {
        checkProgram(root);
    }
    catch (const TypeError &e)
    {
        std::cerr << "\n"
                  << e.what() << std::endl;
    }
}

void TypeChecker::setFilename(const std::string &filename)
{
    this->filename = filename;
}

void TypeChecker::checkProgram(SyntaxTreeNode *node)
{
    // PROG -> main GLOBVARS ALGO FUNCTIONS

    // start by checking global variables
    if (node->getChildren()[1]->getSymbol() == "GLOBVARS")
    {
        checkGlobVars(node->getChildren()[1]);
    }

    // next, check function declarations
    if (node->getChildren()[3]->getSymbol() == "FUNCTIONS")
    {
        checkFunctions(node->getChildren()[3]);
    }

    // finally, check the main algorithm block
    if (node->getChildren()[2]->getSymbol() == "ALGO")
    {
        checkAlgo(node->getChildren()[2]);
    }
}

void TypeChecker::checkGlobVars(SyntaxTreeNode *globVarsNode)
{
    // GLOBVARS -> '' | VTYP VNAME , GLOBVARS

    // base case: if the node is epsilon (empty), stop processing
    if (globVarsNode->getChildren().empty())
    {
        return;
    }

    // extract VTYP and VNAME
    SyntaxTreeNode *vtypNode = globVarsNode->getChildren()[0];  // VTYP node
    SyntaxTreeNode *vnameNode = globVarsNode->getChildren()[1]; // VNAME node

    std::string type = vtypNode->getChildren()[0]->getSymbol();          // the type of the variable (num, text)
    std::string varname = vnameNode->getChildren()[0]->getActualValue(); // the actual variable name

    // check if variable is already declared in the symbol table
    if (symbolTable->lookup(varname) != std::nullopt)
    {
        throw TypeError("Variable '" + varname + "' was already declared", filename, vnameNode->getChildren()[0]->getLineNumber());
    }

    // add the variable and its type to the symbol table
    Symbol symbol(varname, type);
    symbolTable->bind(symbol);

    // handle the rest of the global variables recursively (if any)
    if (globVarsNode->getChildren().size() > 2)
    {
        SyntaxTreeNode *nextGlobvarsNode = globVarsNode->getChildren()[3]; // GLOBVARS (recursive part)
        checkGlobVars(nextGlobvarsNode);
    }
}

void TypeChecker::checkAlgo(SyntaxTreeNode *algoNode)
{
    // ALGO -> begin INSTRUC end

    // enter the new scope for the ALGO block
    symbolTable->enter();

    for (auto child : algoNode->getChildren())
    {
        if (child->getSymbol() == "INSTRUC")
        {
            checkInstruc(child);
        }
    }

    // exit the scope after processing the ALGO block
    symbolTable->exit();
}

void TypeChecker::checkInstruc(SyntaxTreeNode *instrucNode)
{
    // INSTRUC -> '' | COMMAND ; INSTRUC

    // base case: if the node is epsilon (empty), stop processing
    if (instrucNode->getChildren().empty())
    {
        return;
    }

    for (auto child : instrucNode->getChildren())
    {
        if (child->getSymbol() == "COMMAND")
        {
            checkCommand(child);
        }
        else if (child->getSymbol() == "INSTRUC")
        {
            // handle the rest of the instructions recursively (if any)
            checkInstruc(child);
        }
    }
}

void TypeChecker::checkCommand(SyntaxTreeNode *node)
{
    // COMMAND -> skip | halt | print ATOMIC | ASSIGN | CALL | BRANCH | return ATOMIC
    std::string commandType = node->getChildren()[0]->getSymbol();

    if (commandType == "skip")
    {
        return; // nothing to check for skip command
    }
    else if (commandType == "halt")
    {
        return; // nothing to check for halt command
    }
    else if (commandType == "print")
    {
        checkAtomic(node->getChildren()[1]);
    }
    else if (commandType == "ASSIGN")
    {
        checkAssign(node->getChildren()[0]);
    }
    else if (commandType == "CALL")
    {
        checkCall(node->getChildren()[0]);
    }
    else if (commandType == "BRANCH")
    {
        checkBranch(node->getChildren()[0]);
    }
    else if (commandType == "return")
    {
        checkAtomic(node->getChildren()[1]);
    }
}

std::string TypeChecker::checkAtomic(SyntaxTreeNode *node)
{
    // ATOMIC -> VNAME | CONST
    std::string atomicType = node->getChildren()[0]->getSymbol();

    if (atomicType == "VNAME")
    {
        // check if the variable name exists in the symbol table
        std::string varName = node->getChildren()[0]->getChildren()[0]->getActualValue();
        if (symbolTable->lookup(varName) == std::nullopt)
        {
            throw TypeError("Undefined variable " + varName);
        }

        return symbolTable->lookup(varName).value().type(); // return the type of the variable
    }
    else if (atomicType == "CONST")
    {
        // check if it's a valid constant (either numliteral or textliteral)
        std::string constType = node->getChildren()[0]->getChildren()[0]->getSymbol();
        if (constType == "numliteral")
        {
            return "num";
        }
        else if (constType == "textliteral")
        {
            return "text";
        }
        else
        {
            throw TypeError("Invalid constant type " + constType);
        }
    }
    else
    {
        // if it's not VNAME or CONST, it's an invalid atomic expression
        throw TypeError("Invalid atomic expression " + atomicType);
    }
}

void TypeChecker::checkAssign(SyntaxTreeNode *node)
{
    // ASSIGN -> VNAME <input | VNAME = TERM
    std::string varname = node->getChildren()[0]->getChildren()[0]->getActualValue(); // get actual variable name

    // ensure the variable is declared in the symbol table
    auto varSymbol = symbolTable->lookup(varname);
    if (varSymbol == std::nullopt)
    {
        if (node->getChildren()[1]->getSymbol() == "<input")
        {
            throw TypeError("Undeclared variable '" + varname + "' assigned a value", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }
        else
        {
            throw TypeError("Undeclared variable '" + varname + "' assigned a value", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }
    }

    std::string assignOperator = node->getChildren()[1]->getSymbol();

    // case 1: handle input from user at runtime (numeric)
    if (assignOperator == "<input")
    {
        // check if the variable is of numeric type
        if (varSymbol.value().type() != "num")
        {
            throw TypeError("'" + varname + "' must be of numeric type to accept input", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }
    }
    // case 2: handle standard assignment (VNAME = TERM)
    else if (assignOperator == "=")
    {
        // retrieve the term being assigned
        SyntaxTreeNode *termNode = node->getChildren()[2];

        // perform type checking on the term and get its type
        std::string termType = checkTerm(termNode);

        // compare the type of the variable and the term
        if (varSymbol->type() != termType)
        {
            throw TypeError("Cannot assign value of type '" + termType + "' to variable '" + varname + "' of type '" + varSymbol->type() + "'", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }
    }
}

std::optional<std::string> TypeChecker::checkCall(SyntaxTreeNode *node)
{
    // CALL -> FNAME ( ATOMIC , ATOMIC , ATOMIC )
    std::string functionName = node->getChildren()[0]->getChildren()[0]->getActualValue(); // get the function name

    // check if the function exists in the symbol table
    auto functionSymbol = symbolTable->lookup(functionName);
    if (!functionSymbol)
    {
        throw TypeError("Undeclared function '" + functionName + "' called", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
    }

    // // verify that the symbol is indeed a function
    // if (functionSymbol.value().type() != "fname")
    // {
    //     throw TypeError("'" + functionName + "' is not a function", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
    // }

    std::vector<std::string> argTypes; // store the types of the arguments
    for (auto child : node->getChildren())
    {
        if (child->getSymbol() == "ATOMIC")
        {
            std::string atomicType = checkAtomic(child); // check and get the type of the argument
            argTypes.push_back(atomicType);
        }
    }

    // check the number and types of arguments
    checkFunctionArguments(functionSymbol.value(), argTypes, node->getChildren()[0]->getChildren()[0]->getLineNumber());

    // return the function type (if any)
    return functionSymbol.value().type();
}

void TypeChecker::checkFunctionArguments(const Symbol &functionSymbol, std::vector<std::string> argTypes, int lineNumber)
{
    std::vector<std::string> expectedParamTypes = functionSymbol.getParamTypes();

    if (argTypes.size() != expectedParamTypes.size())
    {
        throw TypeError("Incorrect number of arguments in function call to '" + functionSymbol.name() + "'. Expected " + std::to_string(expectedParamTypes.size()) + " but got " + std::to_string(argTypes.size()), filename, lineNumber);
    }

    // check if argument types match expected types
    for (size_t i = 0; i < argTypes.size(); ++i)
    {
        if (argTypes[i] != expectedParamTypes[i])
        {
            throw TypeError("Incorrect type for argument " + std::to_string(i + 1) + " in call to '" + functionSymbol.name() + "'. Expected " + expectedParamTypes[i] + " but got " + argTypes[i], filename, lineNumber);
        }
    }
}

void TypeChecker::checkBranch(SyntaxTreeNode *node)
{
    // BRANCH -> if COND then ALGO else ALGO
    SyntaxTreeNode *condNode = node->getChildren()[1];     // COND (condition)
    SyntaxTreeNode *thenAlgoNode = node->getChildren()[3]; // 1st ALGO (then block)
    SyntaxTreeNode *elseAlgoNode = node->getChildren()[5]; // 2nd ALGO (else block)

    if (condNode->getSymbol() == "COND")
    {
        // check the condition
        checkCond(condNode);
    }
    else
    {
        throw TypeError("Invalid condition type '" + condNode->getSymbol() + "'.", filename, condNode->getLineNumber());
    }

    if (thenAlgoNode->getSymbol() == "ALGO")
    {
        // enter the "then" scope
        symbolTable->enter();
        checkAlgo(thenAlgoNode);
        symbolTable->exit();
    }
    else
    {
        throw TypeError("Invalid 'then' block type '" + thenAlgoNode->getSymbol() + "'.", filename, thenAlgoNode->getLineNumber());
    }

    if (elseAlgoNode->getSymbol() == "ALGO")
    {
        // enter the "else" scope
        symbolTable->enter();
        checkAlgo(elseAlgoNode);
        symbolTable->exit();
    }
    else
    {
        throw TypeError("Invalid 'else' block type '" + elseAlgoNode->getSymbol() + "'.", filename, elseAlgoNode->getLineNumber());
    }
}

std::string TypeChecker::checkTerm(SyntaxTreeNode *node)
{
    // NOTE: function must return the type of the term

    // TERM -> ATOMIC | CALL | OP
    std::string termType = node->getChildren()[0]->getSymbol();

    if (termType == "ATOMIC")
    {
        return checkAtomic(node->getChildren()[0]);
    }
    else if (termType == "CALL")
    {
        std::optional<std::string> callReturnType = checkCall(node->getChildren()[0]);
        if (callReturnType.has_value())
        {
            return callReturnType.value();
        }
        else
        {
            throw TypeError("Invalid return type for function call.", filename, node->getChildren()[0]->getLineNumber());
        }
    }
    else if (termType == "OP")
    {
        return checkOp(node->getChildren()[0]);
    }
    else
    {
        throw TypeError("Invalid term type '" + termType + "'.");
    }
}

std::string TypeChecker::checkOp(SyntaxTreeNode *node)
{
    // OP -> UNOP ( ARG ) | BINOP ( ARG , ARG )
    SyntaxTreeNode *opTypeNode = node->getChildren()[0];

    if (opTypeNode->getSymbol() == "UNOP")
    {
        // unary operation
        return checkUnop(opTypeNode, node->getChildren()[2]);
    }
    else if (opTypeNode->getSymbol() == "BINOP")
    {
        // binary operation
        return checkBinop(opTypeNode, node->getChildren()[2], node->getChildren()[4]);
    }
    else
    {
        throw TypeError("Invalid operation type.");
    }
}

std::string TypeChecker::checkCond(SyntaxTreeNode *node)
{
    // COND -> SIMPLE | COMPOSIT
    if (node->getChildren()[0]->getSymbol() == "SIMPLE")
    {
        return checkSimple(node->getChildren()[0]); // check simple condition
    }
    else if (node->getChildren()[0]->getSymbol() == "COMPOSIT")
    {
        return checkComposit(node->getChildren()[0]); // check composite condition
    }
    else
    {
        throw TypeError("Invalid condition type.");
    }
}

std::string TypeChecker::checkSimple(SyntaxTreeNode *node)
{
    // SIMPLE -> BINOP ( ATOMIC , ATOMIC )
    std::string binOp = node->getChildren()[0]->getChildren()[0]->getSymbol(); // BINOP
    SyntaxTreeNode *leftAtomicNode = node->getChildren()[2];                   // 1st ATOMIC
    SyntaxTreeNode *rightAtomicNode = node->getChildren()[4];                  // 2nd ATOMIC

    // check the types of both atomic expressions
    std::string leftType = checkAtomic(leftAtomicNode);
    std::string rightType = checkAtomic(rightAtomicNode);

    // for binary operations, both ATOMIC values must be of the same type
    if (leftType != rightType)
    {
        throw TypeError("Incompatible types for binary operator '" + binOp + "'.", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
    }

    // ensure the BINOP is valid for the types (in this case, assume both are numeric or comparable)
    if (binOp == "add" || binOp == "sub" || binOp == "mul" || binOp == "div")
    {
        if (leftType != "num" || rightType != "num")
        {
            throw TypeError("Arithmetic operator '" + binOp + "' requires numeric operands.", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }
    }
    else if (binOp == "eq" || binOp == "grt")
    {
        // comparison operators 'eq' (equality) and 'grt' (greater than) should work with the same types
        if (leftType != rightType)
        {
            throw TypeError("Comparison operator '" + binOp + "' requires operands of the same type.", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }
    }
    else if (binOp == "or" || binOp == "and")
    {
        if (leftType != "num" || rightType != "num")
        {
            throw TypeError("Logical operator '" + binOp + "' requires numeric (boolean-like) operands.", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }
    }
    else
    {
        throw TypeError("Unknown binary operator type '" + binOp + "'.");
    }

    return "num"; // conditions ultimately return a numeric type (1 for true, 0 for false)
}

std::string TypeChecker::checkComposit(SyntaxTreeNode *node)
{
    // COMPOSIT -> BINOP ( SIMPLE , SIMPLE ) | UNOP ( SIMPLE )
    if (node->getChildren()[0]->getSymbol() == "BINOP")
    {
        std::string binOp = node->getChildren()[0]->getChildren()[0]->getSymbol(); // BINOP
        SyntaxTreeNode *leftSimpleNode = node->getChildren()[2];                   // 1st SIMPLE
        SyntaxTreeNode *rightSimpleNode = node->getChildren()[4];                  // 2nd SIMPLE

        // recursively check both simple conditions
        std::string leftSimpleType = checkSimple(leftSimpleNode);
        std::string rightSimpleType = checkSimple(rightSimpleNode);

        // both simple conditions should return "num" (boolean-like numeric)
        if (leftSimpleType != "num" || rightSimpleType != "num")
        {
            throw TypeError("Binary operator '" + binOp + "' requires numeric (boolean) conditions.", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }

        // only logical operators (and/or) are valid between two conditions
        if (binOp != "or" && binOp != "and")
        {
            throw TypeError("Binary operator '" + binOp + "' is not valid between conditions.", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }

        return "num"; // conditions result in a numeric (boolean-like) value
    }
    else if (node->getChildren()[0]->getSymbol() == "UNOP")
    {
        std::string unOp = node->getChildren()[0]->getChildren()[0]->getSymbol(); // UNOP
        SyntaxTreeNode *simpleNode = node->getChildren()[2];                      // SIMPLE

        std::string simpleNodeType = checkSimple(simpleNode); // recursively check the simple condition

        // the result of a simple condition should be numeric (boolean-like)
        if (simpleNodeType != "num")
        {
            throw TypeError("Unary operator '" + unOp + "' requires a numeric (boolean-like) condition", filename, node->getChildren()[0]->getChildren()[0]->getLineNumber());
        }

        // only 'not' is a valid unary operator for conditions
        if (unOp != "not")
        {
            throw TypeError("Unknown unary operator type '" + unOp + "' for condition");
        }

        return "num"; // result of a unary operation on a condition is also numeric (boolean-like)
    }
    else
    {
        throw TypeError("Invalid composite condition type");
    }
}

std::string TypeChecker::checkUnop(SyntaxTreeNode *node, SyntaxTreeNode *argNode)
{
    // UNOP -> not | sqrt
    std::string operatorValue = node->getChildren()[0]->getSymbol(); // UNOP (the operator value e.g 'not', 'sqrt')

    std::string argType = checkArg(argNode); // check the type of the argument

    if (operatorValue == "not")
    {
        if (argType != "num")
        {
            throw TypeError("'not' operation requires a numeric argument.", filename, node->getChildren()[0]->getLineNumber());
        }
        return "num"; // the result of 'not' is numeric (e.g., 1 for true, 0 for false)
    }
    else if (operatorValue == "sqrt")
    {
        if (argType != "num")
        {
            throw TypeError("'sqrt' operation requires a numeric argument.", filename, node->getChildren()[0]->getLineNumber());
        }
        return "num"; // the result of 'sqrt' is also numeric
    }
    else
    {
        throw TypeError("Unknown unary operator type '" + operatorValue + "'.");
    }
}

std::string TypeChecker::checkBinop(SyntaxTreeNode *node, SyntaxTreeNode *leftArgNode, SyntaxTreeNode *rightArgNode)
{
    // BINOP -> or | and | eq | grt | add | sub | mul | div
    std::string operatorValue = node->getChildren()[0]->getSymbol(); // (the operator value e.g 'or', 'and', etc)

    std::string leftArgType = checkArg(leftArgNode);   // type of the first argument
    std::string rightArgType = checkArg(rightArgNode); // type of the second argument

    // ensure both arguments are numeric for arithmetic operators
    if (operatorValue == "add" || operatorValue == "sub" || operatorValue == "mul" || operatorValue == "div")
    {
        if (leftArgType != "num" || rightArgType != "num")
        {
            throw TypeError("Arithmetic operator '" + operatorValue + "' require both arguments to be numeric.", filename, node->getChildren()[0]->getLineNumber());
        }
        return "num"; // arithmetic operations result in a numeric type
    }
    // ensure both arguments are the same type for logical and comparison operators
    else if (operatorValue == "or" || operatorValue == "and" || operatorValue == "eq" || operatorValue == "grt")
    {
        if (leftArgType != rightArgType)
        {
            throw TypeError("Binary operator '" + operatorValue + "' requires both arguments to be of the same type.", filename, node->getChildren()[0]->getLineNumber());
        }

        // 'or' and 'and' return numeric types, comparisons return boolean-like numeric types (1 or 0)
        if (operatorValue == "or" || operatorValue == "and")
        {
            return "num";
        }
        else
        {
            return "num"; // comparisons also return numeric values
        }
    }
    else
    {
        throw TypeError("Unknown binary operator type '" + operatorValue + "'.");
    }
}

std::string TypeChecker::checkArg(SyntaxTreeNode *node)
{
    // ARG -> ATOMIC | OP
    SyntaxTreeNode *argTypeNode = node->getChildren()[0];

    if (argTypeNode->getSymbol() == "ATOMIC")
    {
        return checkAtomic(node->getChildren()[0]); // check the type of the atomic value
    }
    else if (argTypeNode->getSymbol() == "OP")
    {
        return checkOp(node->getChildren()[1]); // check the type of the operation
    }
    else
    {
        throw TypeError("Invalid argument type.");
    }
}

void TypeChecker::checkFunctions(SyntaxTreeNode *node)
{
    // FUNCTIONS -> '' | DECL FUNCTIONS
    for (auto child : node->getChildren())
    {
        if (child->getSymbol() == "DECL")
        {
            checkDecl(child);
        }
        else if (child->getSymbol() == "FUNCTIONS")
        {
            checkFunctions(child); // recursively check the next function in the list
        }
    }
}

void TypeChecker::checkDecl(SyntaxTreeNode *node)
{
    // DECL -> HEADER BODY
    for (auto child : node->getChildren())
    {
        if (child->getSymbol() == "HEADER")
        {
            checkHeader(child); // check the function header (return type, parameter types)
        }
        else if (child->getSymbol() == "BODY")
        {
            checkBody(child); // check the function body (variable declarations, return types)
        }
    }
}

void TypeChecker::checkHeader(SyntaxTreeNode *node)
{
    // HEADER -> FTYP FNAME ( VNAME , VNAME , VNAME )
    std::string returnType = node->getChildren()[0]->getChildren()[0]->getSymbol();        // FTYP (function return type)
    std::string functionName = node->getChildren()[1]->getChildren()[0]->getActualValue(); // FNAME (function name)

    // parameter types (assuming exactly 3 parameters as specified by the grammar)
    std::vector<std::string> paramTypes;
    std::vector<int> paramIndexes = {3, 5, 7}; // indexes of the parameter symbols
    for (int m = 0; m < 3; m++)
    {
        int index = paramIndexes[m];

        std::string paramName = node->getChildren()[index]->getChildren()[0]->getActualValue(); // get the parameter name (variable name)

        // check if the parameter is already declared in the symbol table
        if (symbolTable->lookup(paramName) == std::nullopt)
        {
            throw TypeError("Parameter variable '" + paramName + "' was not declared.", filename, node->getChildren()[index]->getChildren()[0]->getLineNumber());
        }

        std::string paramType = symbolTable->lookup(paramName).value().type(); // lookup type of each parameter
        paramTypes.push_back(paramType);
    }

    // check if the function is already declared in the symbol table
    if (symbolTable->lookup(functionName) != std::nullopt)
    {
        throw TypeError("Function '" + functionName + "' was already declared", filename, node->getChildren()[1]->getChildren()[0]->getLineNumber());
    }

    Symbol functionSymbol(functionName, returnType);
    functionSymbol.setParamTypes(paramTypes); // store the parameter types in the function's symbol
    symbolTable->bind(functionSymbol);        // bind the function name in the current scope
}

void TypeChecker::checkBody(SyntaxTreeNode *node)
{
    // BODY -> PROLOG LOCVARS ALGO EPILOG SUBFUNCS end
    symbolTable->enter(); // enter a new scope for the function body

    // start by checking local variables variables first
    if (node->getChildren()[1]->getSymbol() == "LOCVARS")
    {
        // check local variables
        checkLocVars(node->getChildren()[1]);
    }

    // next, check subfunction declarations, if any
    if (node->getChildren()[4]->getSymbol() == "SUBFUNCS")
    {
        checkFunctions(node->getChildren()[4]->getChildren()[0]);
    }

    // finally, check the function algorithm block
    if (node->getChildren()[2]->getSymbol() == "ALGO")
    {
        // check the algorithm (instructions)
        checkAlgo(node->getChildren()[2]);
    }

    symbolTable->exit(); // exit the function's scope
}

void TypeChecker::checkLocVars(SyntaxTreeNode *node)
{
    // LOCVARS -> VTYP VNAME , VTYP VNAME , VTYP VNAME ,
    for (size_t i = 0; i < node->getChildren().size(); i++)
    {
        if (node->getChildren()[i]->getSymbol() == "VTYP")
        {
            std::string varType = node->getChildren()[i]->getChildren()[0]->getSymbol();
            std::string varName = node->getChildren()[i + 1]->getChildren()[0]->getActualValue(); // (local variable name)

            // check if variable is already declared in the symbol table
            if (symbolTable->lookup(varName) != std::nullopt)
            {
                throw TypeError("Variable '" + varName + "' was already declared", filename, node->getChildren()[i + 1]->getChildren()[0]->getLineNumber());
            }

            Symbol varSymbol(varName, varType); // create a symbol for the variable
            symbolTable->bind(varSymbol);       // bind the local variable in the current scope
        }
    }
}
