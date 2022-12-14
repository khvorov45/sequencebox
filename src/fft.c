#include "mltaln.h"

/*
  from "C gengo niyoru saishin algorithm jiten" ISBN4-87408-414-1 Haruhiko Okumura
*/
static void
make_sintbl(int n, double sintbl[]) {
    int    i, n2, n4, n8;
    double c, s, dc, ds, t;

    n2 = n / 2;
    n4 = n / 4;
    n8 = n / 8;
    t = sin(3.14159265358979323846 / n);
    dc = 2 * t * t;
    ds = sqrt(dc * (2 - dc));
    t = 2 * dc;
    c = sintbl[n4] = 1;
    s = sintbl[0] = 0;
    for (i = 1; i < n8; i++) {
        c -= dc;
        dc += t * c;
        s += ds;
        ds -= t * s;
        sintbl[i] = s;
        sintbl[n4 - i] = c;
    }
    if (n8 != 0)
        sintbl[n8] = sqrt(0.5);
    for (i = 0; i < n4; i++)
        sintbl[n2 - i] = sintbl[i];
    for (i = 0; i < n2 + n4; i++)
        sintbl[i + n2] = -sintbl[i];
}
/*
  {\tt fft()}.
*/
static void
make_bitrev(int n, int bitrev[]) {
    int i, j, k, n2;

    n2 = n / 2;
    i = j = 0;
    for (;;) {
        bitrev[i] = j;
        if (++i >= n)
            break;
        k = n2;
        while (k <= j) {
            j -= k;
            k /= 2;
        }
        j += k;
    }
}

int
fft(int n, Fukusosuu* x, int freeflag) {
    static int     last_n = 0; /*  {\tt n} */
    static int*    bitrev = NULL; /*  */
    static double* sintbl = NULL; /*  */
    int            i, j, k, ik, h, d, k2, n4, inverse;
    double         t, s, c, dR, dI;

    if (freeflag) {
        if (bitrev)
            free(bitrev);
        bitrev = NULL;
        if (sintbl)
            free(sintbl);
        sintbl = NULL;
        last_n = 0;
        return (0);
    }

    /*  */
    if (n < 0) {
        n = -n;
        inverse = 1; /*  */
    } else
        inverse = 0;
    n4 = n / 4;
    if (n != last_n || n == 0) {
        last_n = n;
#if 0
                if (sintbl != NULL) {
					free(sintbl);
					sintbl = NULL;
				}
                if (bitrev != NULL) {
					free(bitrev);
					bitrev = NULL;
				}
                if (n == 0) return 0;  /*  */
                sintbl = (double *)malloc((n + n4) * sizeof(double));
                bitrev = (int *)malloc(n * sizeof(int));
#else /* by T. Nishiyama */
        sintbl = realloc(sintbl, (n + n4) * sizeof(double));
        bitrev = realloc(bitrev, n * sizeof(int));
#endif
        if (sintbl == NULL || bitrev == NULL) {
            fprintf(stderr, "\n");
            return 1;
        }
        make_sintbl(n, sintbl);
        make_bitrev(n, bitrev);
    }
    for (i = 0; i < n; i++) { /*  */
        j = bitrev[i];
        if (i < j) {
            t = x[i].R;
            x[i].R = x[j].R;
            x[j].R = t;
            t = x[i].I;
            x[i].I = x[j].I;
            x[j].I = t;
        }
    }
    for (k = 1; k < n; k = k2) { /*  */
#if 0
				fprintf( stderr, "%d / %d\n", k, n );
#endif
        h = 0;
        k2 = k + k;
        d = n / k2;
        for (j = 0; j < k; j++) {
#if 0
					if( j % 1 == 0 )
						fprintf( stderr, "%d / %d\r", j, k );
#endif
            c = sintbl[h + n4];
            if (inverse)
                s = -sintbl[h];
            else
                s = sintbl[h];
            for (i = j; i < n; i += k2) {
#if 0
								if( k>=4194000 ) fprintf( stderr, "in loop %d -  %d < %d, k2=%d\r", j, i, n, k2 );
#endif
                ik = i + k;
                dR = s * x[ik].I + c * x[ik].R;
                dI = c * x[ik].I - s * x[ik].R;
                x[ik].R = x[i].R - dR;
                x[i].R += dR;
                x[ik].I = x[i].I - dI;
                x[i].I += dI;
            }
            h += d;
        }
    }
    if (!inverse) /* n */
        for (i = 0; i < n; i++) {
            x[i].R /= n;
            x[i].I /= n;
        }
    return 0; /*  */
}
