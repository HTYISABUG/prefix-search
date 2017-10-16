#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <stdint.h>

#include "tst.h"

/** constants insert, delete, max word(s) & stack nodes */
enum { INS, DEL, WRDMAX = 256, STKMAX = 512, LMAX = 1024 };
#define REF INS
#define CPY DEL

/* timing helper function */
static double tvgetf(void)
{
    struct timespec ts;
    double sec;

    clock_gettime(CLOCK_REALTIME, &ts);
    sec = ts.tv_nsec;
    sec /= 1e9;
    sec += ts.tv_sec;

    return sec;
}

/* simple trim '\n' from end of buffer filled by fgets */
static void rmcrlf(char *s)
{
    size_t len = strlen(s);
    if (len && s[len - 1] == '\n')
        s[--len] = 0;
}

#define IN_FILE "cities.txt"
#define MEMPOOL_SIZE 10000000

// options for getopt
const char shortopts[] = "";
const struct option longopts[] = {
    {"bench", no_argument, NULL, 'b'},
    {0, 0, 0, 0}
};

// better timing helper function
static inline __attribute__((always_inline))
void get_cycles(unsigned *high, unsigned *low)
{
    asm volatile ("CPUID\n\t"
                  "RDTSC\n\t"
                  "mov %%edx, %0\n\t"
                  "movl %%eax, %1\n\t": "=r" (*high), "=r" (*low)::"%rax","%rbx","%rcx","%rdx"
                 );
}

static inline __attribute__((always_inline))
void get_cycles_end(unsigned *high, unsigned *low)
{
    asm volatile("RDTSCP\n\t"
                 "mov %%edx, %0\n\t"
                 "mov %%eax, %1\n\t"
                 "CPUID\n\t": "=r" (*high), "=r" (*low)::"%rax","%rbx","%rcx","%rdx"
                );
}

static inline __attribute__((always_inline))
uint64_t diff_in_cycles(unsigned high1, unsigned low1,
                        unsigned high2, unsigned low2)
{
    uint64_t start,end;
    start = (((uint64_t) high1 << 32) | low1);
    end = (((uint64_t) high2 << 32) | low2);
    return end - start;
}

int main(int argc, char **argv)
{
    // getopt
    int opt;
    int bench_flag = 0;

    while ((opt = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
        switch (opt) {
        case 'b':
            bench_flag = 1;
            break;
        case '?':
            printf(
                "\nUsage:\n"
                "    ./test_ref [options]\n\n"
                "Options:\n"
                "    --bench        auto input, for benchmark\n"
            );
            return EXIT_FAILURE;
            break;
        default:
            return EXIT_FAILURE;
        }
    }

    // main code
    char word[WRDMAX] = "";
    char *sgl[LMAX] = {NULL};
    tst_node *root = NULL, *res = NULL;
    int idx = 0, sidx = 0;
    FILE *fp = fopen(IN_FILE, "r"), *tp;
    double t1, t2;
    uint64_t timec, timec_all = 0;
    unsigned timec_high1, timec_high2, timec_low1, timec_low2;

    // memory for string
    char *pool = (char *) malloc(MEMPOOL_SIZE * sizeof(char));

    // pointer to pool's top
    char *pptr = pool;

    if (!fp) { /* prompt, open, validate file for reading */
        fprintf(stderr, "error: file open failed '%s'.\n", argv[1]);
        return 1;
    }

    /*t1 = tvgetf();*/
    tp = fopen("append_ref.txt", "w");
    while (fscanf(fp, "%s", pptr) != EOF) {
        char *p = pptr;
        /* FIXME: insert reference to each string */
        get_cycles(&timec_high1, &timec_low1);
        if (!tst_ins_del(&root, &p, INS, REF)) {
            fprintf(stderr, "error: memory exhausted, tst_insert.\n");
            fclose(tp);
            fclose(fp);
            return 1;
        }
        get_cycles_end(&timec_high2, &timec_low2);
        timec = diff_in_cycles(timec_high1, timec_low1, timec_high2, timec_low2);
        timec_all += timec;
        int len = strlen(pptr);
        pptr += (len + 1);
        idx++;
        fprintf(tp, "%d %lu %d\n", idx, timec, len);
    }
    /*t2 = tvgetf();*/

    fclose(tp);
    fclose(fp);
    printf("ternary_tree, loaded %d words in %lu cycles\n", idx, timec_all);

    if (bench_flag) {
        fp = fopen(IN_FILE, "r");
        tp = fopen("prefix_search_ref.txt", "w");
        timec_all = 0;
        int cnt = 0;
        while (fscanf(fp, "%3s", word) != EOF) {
            rmcrlf(word);
            get_cycles(&timec_high1, &timec_low1);
            res = tst_search_prefix(root, word, sgl, &sidx, LMAX);
            get_cycles_end(&timec_high2, &timec_low2);
            timec = diff_in_cycles(timec_high1, timec_low1, timec_high2, timec_low2);
            timec_all += timec;
            fprintf(tp, "%d %lu %d\n", ++cnt, timec, sidx);
            if (res) {
                printf("  %s - searched prefix in %lu cycles\n\n", word, timec);
            } else
                printf("  %s - not found\n", word);
        }
        printf("ternary_tree, searched %d words in %lu cycles\n", cnt, timec_all);
        fclose(tp);
        fclose(fp);
        tst_free(root);
        free(pool);
        return EXIT_SUCCESS;
    }

    for (;;) {
        printf(
            "\nCommands:\n"
            " a  add word to the tree\n"
            " f  find word in tree\n"
            " s  search words matching prefix\n"
            " d  delete word from the tree\n"
            " q  quit, freeing all data\n\n"
            "choice: ");
        fgets(word, sizeof word, stdin);

        switch (*word) {
            char *p = NULL;
        case 'a':
            printf("enter word to add: ");
            if (!fgets(pptr, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(pptr);
            p = pptr;
            t1 = tvgetf();
            /* FIXME: insert reference to each string */
            res = tst_ins_del(&root, &p, INS, REF);
            t2 = tvgetf();
            if (res) {
                pptr += (strlen(pptr) + 1);
                idx++;
                printf("  %s - inserted in %.6f sec. (%d words in tree)\n",
                       (char *) res, t2 - t1, idx);
            } else
                printf("  %s - already exists in list.\n", (char *) res);
            break;
        case 'f':
            printf("find word in tree: ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            t1 = tvgetf();
            res = tst_search(root, word);
            t2 = tvgetf();
            if (res)
                printf("  found %s in %.6f sec.\n", (char *) res, t2 - t1);
            else
                printf("  %s not found.\n", word);
            break;
        case 's':
            printf("find words matching prefix (at least 1 char): ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            t1 = tvgetf();
            res = tst_search_prefix(root, word, sgl, &sidx, LMAX);
            t2 = tvgetf();
            if (res) {
                printf("  %s - searched prefix in %.6f sec\n\n", word, t2 - t1);
                for (int i = 0; i < sidx; i++)
                    printf("suggest[%d] : %s\n", i, sgl[i]);
            } else
                printf("  %s - not found\n", word);
            break;
        case 'd':
            printf("enter word to del: ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            p = word;
            printf("  deleting %s\n", word);
            t1 = tvgetf();
            /* FIXME: remove reference to each string */
            res = tst_ins_del(&root, &p, DEL, REF);
            t2 = tvgetf();
            if (res)
                printf("  delete failed.\n");
            else {
                printf("  deleted %s in %.6f sec\n", word, t2 - t1);
                idx--;
            }
            break;
        case 'q':
            /*tst_free_all(root);*/
            tst_free(root);
            free(pool);
            return 0;
            break;
        default:
            fprintf(stderr, "error: invalid selection.\n");
            break;
        }
    }

    return 0;
}
