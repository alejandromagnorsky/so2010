#include "../include/list.h"
#include <stdio.h>

extern struct ListNamespace List;

int main() {

    list_t list = List.new();
    List.add(list, "HOLA");
    List.add(list, "CHAU");

    printf("%s\n", (char*) list->head->data);
    printf("%s\n", (char*) list->head->next->data);
    printf("%d\n", list->count);
}