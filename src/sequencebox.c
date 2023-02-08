#include <R.h>
#include <Rinternals.h>

// clang-format off
#define aln_IMPLEMENTATION
#define aln_PUBLICAPI static
#define aln_assert(cond) do {if (cond) {} else {error("assertion failure at %s:%d", __FILE__, __LINE__);}} while (0)
#include "align.h"
// clang-format on

SEXP
align_c(SEXP reference, SEXP sequences) {
    int  sequencesCount = LENGTH(sequences);
    SEXP result = PROTECT(allocVector(STRSXP, sequencesCount));

    intptr_t  totalMemoryBytes = 20 * 1024 * 1024;
    aln_Arena arena = {.base = R_alloc(totalMemoryBytes, 1), .size = totalMemoryBytes};
    if (arena.base) {
        SEXP    rRef = STRING_ELT(reference, 0);
        aln_Str alnRef = (aln_Str) {(char*)CHAR(rRef), LENGTH(rRef)};

        aln_Str* alnStrings = aln_arenaAllocArray(&arena, aln_Str, sequencesCount);
        for (int seqIndex = 0; seqIndex < sequencesCount; seqIndex++) {
            SEXP rSeq = STRING_ELT(sequences, seqIndex);
            alnStrings[seqIndex] = (aln_Str) {(char*)CHAR(rSeq), LENGTH(rSeq)};
        }

        aln_Arena       alnOutput = aln_createArenaFromArena(&arena, aln_arenaFreeSize(&arena) / 4);
        aln_Arena       alnTemp = aln_createArenaFromArena(&arena, aln_arenaFreeSize(&arena));
        aln_AlignResult alnResult = aln_align(alnRef, alnStrings, sequencesCount, alnOutput.base, alnOutput.size, alnTemp.base, alnTemp.size);
        if (alnResult.seqCount == sequencesCount) {
            for (int seqIndex = 0; seqIndex < alnResult.seqCount; seqIndex++) {
                aln_Str alnStr = alnResult.seqs[seqIndex];
                SET_STRING_ELT(result, seqIndex, mkCharLen(alnStr.ptr, (int)alnStr.len));
            }
        } else {
            error("unexpected alignment result");
        }
    } else {
        error("could not allocate memory");
    }

    UNPROTECT(1);
    return result;
}