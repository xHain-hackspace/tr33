#pragma once

#include <cstdlib>
#include <stack>
#include <cctype>
#include <cstring>
#include <tuple>
#include <fpm_fixed.hpp>
#include <fpm_math.hpp>

enum ExprType
{
    EXPR_NUMBER,
    EXPR_BINOP,
    EXPR_VAR,
    EXPR_FUNC,
};

enum BinOpType
{
    BINOP_POW,
    BINOP_MOD,
    BINOP_ADD,
    BINOP_SUB,
    BINOP_MUL,
    BINOP_DIV,
    BINOP_LSHIFT,
    BINOP_RSHIFT,
    BINOP_LTE,
    BINOP_GTE,
    BINOP_LT,
    BINOP_GT,
    BINOP_EQ,
    BINOP_NEQ,
    BINOP_OR,
    BINOP_BIT_OR,
    BINOP_AND,
    BINOP_BIT_AND,
    BINOP_BIT_XOR
};

enum Var
{
    VAR_T,
    VAR_I,
    VAR_X,
    VAR_Y,
    VAR_PI,
    VAR_TAU,
};

enum FuncType
{
    FUNC_RAND,
    FUNC_RANDOM,
    FUNC_SIN,
    FUNC_COS,
    FUNC_TAN,
    FUNC_ASIN,
    FUNC_ACOS,
    FUNC_ATAN,
    FUNC_ATAN2,
    FUNC_ASINH,
    FUNC_ACOSH,
    FUNC_ATANH,
    FUNC_FLOOR,
    FUNC_CEIL,
    FUNC_ROUND,
    FUNC_FRACT,
    FUNC_TRUNC,
    FUNC_HYPOT,
};

using fpm::fixed_16_16;

struct Expr
{
    ExprType type;
    union
    {
        float number;
        Var var;
        struct
        {
            BinOpType op;
            Expr *a;
            Expr *b;
        } binop;
        struct
        {
            FuncType func;
            Expr *args[2];
            size_t arity;
        } funcCall;
    };
} typedef Expr;

template <size_t desired_capacity>
class PixelFun
{
private:
    Expr pool[desired_capacity];
    size_t freeIndices[desired_capacity];
    size_t stackTop;
    Expr *root;

public:
    PixelFun() : pool(), stackTop(desired_capacity), freeIndices(), root(nullptr)
    {
        for (size_t i = 0; i < desired_capacity; i++)
        {
            freeIndices[i] = i;
        }
    }

    bool parse(const char *expr)
    {
        if (root != nullptr)
        {
            dealloc();
        }
        const char *rest = parseExpr(expr, root);
        if (rest && *rest == '\0')
        {
            return true;
        }
        dealloc();
        return false;
    }

    fixed_16_16 eval(fixed_16_16 t, fixed_16_16 i, fixed_16_16 x, fixed_16_16 y)
    {
        return eval(root, t, i, x, y);
    }

    void printAST()
    {
        printAST(root, 0);
    }

private:
    Expr *alloc(ExprType exprType)
    {
        if (stackTop == 0)
        {
            Serial.println("Out of memory");
            return nullptr;
        }

        size_t idx = freeIndices[--stackTop];
        pool[idx].type = exprType;
        return &pool[idx];
    }

    void dealloc()
    {
        for (size_t i = 0; i < desired_capacity; i++)
        {
            freeIndices[i] = i;
        }
        stackTop = desired_capacity;
        root = nullptr;
    }

    void dealloc(const Expr *expr)
    {
        if (!expr)
        {
            return;
        }
        size_t idx = expr - pool;
        freeIndices[stackTop++] = idx;
    }

    fixed_16_16 eval(Expr *expr, fixed_16_16 t, fixed_16_16 i, fixed_16_16 x, fixed_16_16 y)
    {
        if (!expr)
        {
            return fixed_16_16(0);
        }

        switch (expr->type)
        {
        case EXPR_NUMBER:
            return fixed_16_16(expr->number);
        case EXPR_VAR:
            switch (expr->var)
            {
            case VAR_T:
                return t;
            case VAR_I:
                return i;
            case VAR_X:
                return x;
            case VAR_Y:
                return y;
            case VAR_PI:
                return fixed_16_16(PI);
            case VAR_TAU:
                return fixed_16_16(PI) * 2;
            }
        case EXPR_FUNC:
            switch (expr->funcCall.func)
            {
            case FUNC_RAND:
            case FUNC_RANDOM:
                return fixed_16_16(random(RAND_MAX)) / fixed_16_16(RAND_MAX);
            case FUNC_SIN:
                return fpm::sin(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_COS:
                return fpm::cos(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_TAN:
                return fpm::tan(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_ASIN:
                return fpm::asin(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_ACOS:
                return fpm::acos(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_ATAN:
                return fpm::atan(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_ATAN2:
                return fpm::atan2(eval(expr->funcCall.args[0], t, i, x, y),
                                  eval(expr->funcCall.args[1], t, i, x, y));
            case FUNC_FLOOR:
                return fpm::floor(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_CEIL:
                return fpm::ceil(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_ROUND:
                return fpm::round(eval(expr->funcCall.args[0], t, i, x, y));
            case FUNC_FRACT:
            {
                auto arg = eval(expr->funcCall.args[0], t, i, x, y);
                return arg - fpm::trunc(arg);
            }
            case FUNC_TRUNC:
                return fpm::trunc(eval(expr->funcCall.args[0], t, i, x, y));

            case FUNC_HYPOT:
                fixed_16_16 a = eval(expr->funcCall.args[0], t, i, x, y);
                fixed_16_16 b = eval(expr->funcCall.args[1], t, i, x, y);
                return fpm::sqrt(a * a + b * b);
            }
        case EXPR_BINOP:
            fixed_16_16 lhs = eval(expr->binop.a, t, i, x, y);
            fixed_16_16 rhs = eval(expr->binop.b, t, i, x, y);
            switch (expr->binop.op)
            {
            case BINOP_POW:
                return fpm::pow(lhs, rhs);
            case BINOP_MOD:
                return fpm::fmod(lhs, rhs);
            case BINOP_ADD:
                return lhs + rhs;
            case BINOP_SUB:
                return lhs - rhs;
            case BINOP_MUL:
                return lhs * rhs;
            case BINOP_DIV:
                if (rhs == fixed_16_16(0))
                {
                    return fixed_16_16(0);
                }
                return lhs / rhs;
                // todo
                //     case BINOP_LSHIFT:
                //         return (float)((int)lhs << (int)rhs);
                //     case BINOP_RSHIFT:
                //         return (float)((int)lhs >> (int)rhs);
                //     case BINOP_LTE:
                //         return lhs <= rhs ? 1 : fixed_16_16(0);
                //     case BINOP_GTE:
                //         return lhs >= rhs ? 1 : fixed_16_16(0);
                //     case BINOP_LT:
                //         return lhs < rhs ? 1 : fixed_16_16(0);
                //     case BINOP_GT:
                //         return lhs > rhs ? 1 : fixed_16_16(0);
                //     case BINOP_EQ:
                //         return lhs == rhs ? 1 : fixed_16_16(0);
                //     case BINOP_NEQ:
                //         return lhs != rhs ? 1 : fixed_16_16(0);
                //     case BINOP_OR:
                //         return (lhs == 1 || rhs == 1) ? 1 : fixed_16_16(0);
                //     case BINOP_BIT_OR:
                //         return (float)((int)lhs | (int)rhs);
                //     case BINOP_AND:
                //         return (lhs == 1 && rhs == 1) ? 1 : fixed_16_16(0);
                //     case BINOP_BIT_AND:
                //         return (float)((int)lhs & (int)rhs);
                //     case BINOP_BIT_XOR:
                //         return (float)((int)lhs ^ (int)rhs);
            }
        }

        return fixed_16_16(0);
    }

    const char *parseExpr(const char *input, Expr *&node)
    {
        input = parseLogical(input, node);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            while (*input && isspace(*input))
            {
                input++;
            }

            BinOpType op;
            const auto nextInput = parseBinOp(input, op);
            if (!nextInput || (op != BINOP_ADD && op != BINOP_SUB))
            {
                break;
            }

            input = nextInput;
            while (*input && isspace(*input))
            {
                input++;
            }

            Expr *rhs = nullptr;
            input = parseLogical(input, rhs);
            if (!input)
            {
                return nullptr;
            }

            Expr *binOp = alloc(EXPR_BINOP);
            if (!binOp)
            {
                return nullptr;
            }

            binOp->binop.op = op;
            binOp->binop.a = node;
            binOp->binop.b = rhs;
            node = binOp;
        }

        return input;
    }

    const char *parseNumber(const char *exprStr, Expr *&expr)
    {
        char *end;
        float value = strtof(exprStr, &end);
        if (end == exprStr)
        {
            return nullptr;
        }

        expr = alloc(EXPR_NUMBER);
        if (!expr)
        {
            return nullptr;
        }

        expr->number = value;
        return end;
    };

    const char *parseBinOp(const char *input, BinOpType &op)
    {
        static const struct
        {
            const char *opStr;
            BinOpType opType;
        } ops[] = {
            {"**", BINOP_POW},
            {"%", BINOP_MOD},
            {"+", BINOP_ADD},
            {"-", BINOP_SUB},
            {"*", BINOP_MUL},
            {"/", BINOP_DIV},
            {"<<", BINOP_LSHIFT},
            {">>", BINOP_RSHIFT},
            {"<=", BINOP_LTE},
            {">=", BINOP_GTE},
            {"<", BINOP_LT},
            {">", BINOP_GT},
            {"==", BINOP_EQ},
            {"!=", BINOP_NEQ},
            {"||", BINOP_OR},
            {"|", BINOP_BIT_OR},
            {"&&", BINOP_AND},
            {"&", BINOP_BIT_AND},
            {"^", BINOP_BIT_XOR},
        };
        for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++)
        {
            size_t len = strlen(ops[i].opStr);
            if (strncmp(input, ops[i].opStr, len) == 0)
            {
                op = ops[i].opType;
                return input + len;
            }
        }
        return nullptr;
    }

    const char *parseTerm(const char *input, Expr *&node)
    {
        input = parsePower(input, node);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            while (*input && isspace(*input))
            {
                input++;
            }

            BinOpType op;
            const char *nextInput = parseBinOp(input, op);
            if (!nextInput || (op != BINOP_MUL && op != BINOP_DIV && op != BINOP_MOD))
            {
                break;
            }

            input = nextInput;
            while (*input && isspace(*input))
            {
                input++;
            }

            Expr *rhs = nullptr;
            input = parsePower(input, rhs);
            if (!input)
            {
                return nullptr;
            }

            Expr *binOp = alloc(EXPR_BINOP);
            if (!binOp)
            {
                return nullptr;
            }

            binOp->binop.op = op;
            binOp->binop.a = node;
            binOp->binop.b = rhs;
            node = binOp;
        }
        return input;
    }

    const char *parsePower(const char *input, Expr *&node)
    {
        input = parsePrimary(input, node);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            const char *nextInput = strncmp(input, "**", 2) == 0 ? input + 2 : nullptr;
            if (!nextInput)
            {
                break;
            }

            while (*nextInput && isspace(*nextInput))
            {
                nextInput++;
            }

            Expr *rhs = nullptr;
            nextInput = parsePrimary(nextInput, rhs);
            if (!nextInput)
            {
                return nullptr;
            }

            Expr *powOp = alloc(EXPR_BINOP);
            if (!powOp)
            {
                return nullptr;
            }

            powOp->binop.op = BINOP_POW;
            powOp->binop.a = node;
            powOp->binop.b = rhs;
            node = powOp;
            input = nextInput;
        }

        return input;
    }

    const char *parseShift(const char *input, Expr *&node)
    {
        input = parseTerm(input, node);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            if (*input && isspace(*input))
            {
                input++;
            }

            BinOpType op;
            const char *nextInput = parseBinOp(input, op);
            if (!nextInput || (op != BINOP_LSHIFT && op != BINOP_RSHIFT))
            {
                break;
            }

            input = nextInput;
            while (*input && isspace(*input))
            {
                input++;
            }

            Expr *rhs = nullptr;
            input = parseTerm(input, rhs);
            if (!input)
            {
                return nullptr;
            }

            Expr *binOp = alloc(EXPR_BINOP);
            if (!binOp)
            {
                return nullptr;
            }

            binOp->binop.op = op;
            binOp->binop.a = node;
            binOp->binop.b = rhs;
            node = binOp;
        }

        return input;
    }

    const char *parseComparison(const char *input, Expr *&node)
    {
        input = parseShift(input, node);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            while (*input && isspace(*input))
            {
                input++;
            }

            BinOpType op;
            const char *nextInput = parseBinOp(input, op);
            if (!nextInput || (op != BINOP_LTE && op != BINOP_GTE && op != BINOP_LT && op != BINOP_GT))
            {
                break;
            }

            input = nextInput;
            while (*input && isspace(*input))
            {
                input++;
            }

            Expr *rhs = nullptr;
            input = parseShift(input, rhs);
            if (!input)
            {
                return nullptr;
            }

            Expr *binOp = alloc(EXPR_BINOP);
            if (!binOp)
            {
                return nullptr;
            }

            binOp->binop.op = op;
            binOp->binop.a = node;
            binOp->binop.b = rhs;
            node = binOp;
        }
        return input;
    }

    const char *parseEquality(const char *input, Expr *&node)
    {
        input = parseComparison(input, node);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            while (*input && isspace(*input))
            {
                input++;
            }

            BinOpType op;
            const char *nextInput = parseBinOp(input, op);
            if (!nextInput || (op != BINOP_EQ && op != BINOP_NEQ))
            {
                break;
            }

            input = nextInput;
            while (*input && isspace(*input))
            {
                input++;
            }

            Expr *rhs = nullptr;
            input = parseComparison(input, rhs);
            if (!input)
            {
                return nullptr;
            }

            Expr *binOp = alloc(EXPR_BINOP);
            if (!binOp)
            {
                return nullptr;
            }

            binOp->binop.op = op;
            binOp->binop.a = node;
            binOp->binop.b = rhs;
            node = binOp;
        }
        return input;
    }

    const char *parseBitwise(const char *input, Expr *&node)
    {
        input = parseEquality(input, node);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            while (*input && isspace(*input))
            {
                input++;
            }

            BinOpType op;
            const char *nextInput = parseBinOp(input, op);
            if (!nextInput || (op != BINOP_BIT_OR && op != BINOP_BIT_AND && op != BINOP_BIT_XOR))
            {
                break;
            }

            input = nextInput;
            while (*input && isspace(*input))
            {
                input++;
            }

            Expr *rhs = nullptr;
            input = parseEquality(input, rhs);
            if (!input)
            {
                return nullptr;
            }

            Expr *binOp = alloc(EXPR_BINOP);
            if (!binOp)
            {
                return nullptr;
            }

            binOp->binop.op = op;
            binOp->binop.a = node;
            binOp->binop.b = rhs;
            node = binOp;
        }
        return input;
    }

    const char *parseLogical(const char *input, Expr *&expr)
    {
        input = parseBitwise(input, expr);
        if (!input)
        {
            return nullptr;
        }

        while (true)
        {
            while (*input && isspace(*input))
            {
                input++;
            }

            BinOpType op;
            const char *nextInput = parseBinOp(input, op);
            if (!nextInput || (op != BINOP_OR && op != BINOP_AND))
            {
                break;
            }

            input = nextInput;
            while (*input && isspace(*input))
            {
                input++;
            }

            Expr *rhs = nullptr;
            input = parseBitwise(input, rhs);
            if (!input)
            {
                return nullptr;
            }

            Expr *binOp = alloc(EXPR_BINOP);
            if (!binOp)
            {
                return nullptr;
            }

            binOp->binop.op = op;
            binOp->binop.a = expr;
            binOp->binop.b = rhs;
            expr = binOp;
        }
        return input;
    }

    const char *parsePrimary(const char *input, Expr *&node)
    {
        while (*input && isspace(*input))
        {
            input++;
        }
        if (*input == '(')
        {
            input++;
            input = parseExpr(input, node);
            if (!input || *input != ')')
            {
                return nullptr;
            }
            return input + 1;
        }
        else if (isAlpha(*input))
        {
            const char *rest = parseFunction(input, node);
            if (rest)
            {
                return rest;
            }
            return parseIdentifier(input, node);
        }
        return parseNumber(input, node);
    }

    const char *parseIdentifier(const char *input, Expr *&node)
    {
        static const struct
        {
            const char *name;
            Var var;
        } vars[] = {
            {"t", VAR_T},
            {"i", VAR_I},
            {"x", VAR_X},
            {"y", VAR_Y},
            {"pi", VAR_PI},
            {"tau", VAR_TAU},
        };

        for (size_t i = 0; i < sizeof(vars) / sizeof(vars[0]); i++)
        {
            size_t len = strlen(vars[i].name);
            if (strncmp(input, vars[i].name, len) == 0)
            {
                node = alloc(EXPR_VAR);
                if (!node)
                {
                    return nullptr;
                }
                node->var = vars[i].var;
                return input + len;
            }
        }

        return nullptr;
    }

    const char *parseFunction(const char *input, Expr *&node)
    {
        static const struct
        {
            const char *name;
            FuncType func;
            size_t arity;
        } funcs[]{
            {"rand", FUNC_RAND, 0},
            {"random", FUNC_RANDOM, 0},
            {"sin", FUNC_SIN, 1},
            {"cos", FUNC_COS, 1},
            {"tan", FUNC_TAN, 1},
            {"asin", FUNC_ASIN, 1},
            {"acos", FUNC_ACOS, 1},
            {"atan", FUNC_ATAN, 1},
            {"atan2", FUNC_ATAN2, 2},
            {"asinh", FUNC_ASINH, 1},
            {"acosh", FUNC_ACOSH, 1},
            {"atanh", FUNC_ATANH, 1},
            {"floor", FUNC_FLOOR, 1},
            {"ceil", FUNC_CEIL, 1},
            {"round", FUNC_ROUND, 1},
            {"fract", FUNC_FRACT, 1},
            {"hypot", FUNC_HYPOT, 2}};

        for (size_t i = 0; i < sizeof(funcs) / sizeof(funcs[0]); i++)
        {
            size_t len = strlen(funcs[i].name);
            if (strncmp(input, funcs[i].name, len) == 0 && input[len] == '(')
            {
                node = alloc(EXPR_FUNC);
                if (!node)
                {
                    return nullptr;
                }
                node->funcCall.func = funcs[i].func;
                node->funcCall.arity = funcs[i].arity;
                input += len + 1;

                for (size_t arg = 0; arg < funcs[i].arity; arg++)
                {
                    if (arg > 0)
                    {
                        if (*input != ',')
                            return nullptr; // Error: missing comma between arguments
                        input++;            // Skip comma
                    }
                    const char *nextInput = parseExpr(input, node->funcCall.args[arg]);
                    if (!nextInput)
                    {
                        return nullptr;
                    }

                    if (arg == funcs[i].arity - 1 && *nextInput != ')')
                    {
                        return nullptr;
                    }
                    if (arg < funcs[i].arity - 1 && *nextInput != ',')
                    {
                        return nullptr;
                    }

                    input = nextInput;
                }
                return input + 1;
            }
        }

        return nullptr;
    }

    void printAST(Expr *node, int indent = 0)
    {
        if (!node)
            return;

        for (int i = 0; i < indent; ++i)
        {
            Serial.print("  ");
        }

        switch (node->type)
        {
        case EXPR_NUMBER:
            Serial.print("Float: ");
            Serial.println(node->number, 6);
            break;

        case EXPR_BINOP:
            Serial.print("BinOp: ");
            switch (node->binop.op)
            {
            case BINOP_POW:
                Serial.println("POW");
                break;
            case BINOP_MOD:
                Serial.println("MOD");
                break;
            case BINOP_ADD:
                Serial.println("ADD");
                break;
            case BINOP_SUB:
                Serial.println("SUB");
                break;
            case BINOP_MUL:
                Serial.println("MUL");
                break;
            case BINOP_DIV:
                Serial.println("DIV");
                break;
            case BINOP_LSHIFT:
                Serial.println("LSHIFT");
            case BINOP_RSHIFT:
                Serial.println("RSHIFT");
                break;
            case BINOP_LTE:
                Serial.println("LTE");
                break;
            case BINOP_GTE:
                Serial.println("GTE");
                break;
            case BINOP_LT:
                Serial.println("LT");
                break;
            case BINOP_GT:
                Serial.println("GT");
                break;
            case BINOP_EQ:
                Serial.println("EQ");
                break;
            case BINOP_NEQ:
                Serial.println("NEQ");
                break;
            case BINOP_OR:
                Serial.println("OR");
                break;
            case BINOP_BIT_OR:
                Serial.println("BIT_OR");
                break;
            case BINOP_AND:
                Serial.println("AND");
                break;
            case BINOP_BIT_AND:
                Serial.println("BIT_AND");
                break;
            case BINOP_BIT_XOR:
                Serial.println("BIT_XOR");
                break;
            }
            printAST(node->binop.a, indent + 1);
            printAST(node->binop.b, indent + 1);
            break;

        case EXPR_VAR:
            Serial.print("Var: ");
            switch (node->var)
            {
            case VAR_T:
                Serial.println("T");
                break;
            case VAR_I:
                Serial.println("I");
                break;
            case VAR_X:
                Serial.println("X");
                break;
            case VAR_Y:
                Serial.println("Y");
                break;
            case VAR_PI:
                Serial.println("PI");
                break;
            case VAR_TAU:
                Serial.println("TAU");
                break;
            }
            break;

        case EXPR_FUNC:
            Serial.print("Func: ");
            switch (node->funcCall.func)
            {
            case FUNC_RAND:
            case FUNC_RANDOM:
                Serial.println("RANDOM");
                break;
            case FUNC_SIN:
                Serial.println("SIN");
                break;
            case FUNC_COS:
                Serial.println("COS");
                break;
            case FUNC_TAN:
                Serial.println("TAN");
                break;
            case FUNC_ASIN:
                Serial.println("ASIN");
                break;
            case FUNC_ACOS:
                Serial.println("ACOS");
                break;
            case FUNC_ATAN:
                Serial.println("ATAN");
                break;
            case FUNC_ATAN2:
                Serial.println("ATAN2");
                break;
            case FUNC_ASINH:
                Serial.println("ASINH");
                break;
            case FUNC_ACOSH:
                Serial.println("ACOSH");
                break;
            case FUNC_ATANH:
                Serial.println("ATANH");
                break;
            case FUNC_FLOOR:
                Serial.println("FLOOR");
                break;
            case FUNC_CEIL:
                Serial.println("CEIL");
                break;
            case FUNC_ROUND:
                Serial.println("ROUND");
                break;
            case FUNC_FRACT:
                Serial.println("FRACT");
                break;
            case FUNC_TRUNC:
                Serial.println("TRUNC");
                break;
            case FUNC_HYPOT:
                Serial.println("HYPOT");
                break;
            }
            for (size_t i = 0; i < node->funcCall.arity; ++i)
            {
                printAST(node->funcCall.args[i], indent + 1);
            }
            break;
        }
    }
};
