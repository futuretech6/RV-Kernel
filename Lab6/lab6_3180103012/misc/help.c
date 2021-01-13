#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    void *a = (void *)0;
    printf("%p", a + 3);

    return 0;
}
