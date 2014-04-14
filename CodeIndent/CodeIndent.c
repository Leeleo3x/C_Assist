/*
 *  Filename: CodeIndent.c
 *  Create: 04/12/2014
 *  Author: Leo Lee , leeleo3x@gmail.com
 *
 */

#include "../C_Assist.h"

struct contentNode *contentHead = NULL;
struct contentNode *contentTail = NULL;

struct contentNode
{
    char * content;
    int length;
    struct contentNode *next;
};

struct contentNode *createNewContentNode(char * string, int len)
{
    struct contentNode * tmp = (struct contentNode *) malloc(sizeof(struct contentNode));
    tmp->content = (char *) malloc(sizeof(char) * strlen(string));
    strcpy(tmp->content, string);
    tmp->length = len;
    return tmp;
}

void addNewContentNode(struct contentNode * node)
{
    if (contentHead == NULL) {
        contentHead = node;        
    } else {
        contentTail->next = node;
    }
    contentTail = node;
    contentTail->next = NULL;
}

void catContent()
{
    struct contentNode * p = contentHead;
    while (p != NULL) {
        printf("%s", p->content);
        p = p->next;
    }
}

void indent(const char * fileName)
{
    FILE *file;
    char line[MAX_LINE_LENGTH];
    int len,lineCount = 0;
    int bracketCount[MAX_LINE_SUM];

    file = fopen(fileName, "r");
    if (file == NULL) return;
    while (fgets(line , MAX_LINE_LENGTH, file) != NULL) {
        len = strlen(line);
        addNewContentNode(createNewContentNode(line, len));
        lineCount ++;
    }
    memset(bracketCount, 0, sizeof(bracketCount));
    int i = 0, current = 0, count = 0;
    struct contentNode * p = contentHead;
    while (p != NULL) {
        len = p->length;
        char * str = p->content;
        i = 0;
        while (i <= len) {
            if (str[i] == '{') {
                current = count;
                while (current < lineCount) {
                    current ++;
                    bracketCount[current]++;
                }
            }
            if (str[i] == '}') {
                current = count;
                while (current < lineCount) {
                    bracketCount[current] --;
                    current ++;
                }
            }
            i++;
        }
        count ++;
        p = p->next;
    }

    fclose(file);
    file = fopen("indentFile.tmp","w");

    int tabCount = 0, h = 0;
    p = contentHead;
    int linePos = 0;
    char * indent;

    while (p != NULL) {
        count = 0;
        tabCount = 0;
        indent = (char *) malloc(sizeof(char));
        char * str = p->content;
        while(str[h] == ' ' || str[h] == '\t') {
            if (str[h] == ' ') {
                count ++;
                h++;
            } 
            else if (str[h] == '\t') {
                tabCount ++;
                h++;
            } 
            else {
                break;
            }
        }
        h = 0;

        while (bracketCount[linePos] > 0) {
            bracketCount[linePos] --;
            indent = strcat(indent, TAB);
        }
        if (count == 0 && tabCount == 0) {
            fprintf(file, "%s%s", indent, str);
        } 
        else if (count > 0 || tabCount > 0) {
            fprintf(file, "%s", indent);
            len = p->length;
            i = 0;
            while (i < len - tabCount - count) {
                fprintf(file, "%c", *(str + i + count + tabCount));
                i++;
            }
        }
        linePos ++;
        p = p->next;
    }
    fclose(file);
}

int main()
{
    indent("test.c");
    return 0;
}
