#include "../programmable_build.h"

#define function static

typedef uint64_t u64;
typedef uint32_t u32;
typedef int32_t  i32;
typedef uint8_t  u8;

typedef struct Rng {
    u64 state;
    u64 inc;
} Rng;

function u32
getRandomU32(Rng* rng) {
    u64 oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    u32 xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    u32 rot = oldstate >> 59u;
    u32 result = (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    return result;
}

function Rng
createRng(u64 initstate, u64 initseq) {
    Rng rng = {.state = 0, .inc = (initseq << 1u) | 1u};
    getRandomU32(&rng);
    rng.state += initstate;
    getRandomU32(&rng);
    return rng;
}

function u32
getRandomU32BiasedBound(Rng* rng, i32 bound) {
    u32 randU32 = getRandomU32(rng);
    u32 result = randU32 % bound;
    return result;
}

typedef struct GenerateSequencesResult {
    prb_String  full;
    prb_String* seqs;
    i32         seqCount;
} GenerateSequencesResult;

function GenerateSequencesResult
generateSequences(prb_Arena* arena, Rng* rng, char* choices, i32 choicesCount, i32 lengthFullSeq, i32 seqCount) {
    char* fullSeqBuf = prb_arenaAllocArray(arena, char, lengthFullSeq + 1);
    for (i32 aaIndex = 0; aaIndex < lengthFullSeq; aaIndex++) {
        u32  choiceIndex = getRandomU32BiasedBound(rng, choicesCount);
        char choice = choices[choiceIndex];
        fullSeqBuf[aaIndex] = choice;
    }
    fullSeqBuf[lengthFullSeq] = '\0';
    prb_String fullSeq = {fullSeqBuf, lengthFullSeq};

    i32  maxTrimFromEnds = lengthFullSeq / 10;
    i32  maxMutations = lengthFullSeq / 10;
    i32* mutationBuffer = prb_arenaAllocArray(arena, i32, maxMutations);

    prb_String* seqs = prb_arenaAllocArray(arena, prb_String, seqCount);
    for (i32 seqIndex = 0; seqIndex < seqCount; seqIndex++) {
        i32 startIndex = getRandomU32BiasedBound(rng, maxTrimFromEnds + 1);
        i32 onePastEndIndex = lengthFullSeq - getRandomU32BiasedBound(rng, maxTrimFromEnds + 1);
        i32 seqLen = onePastEndIndex - startIndex;
        i32 mutationCount = getRandomU32BiasedBound(rng, maxMutations + 1);
        for (i32 mutationIndex = 0; mutationIndex < mutationCount; mutationIndex++) {
            mutationBuffer[mutationIndex] = getRandomU32BiasedBound(rng, seqLen);
        }

        char* seqBuf = prb_arenaAllocArray(arena, char, onePastEndIndex - startIndex + 1);
        for (i32 seqIndex = 0; seqIndex < seqLen; seqIndex++) {
            bool isMutated = false;
            for (i32 mutationIndex = 0; mutationIndex < mutationCount && !isMutated; mutationIndex++) {
                if (seqIndex == mutationBuffer[mutationIndex]) {
                    isMutated = true;
                }
            }

            if (isMutated) {
                u32  choiceIndex = getRandomU32BiasedBound(rng, choicesCount);
                char choice = choices[choiceIndex];
                seqBuf[seqIndex] = choice;
            } else {
                seqBuf[seqIndex] = fullSeqBuf[seqIndex + startIndex];
            }
        }
        seqBuf[seqLen] = '\0';
        seqs[seqIndex] = (prb_String) {seqBuf, seqLen};
    }

    GenerateSequencesResult result = {.full = fullSeq, .seqs = seqs, .seqCount = seqCount};
    return result;
}

function prb_String*
getSeqsFromFile(prb_Arena* arena, prb_String filepath) {
    prb_String*              seqs = 0;
    prb_ReadEntireFileResult readRes = prb_readEntireFile(arena, filepath);
    prb_assert(readRes.success);
    prb_String       contentLeft = prb_strFromBytes(readRes.content);
    prb_LineIterator lineIter = prb_createLineIter(contentLeft);
    while (prb_lineIterNext(&lineIter) == prb_Success) {
        prb_assert(lineIter.curLine.ptr[0] == '>');
        prb_assert(prb_lineIterNext(&lineIter) == prb_Success);
        arrput(seqs, lineIter.curLine);
    }
    return seqs;
}

function prb_String*
alignWithMafft(prb_Arena* arena, prb_String mafftExe, prb_String inputPath, prb_String mafftOuputPath) {
    {
        prb_String cmd = prb_fmt(arena, "%.*s --globalpair --maxiterate 1000 %.*s", prb_LIT(mafftExe), prb_LIT(inputPath));
        prb_writelnToStdout(cmd);
        prb_ProcessHandle proc = prb_execCmd(arena, cmd, prb_ProcessFlag_RedirectStdout, mafftOuputPath);
        prb_assert(proc.status == prb_ProcessStatus_CompletedSuccess);
    }
    prb_String* mafftAlignedSeqs = getSeqsFromFile(arena, mafftOuputPath);
    return mafftAlignedSeqs;
}

int tbfast_main(int argc, char** argv);

int
main() {
    prb_TimeStart testsStart = prb_timeStart();
    prb_Arena     arena_ = prb_createArenaFromVmem(1 * prb_GIGABYTE);
    prb_Arena*    arena = &arena_;
    Rng           rng_ = createRng(1, 3);
    Rng*          rng = &rng_;
    prb_String    testsDir = prb_getParentDir(arena, prb_STR(__FILE__));
    prb_String    rootDir = prb_getParentDir(arena, testsDir);

    GenerateSequencesResult genSeq = {};
    {
        char aminoAcids[] = {'A', 'R', 'N', 'D', 'C', 'E', 'Q', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V'};
        i32  aminoAcidsCount = prb_arrayLength(aminoAcids);
        prb_assert(aminoAcidsCount == 20);
        genSeq = generateSequences(arena, rng, aminoAcids, aminoAcidsCount, 10, 3);
        prb_writelnToStdout(genSeq.full);
        for (i32 seqIndex = 0; seqIndex < genSeq.seqCount; seqIndex++) {
            prb_writelnToStdout(genSeq.seqs[seqIndex]);
        }
    }

    prb_String fastaOutputPath = prb_pathJoin(arena, testsDir, prb_STR("testseqs.fasta"));
    {
        prb_GrowingString gstr = prb_beginString(arena);
        for (i32 seqIndex = 0; seqIndex < genSeq.seqCount; seqIndex++) {
            prb_addStringSegment(&gstr, ">virus%d\n%.*s\n", seqIndex, prb_LIT(genSeq.seqs[seqIndex]));
        }
        prb_String fastaContent = prb_endString(&gstr);
        prb_writeEntireFile(arena, fastaOutputPath, fastaContent.ptr, fastaContent.len);
    }

    prb_assert(prb_removeFileIfExists(arena, prb_pathJoin(arena, rootDir, prb_STR("mafft/core/logfile.txt"))) == prb_Success);

    // TODO(sen) Change to portable when available
    prb_String mafftBinEnvName = prb_STR("MAFFT_BINARIES");
    char*      oldMafftBin = getenv(mafftBinEnvName.ptr);
    prb_assert(oldMafftBin);
    prb_assert(unsetenv(mafftBinEnvName.ptr) == 0);
    prb_String  mafftOuputPath = prb_pathJoin(arena, testsDir, prb_STR("mafft-testseqs.fasta"));
    prb_String* mafftAlignedSeqs = alignWithMafft(arena, prb_STR("mafft"), fastaOutputPath, mafftOuputPath);
    prb_assert(arrlen(mafftAlignedSeqs) == genSeq.seqCount);
    prb_assert(setenv(mafftBinEnvName.ptr, oldMafftBin, 1) == 0);

    prb_String  localMafftExe = prb_pathJoin(arena, rootDir, prb_STR("mafft/core/mafft.tmpl"));
    prb_String  localMafftOuputPath = prb_pathJoin(arena, testsDir, prb_STR("localmafft-testseqs.fasta"));
    prb_String* localMafftAlignedSeqs = alignWithMafft(arena, localMafftExe, fastaOutputPath, localMafftOuputPath);
    prb_assert(arrlen(localMafftAlignedSeqs) == genSeq.seqCount);

    for (i32 seqIndex = 0; seqIndex < genSeq.seqCount; seqIndex++) {
        prb_assert(prb_streq(mafftAlignedSeqs[seqIndex], localMafftAlignedSeqs[seqIndex]));
    }

    // NOTE(sen) Call what I pulled out directly
    prb_String tempDir = prb_pathJoin(arena, testsDir, prb_STR("tmp"));
    prb_assert(prb_clearDirectory(arena, tempDir));
    prb_String cwd = prb_getWorkingDir(arena);
    prb_assert(prb_setWorkingDir(arena, tempDir));
    const char** tbfastArgs = prb_getArgArrayFromString(arena, prb_fmt(arena, "/home/khvorova/Projects/sequencebox/build-debug/mafft/exes/tbfast _ -u 0.0 -l 2.7 -C 0 -b 62 -g -0.10 -f -2.00 -Q 100.0 -h 0.1 -A _ -+ 16 -W 0.00001 -V -1.53 -s 0.0 -C 0 -b 62 -f -1.53 -Q 100.0 -h 0 -F -l 2.7 -X 0.1 -i %.*s", prb_LIT(fastaOutputPath)));
    tbfast_main(arrlen(tbfastArgs), (char**)tbfastArgs);
    prb_assert(prb_setWorkingDir(arena, cwd));

    prb_String* tbfastDirectAlignedSeqs = getSeqsFromFile(arena, prb_pathJoin(arena, tempDir, prb_STR("pre")));
    prb_assert(arrlen(tbfastDirectAlignedSeqs) == genSeq.seqCount);
    for (i32 seqIndex = 0; seqIndex < genSeq.seqCount; seqIndex++) {
        prb_assert(prb_streq(mafftAlignedSeqs[seqIndex], tbfastDirectAlignedSeqs[seqIndex]));
    }

    prb_writelnToStdout(prb_fmt(arena, "tests took %.2fms", prb_getMsFrom(testsStart)));
    return 0;
}