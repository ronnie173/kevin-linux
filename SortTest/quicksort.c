/* this program has segment fault bug */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXN 10000000

typedef int DType;

DType realx[MAXN];
int *x = realx; /* allow x to shift for heaps */
int n;

void swap(int i, int j) {
    DType t = x[i];
    x[i] = x[j];
    x[j] = t;
}

int randint(int l, int u) {
    return l + (RAND_MAX * rand() + rand()) % (u-l+1);
}

void qsort1(int l, int u) {
    int i, m;
    if (l >= u)
        return;
    m = l;
    for (i = l+1; i <= u; i++)
        if (x[i] < x[l])
            swap(++m, i);
    swap(l, m);
    qsort1(l, m-1);
    qsort1(m+1, u);
}

void quicksort(int l, int u) {
    int i, m;

    if (l >= u) return;

    swap(l, randint(l, u));
    m = l;

    for (i = l + 1; i <= u; i++)
        if (x[i] < x[l])
            swap(++m, i);

    swap(l, m);
    quicksort(l, m - 1);
    quicksort(m + 1, u);
}

int main() {
    int i, mod, start, clicks;

    scanf("%d %d", &n, &mod);
    if (mod <= 0)
        mod = 10 * n ;
    for (i = 0; i < n; i++)
        x[i] = randint(0, mod - 1);

    start = clock();
    //quicksort(0, n - 1);
    qsort1(0, n - 1);

    clicks = clock() - start;

    for (i = 0; i < n - 1; i++) {
        printf("x[%d]=%d\n", i, x[i]);
        if (x[i] > x[i + 1])
            printf("  SORT BUG i=%d\n", i);
    }

    printf("%d\t%d\t%d\t%g\n",
            n, mod, clicks,
            1e9 * clicks / ((float) CLOCKS_PER_SEC * n));

    return 0;
}
