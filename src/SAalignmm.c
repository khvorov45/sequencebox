#include "mltaln.h"

#define DEBUG 0

static void
match_calc(Context* ctx, double* match, double** cpmx1, double** cpmx2, int i1, int lgth2, double** doublework, int** intwork, int initialize) {
    int j, k, l;
    //	double scarr[26];
    double** cpmxpd = doublework;
    int**    cpmxpdn = intwork;
    int      count = 0;
    double*  scarr;
    scarr = calloc(ctx->nalphabets, sizeof(double));

    if (initialize) {
        for (j = 0; j < lgth2; j++) {
            count = 0;
            for (l = 0; l < ctx->nalphabets; l++) {
                if (cpmx2[l][j]) {
                    cpmxpd[count][j] = cpmx2[l][j];
                    cpmxpdn[count][j] = l;
                    count++;
                }
            }
            cpmxpdn[count][j] = -1;
        }
    }

    for (l = 0; l < ctx->nalphabets; l++) {
        scarr[l] = 0.0;
        for (k = 0; k < ctx->nalphabets; k++)
            scarr[l] += ctx->n_dis[k][l] * cpmx1[k][i1];
    }
    for (j = 0; j < lgth2; j++) {
        match[j] = 0;
        for (k = 0; cpmxpdn[k][j] > -1; k++)
            match[j] += scarr[cpmxpdn[k][j]] * cpmxpd[k][j];
    }
    free(scarr);
}

static double
Atracking(Context* ctx, double* lasthorizontalw, double* lastverticalw, char** seq1, char** seq2, char** mseq1, char** mseq2, int** ijp, int icyc, int jcyc) {
    int i, j, k, l, iin, jin, ifi, jfi, lgth1, lgth2;
    //	char gap[] = "-";
    char*  gap;
    double wm;
    gap = ctx->newgapstr;
    lgth1 = strlen(seq1[0]);
    lgth2 = strlen(seq2[0]);

#if DEBUG
    for (i = 0; i < lgth1; i++) {
        fprintf(stderr, "lastverticalw[%d] = %f\n", i, lastverticalw[i]);
    }
#endif

    if (ctx->outgap == 1)
        ;
    else {
        wm = lastverticalw[0];
        for (i = 0; i < lgth1; i++) {
            if (lastverticalw[i] >= wm) {
                wm = lastverticalw[i];
                iin = i;
                jin = lgth2 - 1;
                ijp[lgth1][lgth2] = +(lgth1 - i);
            }
        }
        for (j = 0; j < lgth2; j++) {
            if (lasthorizontalw[j] >= wm) {
                wm = lasthorizontalw[j];
                iin = lgth1 - 1;
                jin = j;
                ijp[lgth1][lgth2] = -(lgth2 - j);
            }
        }
    }

    for (i = 0; i < lgth1 + 1; i++) {
        ijp[i][0] = i + 1;
    }
    for (j = 0; j < lgth2 + 1; j++) {
        ijp[0][j] = -(j + 1);
    }

    for (i = 0; i < icyc; i++) {
        mseq1[i] += lgth1 + lgth2;
        *mseq1[i] = 0;
    }
    for (j = 0; j < jcyc; j++) {
        mseq2[j] += lgth1 + lgth2;
        *mseq2[j] = 0;
    }
    iin = lgth1;
    jin = lgth2;
    for (k = 0; k <= lgth1 + lgth2; k++) {
        if (ijp[iin][jin] < 0) {
            ifi = iin - 1;
            jfi = jin + ijp[iin][jin];
        } else if (ijp[iin][jin] > 0) {
            ifi = iin - ijp[iin][jin];
            jfi = jin - 1;
        } else {
            ifi = iin - 1;
            jfi = jin - 1;
        }
        l = iin - ifi;
        while (--l) {
            for (i = 0; i < icyc; i++)
                *--mseq1[i] = seq1[i][ifi + l];
            for (j = 0; j < jcyc; j++)
                *--mseq2[j] = *gap;
            k++;
        }
        l = jin - jfi;
        while (--l) {
            for (i = 0; i < icyc; i++)
                *--mseq1[i] = *gap;
            for (j = 0; j < jcyc; j++)
                *--mseq2[j] = seq2[j][jfi + l];
            k++;
        }
        if (iin <= 0 || jin <= 0)
            break;
        for (i = 0; i < icyc; i++)
            *--mseq1[i] = seq1[i][ifi];
        for (j = 0; j < jcyc; j++)
            *--mseq2[j] = seq2[j][jfi];
        k++;
        iin = ifi;
        jin = jfi;
    }
    return (0.0);
}

double
Aalign(Context* ctx, char** seq1, char** seq2, double* eff1, double* eff2, int icyc, int jcyc, int alloclen) {
    register int    i, j;
    int             lasti; /* outgap == 0 -> lgth1, outgap == 1 -> lgth1+1 */
    int             lgth1, lgth2;
    int             resultlen;
    double          wm = 0.0; /* int ?????? */
    double          g;
    double          x;
    static double   mi, *m;
    static int**    ijp;
    static int      mpi, *mp;
    static double*  currentw;
    static double*  previousw;
    static double*  match;
    static double*  initverticalw; /* kufuu sureba iranai */
    static double*  lastverticalw; /* kufuu sureba iranai */
    static char**   mseq1;
    static char**   mseq2;
    static char**   mseq;
    static double** cpmx1;
    static double** cpmx2;
    static int**    intwork;
    static double** doublework;
    static int      orlgth1 = 0, orlgth2 = 0;

#if DEBUG
    fprintf(stderr, "eff in SA+++align\n");
    for (i = 0; i < icyc; i++)
        fprintf(stderr, "eff1[%d] = %f\n", i, eff1[i]);
#endif
    if (orlgth1 == 0) {
        mseq1 = AllocateCharMtx(ctx->njob, 1);
        mseq2 = AllocateCharMtx(ctx->njob, 1); /* by J. Thompson */
    }

    lgth1 = strlen(seq1[0]);
    lgth2 = strlen(seq2[0]);

    if (lgth1 > orlgth1 || lgth2 > orlgth2) {
        int ll1, ll2;

        if (orlgth1 > 0 && orlgth2 > 0) {
            FreeFloatVec(currentw);
            FreeFloatVec(previousw);
            FreeFloatVec(match);
            FreeFloatVec(initverticalw);
            FreeFloatVec(lastverticalw);

            FreeFloatVec(m);
            FreeIntVec(mp);

            FreeCharMtx(mseq);

            FreeFloatMtx(cpmx1);
            FreeFloatMtx(cpmx2);

            FreeFloatMtx(doublework);
            FreeIntMtx(intwork);
        }

        ll1 = MAX((int)(1.1 * lgth1), orlgth1) + 100;
        ll2 = MAX((int)(1.1 * lgth2), orlgth2) + 100;

        fprintf(stderr, "\ntrying to allocate (%d+%d)xn matrices ... ", ll1, ll2);

        currentw = AllocateFloatVec(ll2 + 2);
        previousw = AllocateFloatVec(ll2 + 2);
        match = AllocateFloatVec(ll2 + 2);

        initverticalw = AllocateFloatVec(ll1 + 2);
        lastverticalw = AllocateFloatVec(ll1 + 2);

        m = AllocateFloatVec(ll2 + 2);
        mp = AllocateIntVec(ll2 + 2);

        mseq = AllocateCharMtx(ctx->njob, ll1 + ll2);

        cpmx1 = AllocateFloatMtx(ctx->nalphabets, ll1 + 2);
        cpmx2 = AllocateFloatMtx(ctx->nalphabets, ll2 + 2);

        doublework = AllocateFloatMtx(ctx->nalphabets, MAX(ll1, ll2) + 2);
        intwork = AllocateIntMtx(ctx->nalphabets, MAX(ll1, ll2) + 2);

        fprintf(stderr, "succeeded\n");

        orlgth1 = ll1;
        orlgth2 = ll2;
    }

    for (i = 0; i < icyc; i++)
        mseq1[i] = mseq[i];
    for (j = 0; j < jcyc; j++)
        mseq2[j] = mseq[icyc + j];

    if (orlgth1 > ctx->commonAlloc1 || orlgth2 > ctx->commonAlloc2) {
        int ll1, ll2;

        if (ctx->commonAlloc1 && ctx->commonAlloc2) {
            FreeIntMtx(ctx->commonIP);
        }

        ll1 = MAX(orlgth1, ctx->commonAlloc1);
        ll2 = MAX(orlgth2, ctx->commonAlloc2);

        fprintf(stderr, "\n\ntrying to allocate %dx%d matrices ... ", ll1 + 1, ll2 + 1);

        ctx->commonIP = AllocateIntMtx(ll1 + 10, ll2 + 10);

        fprintf(stderr, "succeeded\n\n");

        ctx->commonAlloc1 = ll1;
        ctx->commonAlloc2 = ll2;
    }
    ijp = ctx->commonIP;

    cpmx_calc(ctx, seq1, cpmx1, eff1, strlen(seq1[0]), icyc);
    cpmx_calc(ctx, seq2, cpmx2, eff2, strlen(seq2[0]), jcyc);

    match_calc(ctx, initverticalw, cpmx2, cpmx1, 0, lgth1, doublework, intwork, 1);
    match_calc(ctx, currentw, cpmx1, cpmx2, 0, lgth2, doublework, intwork, 1);

    if (ctx->outgap == 1) {
        for (i = 1; i < lgth1 + 1; i++) {
            initverticalw[i] += ctx->penalty * 0.5;
        }
        for (j = 1; j < lgth2 + 1; j++) {
            currentw[j] += ctx->penalty * 0.5;
        }
    }

    for (j = 0; j < lgth2 + 1; ++j) {
        m[j] = currentw[j - 1] + ctx->penalty * 0.5;
        mp[j] = 0;
    }

    lastverticalw[0] = currentw[lgth2 - 1];

    if (ctx->outgap)
        lasti = lgth1 + 1;
    else
        lasti = lgth1;

    for (i = 1; i < lasti; i++) {
        doublencpy(previousw, currentw, lgth2 + 1);
        previousw[0] = initverticalw[i - 1];

        match_calc(ctx, currentw, cpmx1, cpmx2, i, lgth2, doublework, intwork, 0);
        currentw[0] = initverticalw[i];

        mi = previousw[0] + ctx->penalty * 0.5;
        mpi = 0;
        for (j = 1; j < lgth2 + 1; j++) {
            wm = previousw[j - 1];
            ijp[i][j] = 0;

            g = ctx->penalty * 0.5;
            x = mi + g;
            if (x > wm) {
                wm = x;
                ijp[i][j] = -(j - mpi);
            }
            g = ctx->penalty * 0.5;
            x = previousw[j - 1] + g;
            if (mi <= x) {
                mi = x;
                mpi = j - 1;
            }

            g = ctx->penalty * 0.5;
            x = m[j] + g;
            if (x > wm) {
                wm = x;
                ijp[i][j] = +(i - mp[j]);
            }
            g = ctx->penalty * 0.5;
            x = previousw[j - 1] + g;
            if (m[j] <= x) {
                m[j] = x;
                mp[j] = i - 1;
            }
            currentw[j] += wm;
        }
        lastverticalw[i] = currentw[lgth2 - 1];
    }

    Atracking(ctx, currentw, lastverticalw, seq1, seq2, mseq1, mseq2, ijp, icyc, jcyc);

    resultlen = strlen(mseq1[0]);
    if (alloclen < resultlen || resultlen > N) {
        fprintf(stderr, "alloclen=%d, resultlen=%d, N=%d\n", alloclen, resultlen, N);
        ErrorExit("LENGTH OVER!\n");
    }

    for (i = 0; i < icyc; i++)
        strcpy(seq1[i], mseq1[i]);
    for (j = 0; j < jcyc; j++)
        strcpy(seq2[j], mseq2[j]);
    /*
	fprintf( stderr, "\n" );
	for( i=0; i<icyc; i++ ) fprintf( stderr, "%s\n", mseq1[i] );
	fprintf( stderr, "#####\n" );
	for( j=0; j<jcyc; j++ ) fprintf( stderr, "%s\n", mseq2[j] );
	*/
    return (wm);
}
