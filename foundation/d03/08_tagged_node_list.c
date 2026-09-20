/*
 * 本文件只证明：带 tag 的节点可以串成一条链表，遍历时先看 tag 再读对应字段。
 * 不证明：PG List 的数组实现、foreach 宏展开、深拷贝。
 */
#include "check.h"
#include <stdlib.h>
#include <string.h>

typedef enum { T_INT = 1, T_CSTR = 2 } Tag;

typedef struct Node {
    Tag tag;
    struct Node *next;
} Node;

typedef struct IntNode {
    Tag tag;
    struct Node *next;
    int value;
} IntNode;

typedef struct CStrNode {
    Tag tag;
    struct Node *next;
    const char *text;
} CStrNode;

static IntNode *make_int(int value)
{
    IntNode *n = malloc(sizeof(*n));
    if (n == NULL) {
        return NULL;
    }
    n->tag = T_INT;
    n->next = NULL;
    n->value = value;
    return n;
}

static CStrNode *make_cstr(const char *text)
{
    CStrNode *n = malloc(sizeof(*n));
    if (n == NULL) {
        return NULL;
    }
    n->tag = T_CSTR;
    n->next = NULL;
    n->text = text;
    return n;
}

static void list_free(Node *head)
{
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void)
{
    IntNode *a = make_int(10);
    CStrNode *b = make_cstr("mid");
    IntNode *c = make_int(32);
    FD_CHECK(a && b && c, "three nodes allocated");

    a->next = (Node *)b;
    b->next = (Node *)c;

    Node *head = (Node *)a;
    int sum = 0;
    int seen_cstr = 0;
    int count = 0;
    for (Node *n = head; n != NULL; n = n->next) {
        count += 1;
        if (n->tag == T_INT) {
            sum += ((IntNode *)n)->value;
        } else if (n->tag == T_CSTR) {
            seen_cstr += strcmp(((CStrNode *)n)->text, "mid") == 0;
        }
    }

    FD_CHECK(count == 3, "walk visits three nodes");
    FD_CHECK(sum == 42, "int nodes contribute 10 + 32");
    FD_CHECK(seen_cstr == 1, "cstr node is recognized by tag");
    FD_CHECK(head->tag == T_INT, "list head tag is visible without downcast");

    list_free(head);
    return 0;
}
