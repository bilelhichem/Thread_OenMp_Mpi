#include <stdio.h>

int main(int argc, char **argv)
{
    int book, page;

    #pragma omp parallel for private(page)
    for (book = 0; book < 3; book++)
        for (page = 0; page < 5; page++)
            printf("I read the page %d of the book %d .\n", page, book);
}
