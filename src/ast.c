#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

NodeAST *createNode(NodeType type) {

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

NodeAST *createNodeFloat(float value) {
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
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 || strcmp(op, "*") == 0 || strcmp(op, "/") == 0) {
        if (!isNumeric(left->dataType) || !isNumeric(right->dataType)) {
            fprintf(stderr, "Erro semântico: operação aritmética inválida\n");

            strcpy(newNode->dataType, "error");
            return newNode;
        }

        if (strcmp(left->dataType, "float") == 0 || strcmp(right->dataType, "float") == 0)
            strcpy(newNode->dataType, "float");
        else
            strcpy(newNode->dataType, "int");
    }

    // Operadores relacionais
    else if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<=") == 0
        || strcmp(op, ">=") == 0 || strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {

        if (!isNumeric(left->dataType) || !isNumeric(right->dataType)) {
            fprintf(stderr, "Erro semântico: comparação inválida\n");

            strcpy(newNode->dataType, "error");
            return newNode;
        }
        strcpy(newNode->dataType, "int");
    }

    // Operadores lógicos 
    else if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
        if (!isBooleanCompatible(left->dataType) || !isBooleanCompatible(right->dataType)) {
            fprintf(stderr, "Erro semântico: operação lógica inválida\n");

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
    
    if (left) {
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

int isNumeric(char *type) {
    return strcmp(type, "int") == 0 || strcmp(type, "float") == 0;
}

int isBooleanCompatible(char *type) {
    return strcmp(type, "int") == 0;
}

int isConditionValid(NodeAST *expr) {
    return strcmp(expr->dataType, "int") == 0;
}

int isAssignable(const char *lhs, const char *rhs) {
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

    if (params)
        addChild(newNode, params);

    if (body)
        addChild(newNode, body);

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

    if (left->type != AST_SEQ) {
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
                 "Erro semântico: quantidade de argumentos incompatível na chamada de função");
        return 0;
    }

    NodeAST *paramNode = listHead(params);
    NodeAST *argNode = listHead(args);

    NodeAST *nextParams = listTail(params);
    NodeAST *nextArgs = listTail(args);

    if (!paramNode || paramNode->type != AST_DECL)
    {
        snprintf(message, messageSize,
                 "Erro semântico: assinatura de função inválida");
        return 0;
    }

    if (!argNode || !argNode->dataType[0])
    {
        snprintf(message, messageSize,
                 "Erro semântico: argumento sem tipo definido na chamada de função");
        return 0;
    }

    if (strcmp(paramNode->op, argNode->dataType) != 0)
    {
        snprintf(message, messageSize,
                 "Erro semântico: tipo de argumento incompatível na chamada de função; esperado %s, recebido %s",
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
                 "Erro semântico: função sem assinatura registrada: %s",
                 name);
        return 0;
    }

    NodeAST *params = NULL;

    /*
     * Estrutura do AST da função:
     * - se child_count == 0: sem params e sem body (inválido)
     * - se child_count == 1: normalmente significa que só existe o corpo (sem parâmetros)
     * - se child_count >= 2: children[0] = params, children[1] = body
     */
    if (funcAst->child_count >= 2) {
        params = funcAst->children[0];
    } else {
        params = NULL; /* sem parâmetros */
    }

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
        if (root->child_count >= 1 && root->children[0])
        {
            printAST(root->children[0], level);
            printf("\n");
        }
        if (root->child_count >= 2 && root->children[1])
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

TAC *createTAC(const char *op, const char *arg1, const char *arg2, const char *result) {
    TAC *t = malloc(sizeof(TAC));
    if (!t) return NULL;
    memset(t, 0, sizeof(TAC));
    if (op) strncpy(t->op, op, sizeof(t->op)-1);
    if (arg1) strncpy(t->arg1, arg1, sizeof(t->arg1)-1);
    if (arg2) strncpy(t->arg2, arg2, sizeof(t->arg2)-1);
    if (result) strncpy(t->result, result, sizeof(t->result)-1);
    t->next = NULL;
    return t;
}

TAC *insertTAC(TAC *list, TAC *instr) {
    if (!instr) return list;
    if (!list) return instr;
    TAC *p = list;
    while (p->next) p = p->next;
    p->next = instr;
    return list;
}

void printTAC(TAC *list) {
    for (TAC *p = list; p; p = p->next) {
        if (strcmp(p->op, "=") == 0) {
            printf("%s = %s\n", p->result, p->arg1);
        } else if (strcmp(p->op, "ret") == 0) {
            if (p->arg1[0])
                printf("ret %s\n", p->arg1);
            else
                printf("ret\n");
        } else if (strlen(p->arg2) > 0) {
            printf("%s = %s %s %s\n", p->result, p->arg1, p->op, p->arg2);
        } else if (strcmp(p->op, "call") == 0) {
            if (strlen(p->result) > 0)
                printf("%s = call %s\n", p->result, p->arg1);
            else
                printf("call %s\n", p->arg1);
        } else if (strcmp(p->op, "label") == 0) {
            printf("%s:\n", p->result);
        } else if (strcmp(p->op, "goto") == 0) {
            printf("goto %s\n", p->result);
        } else if (strcmp(p->op, "ifz") == 0) {
            printf("ifz %s goto %s\n", p->arg1, p->result);
        } else {
            /* fallback */
            printf("<tac op='%s' a1='%s' a2='%s' res='%s'>\n", p->op, p->arg1, p->arg2, p->result);
        }
    }
}

void freeTAC(TAC *list) {
    while (list) {
        TAC *n = list->next;
        free(list);
        list = n;
    }
}

static int tempCounter = 0;

char *newTemp() {
    char buffer[32];

    snprintf(buffer,
             sizeof(buffer),
             "t%d",
             tempCounter++);

    return strdup(buffer);
}

static int labelCounter = 0;

char *newLabel() {
    char buffer[32];

    snprintf(buffer,
             sizeof(buffer),
             "L%d",
             labelCounter++);

    return strdup(buffer);
}

static char *genExprTAC(NodeAST *expr, TAC **list)
{
    if (!expr)
        return strdup("");

    switch (expr->type) {
        case AST_NUM: {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%d", expr->value);
            return strdup(buffer);
        }
        case AST_FLOAT: {
            char buffer[64];
            snprintf(buffer, sizeof(buffer), "%f", expr->floatValue);
            return strdup(buffer);
        }
        case AST_ID:
            return strdup(expr->name);

        case AST_BINOP: {
            char *left = genExprTAC(expr->children[0], list);
            char *right = genExprTAC(expr->children[1], list);

            char *temp = newTemp();

            TAC *instr = createTAC(
                expr->op,
                left,
                right,
                temp
            );

            *list = insertTAC(*list, instr);

            free(left);
            free(right);

            return temp;
        }

        case AST_UNOP:
            return strdup("");

        case AST_CALL:
            return strdup("");

        default:
            return strdup("");
    }
}

static TAC *genNodeTAC(NodeAST *node, TAC *list) {
    if (!node) return list;

    switch (node->type) {
        case AST_SEQ:
            list = genNodeTAC(node->children[0], list);
            list = genNodeTAC(node->children[1], list);
            return list;

        case AST_DECL: {

            if (node->child_count == 2) {

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
        case AST_ASSIGN: {
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
        case AST_IF: {
            char *condTemp = genExprTAC(node->children[0], &list);
            char *labelFalse = newLabel();

            TAC *instrIfz = createTAC("ifz", condTemp, "", labelFalse);
            list = insertTAC(list, instrIfz);
            list = genNodeTAC(node->children[1], list);

            if (node->child_count == 3 && node->children[2] != NULL) {
                char *labelEnd = newLabel();
                
                TAC *instrGoto = createTAC("goto", "", "", labelEnd);
                list = insertTAC(list, instrGoto);

                TAC *instrLabelFalse = createTAC("label", "", "", labelFalse);
                list = insertTAC(list, instrLabelFalse);

                list = genNodeTAC(node->children[2], list);

                TAC *instrLabelEnd = createTAC("label", "", "", labelEnd);
                list = insertTAC(list, instrLabelEnd);
                
                free(labelEnd);
            } else {
                TAC *instrLabelFalse = createTAC("label", "", "", labelFalse);
                list = insertTAC(list, instrLabelFalse);
            }

            free(condTemp);
            free(labelFalse);

            return list;
        }

        case AST_WHILE:
            return list;

        case AST_FOR:
        case AST_DO_WHILE:
            return list;

        case AST_FUNC:
            return list;

        case AST_RETURN: {
            if (node->child_count == 1) {
                char *vbuf = genExprTAC(node->children[0], &list);
                TAC *r = createTAC("ret", vbuf ? vbuf : "", "", "");
                list = insertTAC(list, r);
                free(vbuf);
            } else {
                TAC *r = createTAC("ret", "", "", "");
                list = insertTAC(list, r);
            }
            return list;
        }

        default:
            return list;
    }
}

void generateTAC(NodeAST *root) {
    TAC *list = NULL;
    list = genNodeTAC(root, list);

    printf("--- TAC emitted (basic) ---\n");
    printTAC(list);
    printf("--- end TAC ---\n");

    freeTAC(list);
}