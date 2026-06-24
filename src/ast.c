#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

NodeAST *createNode(NodeType type)
{

    NodeAST *newNode = malloc(sizeof(NodeAST));

    newNode->type = type;
    newNode->child_count = 0;
    memset(newNode->dataType, 0, sizeof(newNode->dataType));

    for (int i = 0; i < MAX_CHILDREN; i++)
    {
        newNode->children[i] = NULL;
    }
    return newNode;
}

NodeAST *createNodeNum(int value)
{
    NodeAST *newNode = createNode(AST_NUM);
    newNode->value = value;
    strcpy(newNode->dataType, "int");
    return newNode;
}

NodeAST *createNodeFloat(float value)
{
    NodeAST *newNode = createNode(AST_FLOAT);
    newNode->floatValue = value;
    strcpy(newNode->dataType, "float");
    return newNode;
}

NodeAST *createNodeId(char *name)
{
    NodeAST *newNode = createNode(AST_ID);
    strcpy(newNode->name, name);
    const char *t = getSymbolType(name);

    if (t)
        strcpy(newNode->dataType, t);
    else
        strcpy(newNode->dataType, "undefined");

    return newNode;
}

NodeAST *createNodeBinOp(char *op, NodeAST *left, NodeAST *right)
{
    NodeAST *newNode = createNode(AST_BINOP);
    strcpy(newNode->op, op);

    addChild(newNode, left);
    addChild(newNode, right);

    // Operações aritméticas
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
    {
        if (!isNumeric(left->dataType) || !isNumeric(right->dataType))
        {
            fprintf(stderr, "[SEMANTIC ERROR] operação aritmética inválida\n");

            strcpy(newNode->dataType, "error");
            return newNode;
        }

        if (strcmp(left->dataType, "float") == 0 || strcmp(right->dataType, "float") == 0)
            strcpy(newNode->dataType, "float");
        else
            strcpy(newNode->dataType, "int");
    }

    // Operadores relacionais
    else if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 || strcmp(op, "==") == 0 || strcmp(op, "!=") == 0)
    {

        if (!isNumeric(left->dataType) || !isNumeric(right->dataType))
        {
            fprintf(stderr, "[SEMANTIC ERROR] comparação inválida\n");

            strcpy(newNode->dataType, "error");
            return newNode;
        }
        strcpy(newNode->dataType, "int");
    }

    // Operadores lógicos
    else if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0)
    {
        if (!isBooleanCompatible(left->dataType) || !isBooleanCompatible(right->dataType))
        {
            fprintf(stderr, "\n[SEMANTIC ERROR] operação lógica inválida");

            strcpy(newNode->dataType, "error");
            return newNode;
        }
        strcpy(newNode->dataType, "int");
    }
    return newNode;
}

NodeAST *createNodeUnOp(char *op, NodeAST *left)
{
    NodeAST *newNode = createNode(AST_UNOP);
    strcpy(newNode->op, op);

    addChild(newNode, left);

    if (left)
    {
        strcpy(newNode->dataType, left->dataType);
    }

    return newNode;
}

NodeAST *createNodeSeq(NodeAST *first, NodeAST *second)
{
    if (!first)
        return second;

    NodeAST *newNode = createNode(AST_SEQ);

    addChild(newNode, first);
    addChild(newNode, second);

    return newNode;
}

NodeAST *createNodeIf(NodeAST *cond, NodeAST *if_body, NodeAST *else_body)
{
    NodeAST *newNode = createNode(AST_IF);

    addChild(newNode, cond);
    addChild(newNode, if_body);

    if (else_body)
        addChild(newNode, else_body);

    return newNode;
}

NodeAST *createNodeFor(NodeAST *init, NodeAST *cond, NodeAST *step, NodeAST *body)
{
    NodeAST *newNode = createNode(AST_FOR);

    addChild(newNode, init);
    addChild(newNode, cond);
    addChild(newNode, step);
    addChild(newNode, body);

    return newNode;
}

NodeAST *createNodeWhile(NodeAST *cond, NodeAST *body)
{
    NodeAST *newNode = createNode(AST_WHILE);

    addChild(newNode, cond);
    addChild(newNode, body);

    return newNode;
}

NodeAST *createNodeDoWhile(NodeAST *body, NodeAST *cond)
{
    NodeAST *newNode = createNode(AST_DO_WHILE);

    addChild(newNode, body);
    addChild(newNode, cond);

    return newNode;
}

NodeAST *createNodeAssign(NodeAST *id, NodeAST *value)
{
    NodeAST *newNode = createNode(AST_ASSIGN);

    addChild(newNode, id);
    addChild(newNode, value);

    return newNode;
}

NodeAST *createNodeDecl(char *type, NodeAST *id, NodeAST *value)
{
    NodeAST *newNode = createNode(AST_DECL);
    strcpy(newNode->op, type);

    addChild(newNode, id);

    if (value)
        addChild(newNode, value);

    return newNode;
}

int isNumeric(char *type)
{
    return strcmp(type, "int") == 0 || strcmp(type, "float") == 0;
}

int isBooleanCompatible(char *type)
{
    return strcmp(type, "int") == 0;
}

int isConditionValid(NodeAST *expr)
{
    return strcmp(expr->dataType, "int") == 0;
}

int isAssignable(const char *lhs, const char *rhs)
{
    if (strcmp(rhs, "error") == 0)
        return 0;

    if (strcmp(lhs, rhs) == 0)
        return 1;

    if (strcmp(lhs, "float") == 0 && strcmp(rhs, "int") == 0)
        return 1;

    return 0;
}

NodeAST *createNodeFunc(char *name, char *ret_type,
                        NodeAST *params,
                        NodeAST *body)
{
    NodeAST *newNode = createNode(AST_FUNC);

    strcpy(newNode->name, name);
    strcpy(newNode->op, ret_type);

    newNode->children[0] = params;
    newNode->children[1] = body;
    newNode->child_count = 2;

    return newNode;
}

NodeAST *createNodeCall(char *name, NodeAST *args)
{
    NodeAST *newNode = createNode(AST_CALL);

    strcpy(newNode->name, name);

    Symbol *func = searchFunction(name);
    if (func)
        strcpy(newNode->dataType, func->type);

    if (args)
        addChild(newNode, args);

    return newNode;
}

NodeAST *createNodeReturn(NodeAST *value)
{
    NodeAST *newNode = createNode(AST_RETURN);

    if (value)
        addChild(newNode, value);

    return newNode;
}

static NodeAST *listHead(NodeAST *node)
{
    if (!node)
        return NULL;

    if (node->type == AST_SEQ)
        return listHead(node->children[0]);

    return node;
}

static NodeAST *listTail(NodeAST *node)
{
    if (!node)
        return NULL;

    if (node->type != AST_SEQ)
        return NULL;

    NodeAST *left = node->children[0];
    NodeAST *right = node->children[1];

    if (!left)
        return NULL;

    if (left->type != AST_SEQ)
    {
        /* simplest case: node = seq(elem, rest) -> tail is rest */
        return right;
    }

    /* left is itself a seq: reduce one level: seq( left_left, left_right ), right
       we want a new sequence that represents left_right followed by right */
    NodeAST *newLeft = left->children[1];
    if (!newLeft)
        return right;

    return createNodeSeq(newLeft, right);
}

static int compareFunctionArgs(NodeAST *params,
                               NodeAST *args,
                               char *message,
                               size_t messageSize)
{
    if (!params && !args)
        return 1;

    if (!params || !args)
    {
        snprintf(message, messageSize,
                 "\n[SEMANTIC ERROR] quantidade de argumentos incompatível na chamada de função");
        return 0;
    }

    NodeAST *paramNode = listHead(params);
    NodeAST *argNode = listHead(args);

    NodeAST *nextParams = listTail(params);
    NodeAST *nextArgs = listTail(args);

    if (!paramNode || paramNode->type != AST_DECL)
    {
        snprintf(message, messageSize,
                 "\n[SEMANTIC ERROR] assinatura de função inválida");
        return 0;
    }

    if (!argNode || !argNode->dataType[0])
    {
        snprintf(message, messageSize,
                 "\n[SEMANTIC ERROR] argumento sem tipo definido na chamada de função");
        return 0;
    }

    if (strcmp(paramNode->op, argNode->dataType) != 0)
    {
        snprintf(message, messageSize,
                 "\n[SEMANTIC ERROR] tipo de argumento incompatível na chamada de função -> esperado %s, recebido %s",
                 paramNode->op,
                 argNode->dataType);
        return 0;
    }

    return compareFunctionArgs(nextParams,
                               nextArgs,
                               message,
                               messageSize);
}

int checkFunctionCallArgs(char *name,
                          NodeAST *args,
                          char *message,
                          size_t messageSize)
{
    NodeAST *funcAst = getFunctionAst(name);

    if (!funcAst)
    {
        snprintf(message,
                 messageSize,
                 "\n[SEMANTIC ERROR] função sem assinatura registrada: %s",
                 name);
        return 0;
    }

    NodeAST *params = funcAst->children[0];

    if (!compareFunctionArgs(params, args,
                             message, messageSize))
        return 0;

    return 1;
}

void addChild(NodeAST *parent, NodeAST *child)
{
    if (!parent || !child)
        return;

    if (parent->child_count < MAX_CHILDREN)
    {
        parent->children[parent->child_count++] = child;
    }
}

void printAST(NodeAST *root, int level)
{
    if (!root)
        return;

    switch (root->type)
    {
    case AST_NUM:
        printf("%d", root->value);
        break;

    case AST_FLOAT:
        printf("%f", root->floatValue);
        break;

    case AST_ID:
        printf("%s", root->name);
        break;

    case AST_BINOP:
        printf("(");
        printAST(root->children[0], level);
        printf(" %s ", root->op);
        printAST(root->children[1], level);
        printf(")");
        break;

    case AST_UNOP:
        printf("(%s", root->op);
        printAST(root->children[0], level);
        printf(")");
        break;

    case AST_SEQ:
        printAST(root->children[0], level);

        if (root->children[0] &&
            root->children[0]->type != AST_IF &&
            root->children[0]->type != AST_WHILE &&
            root->children[0]->type != AST_DO_WHILE &&
            root->children[0]->type != AST_FOR)
        {
            printf(";\n");
        }
        else
        {
            printf("\n");
        }

        printAST(root->children[1], level);
        break;
    case AST_IF:
        printf("if (");
        printAST(root->children[0], level);
        printf(") {\n");
        printAST(root->children[1], level);
        printf("\n}");

        if (root->child_count == 3)
        {
            printf(" else {\n");
            printAST(root->children[2], level);
            printf("\n}");
        }
        break;
    case AST_FUNC:
        printf("func %s() -> %s {\n", root->name, root->op);
        if (root->children[0])
        {
            printAST(root->children[0], level);
            printf("\n");
        }
        if (root->children[1])
        {
            printAST(root->children[1], level);
            printf("\n");
        }
        printf("}\n");
        break;
    case AST_RETURN:
        printf("return");
        if (root->child_count == 1)
        {
            printf(" ");
            printAST(root->children[0], level);
        }
        printf(";\n");
        break;
    case AST_WHILE:
        printf("while (");
        printAST(root->children[0], level);
        printf(") {\n");
        printAST(root->children[1], level);
        printf("\n}");
        break;
    case AST_DO_WHILE:
        printf("do {\n ");
        printAST(root->children[0], level);
        printf("\n} while (");
        printAST(root->children[1], level);
        printf(")");
        break;
    case AST_FOR:
        printf("for (");

        if (root->children[0])
            printAST(root->children[0], level);

        printf("; ");

        if (root->children[1])
            printAST(root->children[1], level);

        printf("; ");

        if (root->children[2])
            printAST(root->children[2], level);

        printf(") {\n");

        if (root->children[3])
            printAST(root->children[3], level);

        printf("\n}");
        break;
    case AST_ASSIGN:
        printf("(ASSIGN ");
        printAST(root->children[0], level);
        printf(" = ");
        printAST(root->children[1], level);
        printf(")");
        break;
    case AST_DECL:
        printf("(DECL %s ", root->op);
        printAST(root->children[0], level);

        if (root->child_count == 2)
        {
            printf(" = ");
            printAST(root->children[1], level);
        }

        printf(")");
        break;

    case AST_CALL:
        printf("%s(", root->name);

        if (root->child_count > 0)
            printAST(root->children[0], level);

        printf(")");
        break;

    case AST_BREAK:
        printf("break");
        break;

    case AST_CONTINUE:
        printf("continue");
        break;

    default:
        printf("Unknown");
        break;
    }
}

TAC *createTAC(const char *op, const char *arg1, const char *arg2, const char *result)
{
    TAC *t = malloc(sizeof(TAC));
    if (!t)
        return NULL;
    memset(t, 0, sizeof(TAC));
    if (op)
        strncpy(t->op, op, sizeof(t->op) - 1);
    if (arg1)
        strncpy(t->arg1, arg1, sizeof(t->arg1) - 1);
    if (arg2)
        strncpy(t->arg2, arg2, sizeof(t->arg2) - 1);
    if (result)
        strncpy(t->result, result, sizeof(t->result) - 1);
    t->next = NULL;
    return t;
}

TAC *insertTAC(TAC *list, TAC *instr)
{
    if (!instr)
        return list;
    if (!list)
        return instr;
    TAC *p = list;
    while (p->next)
        p = p->next;
    p->next = instr;
    return list;
}

void printTAC(TAC *list)
{
    for (TAC *p = list; p; p = p->next)
    {
        if (strcmp(p->op, "=") == 0)
        {
            printf("%s = %s\n", p->result, p->arg1);
        }
        else if (strcmp(p->op, "ret") == 0)
        {
            if (p->arg1[0])
                printf("ret %s\n", p->arg1);
            else
                printf("ret\n");
        }
        else if (strcmp(p->op, "func") == 0)
        {
            printf("func %s -> %s\n", p->arg1, p->arg2);
        }
        else if (strcmp(p->op, "fparam") == 0)
        {
            printf("fparam %s %s\n", p->arg1, p->arg2);
        }
        else if (strcmp(p->op, "param") == 0)
        {
            printf("param %s\n", p->arg1);
        }
        else if (strcmp(p->op, "call") == 0)
        {
            if (strlen(p->result) > 0)
                printf("%s = call %s, %s\n", p->result, p->arg1, p->arg2);
            else
                printf("call %s, %s\n", p->arg1, p->arg2);
        }
        else if (strcmp(p->op, "endfunc") == 0)
        {
            printf("endfunc %s\n", p->arg1);
        }
        else if (strcmp(p->op, "label") == 0)
        {
            printf("%s:\n", p->result);
        }
        else if (strcmp(p->op, "goto") == 0)
        {
            printf("goto %s\n", p->result);
        }
        else if (strcmp(p->op, "ifz") == 0)
        {
            printf("ifz %s goto %s\n", p->arg1, p->result);
        }
        else if (strlen(p->arg2) > 0)
        {
            printf("%s = %s %s %s\n", p->result, p->arg1, p->op, p->arg2);
        }
        else
        {
            /* fallback */
            printf("<tac op='%s' a1='%s' a2='%s' res='%s'>\n", p->op, p->arg1, p->arg2, p->result);
        }
    }
}

void freeTAC(TAC *list)
{
    while (list)
    {
        TAC *n = list->next;
        free(list);
        list = n;
    }
}

static int tempCounter = 0;

char *newTemp()
{
    char buffer[32];

    snprintf(buffer,
             sizeof(buffer),
             "t%d",
             tempCounter++);

    return strdup(buffer);
}

static int labelCounter = 0;

char *newLabel()
{
    char buffer[32];

    snprintf(buffer,
             sizeof(buffer),
             "L%d",
             labelCounter++);

    return strdup(buffer);
}

static char *genExprTAC(NodeAST *expr, TAC **list);

static void genArgsTAC(NodeAST *node, TAC **list, int *count)
{
    if (!node)
        return;

    if (node->type == AST_SEQ)
    {
        genArgsTAC(node->children[0], list, count);
        genArgsTAC(node->children[1], list, count);
        return;
    }

    char *value = genExprTAC(node, list);
    *list = insertTAC(*list, createTAC("param", value, "", ""));
    (*count)++;
    free(value);
}

static char *genCallTAC(NodeAST *node, TAC **list, int withResult)
{
    int count = 0;

    if (node->child_count > 0)
        genArgsTAC(node->children[0], list, &count);

    char countBuffer[16];
    snprintf(countBuffer, sizeof(countBuffer), "%d", count);

    char *result = withResult ? newTemp() : strdup("");
    *list = insertTAC(*list,
                      createTAC("call", node->name,
                                countBuffer, result));
    return result;
}

static void genFormalParamsTAC(NodeAST *node, TAC **list)
{
    if (!node)
        return;

    if (node->type == AST_SEQ)
    {
        genFormalParamsTAC(node->children[0], list);
        genFormalParamsTAC(node->children[1], list);
        return;
    }

    if (node->type == AST_DECL && node->child_count > 0)
        *list = insertTAC(*list,
                          createTAC("fparam", node->op,
                                    node->children[0]->name, ""));
}

static char *genExprTAC(NodeAST *expr, TAC **list)
{
    if (!expr)
        return strdup("");

    switch (expr->type)
    {
    case AST_NUM:
    {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", expr->value);
        return strdup(buffer);
    }
    case AST_FLOAT:
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%f", expr->floatValue);
        return strdup(buffer);
    }
    case AST_ID:
        return strdup(expr->name);

    case AST_BINOP:
    {
        char *left = genExprTAC(expr->children[0], list);
        char *right = genExprTAC(expr->children[1], list);

        char *temp = newTemp();

        TAC *instr = createTAC(
            expr->op,
            left,
            right,
            temp);

        *list = insertTAC(*list, instr);

        free(left);
        free(right);

        return temp;
    }

    case AST_UNOP:
        return strdup("");

    case AST_CALL:
        return genCallTAC(expr, list, 1);

    default:
        return strdup("");
    }
}

static TAC *genNodeTAC(NodeAST *node, TAC *list)
{
    if (!node)
        return list;

    static char *breakLabelStack[64];
    static char *continueLabelStack[64];
    static int loopDepth = 0;

    switch (node->type)
    {
    case AST_SEQ:
        list = genNodeTAC(node->children[0], list);
        list = genNodeTAC(node->children[1], list);
        return list;

    case AST_DECL:
    {

        if (node->child_count == 2)
        {

            char *value =
                genExprTAC(node->children[1], &list);

            TAC *instr =
                createTAC("=",
                          value,
                          "",
                          node->children[0]->name);

            list = insertTAC(list, instr);

            free(value);
        }

        return list;
    }
    case AST_ASSIGN:
    {
        char *value =
            genExprTAC(node->children[1], &list);

        TAC *instr =
            createTAC("=",
                      value,
                      "",
                      node->children[0]->name);

        list = insertTAC(list, instr);

        free(value);

        return list;
    }
    case AST_IF:
    {
        char *condTemp = genExprTAC(node->children[0], &list);
        char *labelFalse = newLabel();

        TAC *instrIfz = createTAC("ifz", condTemp, "", labelFalse);
        list = insertTAC(list, instrIfz);
        list = genNodeTAC(node->children[1], list);

        if (node->child_count == 3 && node->children[2] != NULL)
        {
            char *labelEnd = newLabel();

            TAC *instrGoto = createTAC("goto", "", "", labelEnd);
            list = insertTAC(list, instrGoto);

            TAC *instrLabelFalse = createTAC("label", "", "", labelFalse);
            list = insertTAC(list, instrLabelFalse);

            list = genNodeTAC(node->children[2], list);

            TAC *instrLabelEnd = createTAC("label", "", "", labelEnd);
            list = insertTAC(list, instrLabelEnd);

            free(labelEnd);
        }
        else
        {
            TAC *instrLabelFalse = createTAC("label", "", "", labelFalse);
            list = insertTAC(list, instrLabelFalse);
        }

        free(condTemp);
        free(labelFalse);

        return list;
    }

    case AST_WHILE:
    {
        char *labelStart = newLabel();
        char *labelFalse = newLabel();

        /* Armazene os rótulos (labels) alocados na pilha (stack), sem fazer um strdup adicional. */
        breakLabelStack[loopDepth] = labelFalse;
        continueLabelStack[loopDepth] = labelStart;
        loopDepth++;

        list = insertTAC(list, createTAC("label", "", "", labelStart));

        if (node->children[0])
        {
            char *condTemp = genExprTAC(node->children[0], &list);
            list = insertTAC(list, createTAC("ifz", condTemp, "", labelFalse));
            free(condTemp);
        }

        list = genNodeTAC(node->children[1], list);

        list = insertTAC(list, createTAC("goto", "", "", labelStart));

        list = insertTAC(list, createTAC("label", "", "", labelFalse));

        loopDepth--;
        free(breakLabelStack[loopDepth]);
        free(continueLabelStack[loopDepth]);
        return list;
    }

    case AST_FOR:
    {
        NodeAST *init = node->children[0];
        NodeAST *cond = node->children[1];
        NodeAST *step = node->children[2];
        NodeAST *body = node->children[3];

        if (init)
            list = genNodeTAC(init, list);
        char *labelStart = newLabel();
        char *labelFalse = newLabel();
        char *labelStep = newLabel();

        breakLabelStack[loopDepth] = labelFalse;
        continueLabelStack[loopDepth] = labelStep;
        loopDepth++;

        list = insertTAC(list, createTAC("label", "", "", labelStart));

        if (cond)
        {
            char *condTemp = genExprTAC(cond, &list);
            list = insertTAC(list, createTAC("ifz", condTemp, "", labelFalse));
            free(condTemp);
        }

        list = genNodeTAC(body, list);

        list = insertTAC(list, createTAC("label", "", "", labelStep));

        if (step)
            list = genNodeTAC(step, list);

        list = insertTAC(list, createTAC("goto", "", "", labelStart));

        list = insertTAC(list, createTAC("label", "", "", labelFalse));

        /* Libera o labelStart, que não está armazenado na pilha. */
        free(labelStart);

        loopDepth--;
        free(breakLabelStack[loopDepth]);
        free(continueLabelStack[loopDepth]);

        return list;
    }

    case AST_DO_WHILE:
    {
        NodeAST *body = node->children[0];
        NodeAST *cond = node->children[1];

        char *labelStart = newLabel();
        char *labelCond = newLabel();
        char *labelFalse = newLabel();

        breakLabelStack[loopDepth] = labelFalse;
        continueLabelStack[loopDepth] = labelCond;
        loopDepth++;

        list = insertTAC(list, createTAC("label", "", "", labelStart));

        list = genNodeTAC(body, list);

        /* Rótulo da condição: avaliar a condição aqui!! */
        list = insertTAC(list, createTAC("label", "", "", labelCond));

        if (cond)
        {
            char *condTemp = genExprTAC(cond, &list);
            list = insertTAC(list, createTAC("ifz", condTemp, "", labelFalse));
            list = insertTAC(list, createTAC("goto", "", "", labelStart));
            free(condTemp);
        }

        list = insertTAC(list, createTAC("label", "", "", labelFalse));

        /* Libere o o labelStart, que não está armazenado na pilha!!!*/
        free(labelStart);

        loopDepth--;
        free(breakLabelStack[loopDepth]);
        free(continueLabelStack[loopDepth]);

        return list;
    }

    case AST_FUNC:
        list = insertTAC(list,
                         createTAC("func", node->name,
                                   node->op, ""));
        genFormalParamsTAC(node->children[0], &list);
        list = genNodeTAC(node->children[1], list);
        list = insertTAC(list,
                         createTAC("endfunc", node->name,
                                   "", ""));
        return list;

    case AST_CALL:
    {
        char *value = genCallTAC(node, &list, 0);
        free(value);
        return list;
    }

    case AST_RETURN:
    {
        if (node->child_count == 1)
        {
            char *vbuf = genExprTAC(node->children[0], &list);
            TAC *r = createTAC("ret", vbuf ? vbuf : "", "", "");
            list = insertTAC(list, r);
            free(vbuf);
        }
        else
        {
            TAC *r = createTAC("ret", "", "", "");
            list = insertTAC(list, r);
        }
        return list;
    }
    case AST_BREAK:
    {
        if (loopDepth > 0)
        {
            list = insertTAC(
                list,
                createTAC(
                    "goto",
                    "",
                    "",
                    breakLabelStack[loopDepth - 1]));
        }

        return list;
    }
    case AST_CONTINUE:
    {
        if (loopDepth > 0)
        {
            list = insertTAC(
                list,
                createTAC(
                    "goto",
                    "",
                    "",
                    continueLabelStack[loopDepth - 1]));
        }

        return list;
    }

    default:
        return list;
    }
}

void generateTAC(NodeAST *root)
{
    TAC *list = NULL;
    list = genNodeTAC(root, list);

    /* Optionally run TAC optimizations. Use environment variable OPTIMIZE_TAC=1 to enable. */
    char *opt = getenv("OPTIMIZE_TAC");
    if (opt && strcmp(opt, "1") == 0)
    {
        TAC *opted = optimizeTAC(list);
        if (opted)
        {
            freeTAC(list);
            list = opted;
        }
    }

    printf("\n=== TAC ===\n");
    printTAC(list);
    freeTAC(list);
}

/* Helper: check if string represents integer constant */
static int isIntConst(const char *s)
{
    if (!s || !s[0])
        return 0;
    char *p = (char *)s;
    if (*p == '-' || *p == '+')
        p++;
    int has = 0;
    while (*p)
    {
        if (*p < '0' || *p > '9')
            return 0;
        has = 1;
        p++;
    }
    return has;
}

/* Very small expression evaluator for integer binary ops. Returns 1 if folded and writes result to out (allocated strdup) */
static int tryFoldBinary(const char *op, const char *a1, const char *a2, char **out)
{
    if (!isIntConst(a1) || !isIntConst(a2))
        return 0;
    long v1 = strtol(a1, NULL, 10);
    long v2 = strtol(a2, NULL, 10);
    long r = 0;
    if (strcmp(op, "+") == 0)
        r = v1 + v2;
    else if (strcmp(op, "-") == 0)
        r = v1 - v2;
    else if (strcmp(op, "*") == 0)
        r = v1 * v2;
    else if (strcmp(op, "/") == 0)
    {
        if (v2 == 0)
            return 0;
        r = v1 / v2;
    }
    else if (strcmp(op, "<") == 0)
        r = v1 < v2;
    else if (strcmp(op, ">") == 0)
        r = v1 > v2;
    else if (strcmp(op, "<=") == 0)
        r = v1 <= v2;
    else if (strcmp(op, ">=") == 0)
        r = v1 >= v2;
    else if (strcmp(op, "==") == 0)
        r = v1 == v2;
    else if (strcmp(op, "!=") == 0)
        r = v1 != v2;
    else
        return 0;
    char buf[64];
    snprintf(buf, sizeof(buf), "%ld", r);
    *out = strdup(buf);
    return 1;
}

/* Count uses of temporaries (names starting with 't') */
static int isTempName(const char *s) { return s && s[0] == 't'; }

TAC *optimizeTAC(TAC *list)
{
    if (!list)
        return NULL;
    /* First pass: constant folding. Replace binary ops on constants with = const */
    for (TAC *p = list; p; p = p->next)
    {
        if (p->op[0] != '\0' && strcmp(p->op, "=") != 0 && strcmp(p->op, "call") != 0 && strcmp(p->op, "param") != 0 && strcmp(p->op, "fparam") != 0 && strcmp(p->op, "ifz") != 0 && strcmp(p->op, "goto") != 0 && strcmp(p->op, "label") != 0 && strcmp(p->op, "ret") != 0 && strcmp(p->op, "func") != 0 && strcmp(p->op, "endfunc") != 0)
        {
            char *folded = NULL;
            if (tryFoldBinary(p->op, p->arg1, p->arg2, &folded))
            {
                /* convert to assignment */
                strncpy(p->op, "=", sizeof(p->op) - 1);
                strncpy(p->arg1, folded, sizeof(p->arg1) - 1);
                p->arg2[0] = '\0';
                free(folded);
            }
        }
    }

    /* Second pass: constant propagation for temporaries. Build map of definitions, then replace uses. */
    /* For simplicity, only propagate assignments of constants to temps: tX = CONST */
    typedef struct Def
    {
        char *name;
        char *val;
        struct Def *next;
    } Def;
    Def *defs = NULL;

    for (TAC *p = list; p; p = p->next)
    {
        if (strcmp(p->op, "=") == 0 && isTempName(p->result) && isIntConst(p->arg1) && p->arg2[0] == 0)
        {
            Def *d = malloc(sizeof(Def));
            d->name = strdup(p->result);
            d->val = strdup(p->arg1);
            d->next = defs;
            defs = d;
        }
        else if (p->arg1[0] && isTempName(p->arg1))
        {
            /* replace arg1 if we have a def */
            for (Def *d = defs; d; d = d->next)
            {
                if (strcmp(d->name, p->arg1) == 0)
                {
                    strncpy(p->arg1, d->val, sizeof(p->arg1) - 1);
                    break;
                }
            }
        }
        if (p->arg2[0] && isTempName(p->arg2))
        {
            for (Def *d = defs; d; d = d->next)
            {
                if (strcmp(d->name, p->arg2) == 0)
                {
                    strncpy(p->arg2, d->val, sizeof(p->arg2) - 1);
                    break;
                }
            }
        }
    }

    /* free defs list */
    while (defs)
    {
        Def *n = defs->next;
        free(defs->name);
        free(defs->val);
        free(defs);
        defs = n;
    }

    /* Third pass: dead code elimination for temporaries: remove assignments to temps that are never used later. Conservative single-pass mark-and-sweep.
       Compute usage counts. */
    /* Build usage map */
    typedef struct Use
    {
        char *name;
        int count;
        struct Use *next;
    } Use;
    Use *uses = NULL;
    for (TAC *p = list; p; p = p->next)
    {
        if (p->arg1[0] && isTempName(p->arg1))
        {
            Use *u = uses;
            while (u && strcmp(u->name, p->arg1) != 0)
                u = u->next;
            if (!u)
            {
                u = malloc(sizeof(Use));
                u->name = strdup(p->arg1);
                u->count = 0;
                u->next = uses;
                uses = u;
            }
            u->count++;
        }
        if (p->arg2[0] && isTempName(p->arg2))
        {
            Use *u = uses;
            while (u && strcmp(u->name, p->arg2) != 0)
                u = u->next;
            if (!u)
            {
                u = malloc(sizeof(Use));
                u->name = strdup(p->arg2);
                u->count = 0;
                u->next = uses;
                uses = u;
            }
            u->count++;
        }
    }

    /* Remove nodes that are assignments to temps with zero uses. We'll rebuild list into new list. */
    TAC *newList = NULL;
    for (TAC *p = list; p; p = p->next)
    {
        int drop = 0;
        if (strcmp(p->op, "=") == 0 && isTempName(p->result))
        {
            /* does result appear in uses? */
            Use *u = uses;
            while (u && strcmp(u->name, p->result) != 0)
                u = u->next;
            if (!u)
                drop = 1;
        }
        if (!drop)
        {
            /* clone node */
            TAC *c = createTAC(p->op, p->arg1, p->arg2, p->result);
            newList = insertTAC(newList, c);
        }
    }

    /* free uses */
    while (uses)
    {
        Use *n = uses->next;
        free(uses->name);
        free(uses);
        uses = n;
    }

    return newList;
}

static int isAtom(NodeAST *node) {
    if (!node) return 1;
    return (node->type == AST_NUM || node->type == AST_FLOAT || node->type == AST_ID);
}

char *genExprPython(NodeAST *expr)
{
    if (!expr)
        return strdup("");

    switch (expr->type)
    {
    case AST_NUM:
    {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%d", expr->value);
        return strdup(buffer);
    }

    case AST_FLOAT:
    {
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "%f", expr->floatValue);
        return strdup(buffer);
    }

    case AST_ID:
        return strdup(expr->name);

    case AST_BINOP:
    {
        char *left = genExprPython(expr->children[0]);
        char *right = genExprPython(expr->children[1]);

        const char *pyOp = expr->op;
        if (strcmp(expr->op, "&&") == 0)
        {
            pyOp = "and";
        }
        else if (strcmp(expr->op, "||") == 0)
        {
            pyOp = "or";
        }

        size_t size = strlen(left) + strlen(pyOp) + strlen(right) + 8;
        char *result = malloc(size);

        if (result)
        {
            // Aplica a regra do átomo para evitar parênteses duplicados
            if (isAtom(expr->children[0]) && isAtom(expr->children[1])) {
                snprintf(result, size, "%s %s %s", left, pyOp, right);
            } else {
                snprintf(result, size, "(%s %s %s)", left, pyOp, right);
            }
        }

        free(left);
        free(right);

        return result;
    }

    case AST_UNOP:
    {
        char *child = genExprPython(expr->children[0]);

        const char *pyOp = expr->op;
        if (strcmp(expr->op, "!") == 0)
        {
            pyOp = "not ";
        }

        size_t size = strlen(pyOp) + strlen(child) + 4;
        char *result = malloc(size);

        if (result)
        {
            // Aplica a regra do átomo para o operador unário
            if (isAtom(expr->children[0])) {
                snprintf(result, size, "%s%s", pyOp, child);
            } else {
                snprintf(result, size, "(%s%s)", pyOp, child);
            }
        }

        free(child);
        return result;
    }

    case AST_CALL:
    {
        char *args = genExprPython(expr->children[0]);

        size_t size = strlen(expr->name) + strlen(args) + 3;
        char *result = malloc(size);

        if (result)
        {
            snprintf(result, size, "%s(%s)", expr->name, args);
        }

        free(args);
        return result;
    }

    case AST_SEQ:
    {
        char *left = genExprPython(expr->children[0]);
        char *right = genExprPython(expr->children[1]);

        if (strlen(left) == 0)
        {
            free(left);
            return right;
        }
        if (strlen(right) == 0)
        {
            free(right);
            return left;
        }

        size_t size = strlen(left) + strlen(right) + 3;
        char *result = malloc(size);

        if (result)
        {
            snprintf(result, size, "%s, %s", left, right);
        }

        free(left);
        free(right);
        return result;
    }

    default:
        return strdup("");
    }
}

static void printIndent(FILE *out, int level)
{
    for (int i = 0; i < level; i++)
    {
        fprintf(out, "    "); // 4 espaços
    }
}

static void genParamsPython(NodeAST *params, FILE *out, int *first)
{
    if (!params)
        return;

    if (params->type == AST_SEQ)
    {
        genParamsPython(params->children[0], out, first);
        genParamsPython(params->children[1], out, first);
        return;
    }

    if (params->type == AST_DECL && params->child_count > 0)
    {
        if (!*first)
            fprintf(out, ", ");

        fprintf(out, "%s", params->children[0]->name);
        *first = 0;
    }
}

void genNodePython(NodeAST *node, FILE *out, int indent)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_SEQ:
        genNodePython(node->children[0], out, indent);
        genNodePython(node->children[1], out, indent);
        break;

    case AST_DECL:
    {
        char *varName = node->children[0]->name;

        if (node->child_count == 2)
        {
            char *value = genExprPython(node->children[1]);

            printIndent(out, indent);

            fprintf(out,
                    "%s = %s\n",
                    varName,
                    value);

            free(value);
        }

        break;
    }

    case AST_ASSIGN:
    {
        char *value =
            genExprPython(node->children[1]);
        printIndent(out, indent);
        fprintf(out,
                "%s = %s\n",
                node->children[0]->name,
                value);

        free(value);
        break;
    }

    case AST_IF:
    {        
        char *cond =
            genExprPython(node->children[0]);

        printIndent(out, indent);
        fprintf(out,
                "if %s:\n",
                cond);

        genNodePython(
            node->children[1],
            out,
            indent + 1);

        if (node->child_count == 3)
        {
            printIndent(out, indent);
            fprintf(out, "else:\n");

            genNodePython(
                node->children[2],
                out,
                indent + 1);
        }

        free(cond);
        break;
    }
    case AST_WHILE:
    {
        char *cond =
            genExprPython(node->children[0]);

        printIndent(out, indent);

        fprintf(out,
                "while %s:\n",
                cond);

        genNodePython(
            node->children[1],
            out,
            indent + 1);

        free(cond);
        break;
    }
    case AST_FOR:
    {
        NodeAST *init = node->children[0];
        NodeAST *cond = node->children[1];
        NodeAST *step = node->children[2];
        NodeAST *body = node->children[3];

        if (init)
            genNodePython(init, out, indent);

        char *condStr = genExprPython(cond);

        printIndent(out, indent);
        fprintf(out, "while %s:\n", condStr);

        free(condStr);

        if (body)
            genNodePython(body, out, indent + 1);

        if (step)
            genNodePython(step, out, indent + 1);

        break;
    }
    case AST_DO_WHILE:
    {
        char *cond = genExprPython(node->children[1]);

        printIndent(out, indent);
        fprintf(out, "while True:\n");

        genNodePython(
            node->children[0],
            out,
            indent + 1);

        printIndent(out, indent + 1);
        // REMOVIDO os parênteses redundantes do formato!
        fprintf(out, "if not %s:\n", cond);

        printIndent(out, indent + 2);
        fprintf(out, "break\n");

        free(cond);
        break;
    }
    case AST_BREAK:
    {
        printIndent(out, indent);
        fprintf(out, "break\n");
        break;
    }
    case AST_CONTINUE:
    {
        printIndent(out, indent);
        fprintf(out, "continue\n");
        break;
    }
    case AST_FUNC:
    {
        fprintf(out,
                "def %s(",
                node->name);

        int firstParam = 1;
        genParamsPython(node->children[0], out, &firstParam);

        fprintf(out, "):\n");

        if (node->children[1])
        {
            genNodePython(
                node->children[1],
                out,
                indent + 1);
        }
        else
        {
            printIndent(out, indent + 1);
            fprintf(out, "pass\n");
        }

        fprintf(out, "\n");
        break;
    }
    case AST_RETURN:
    {
        printIndent(out, indent);

        fprintf(out, "return");

        if (node->child_count == 1)
        {
            char *value =
                genExprPython(node->children[0]);

            fprintf(out,
                    " %s",
                    value);

            free(value);
        }

        fprintf(out, "\n");
        break;
    }
    case AST_CALL:
    {
        char *call =
            genExprPython(node);

        printIndent(out, indent);
        fprintf(out,
                "%s\n",
                call);

        free(call);
        break;
    }
    default:
        break;
    }
}

void generatePythonFile(NodeAST *root, char *filename)
{
    if (!root)
    {
        printf("[INFO] AST vazia, nenhum código Python gerado.\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "[CODEGEN ERROR] não foi possível criar ou abrir o arquivo '%s'.\n", filename);
        return;
    }

    printf("\n[CODEGEN] gerando código Python no arquivo '%s'...\n", filename);

    genNodePython(root, file, 0);
    fclose(file);

    printf("[CODEGEN] arquivo '%s' gerado com sucesso!\n", filename);
}
