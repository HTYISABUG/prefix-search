#include <cstdio>
#include <cstdint>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <tuple>
#include <algorithm>
using namespace std;

#define CRIT_VALUE 1.96

typedef tuple<int, uint64_t, int> tt;

bool len(tt a, tt b)
{
    return get<2>(a) < get<2>(b);
}

int main(void)
{
    /********** cpy **********/
    vector<tt> v;
    FILE *ip = fopen("append_cpy.txt", "r"), *op1, *op2;
    int t1, t3;
    uint64_t t2, n;
    double avg = 0, std = 0;

    while (fscanf(ip, "%d %lu %d", &t1, &t2, &t3) != EOF) {
        v.push_back(make_tuple(t1, t2, t3));
        avg += t2;
    }
    n = v.size();
    avg /= n;
    fclose(ip);

    auto begin = v.begin(), end = v.end();
    printf("average cycles: %lf cycles\n", avg);
    for (auto it = begin; it != end; it++) {
        std += pow((double)get<1>(*it) - avg, 2);
    }
    std = sqrt(std / n);
    printf("standard deviation: %lf\n", std);
    printf("confidence interval (95%%): (%lf, %lf)\n\n",
           avg - CRIT_VALUE * std / sqrt(n),
           avg + CRIT_VALUE * std / sqrt(n)
          );

    sort(begin, end, len);
    op1 = fopen("len_cpy.txt", "w");
    for (auto it = begin, head = begin; it != end; it++) {
        if (it + 1 == end) {
            fprintf(op1, "%d %ld\n", get<2>(*it), it - head + 1);
            head = it + 1;
        } else if (get<2>(*it) != get<2>(*(it + 1))) {
            fprintf(op1, "%d %ld\n", get<2>(*it), it - head + 1);
            head = it + 1;
        }
    }
    fclose(op1);

    op1 = fopen("data_cpy.txt", "w");
    op2 = fopen("ldata_cpy.txt", "w");
    for (auto it = begin; it != end; it++) {
        fprintf(get<2>(*it) <= 10 ? op1 : op2, "%d %lu\n", get<0>(*it), get<1>(*it));
    }
    fclose(op2);
    fclose(op1);

    op1 = fopen("high_cpy.txt", "w");
    int cnt = 0;
    for (auto it = begin; it != end; it++) {
        if (get<1>(*it) > 20000) {
            ++cnt;
        }

        if (it + 1 == end) {
            fprintf(op1, "%d %d\n", get<2>(*it), cnt);
            cnt = 0;
        } else if (get<2>(*it) != get<2>(*(it + 1))) {
            fprintf(op1, "%d %d\n", get<2>(*it), cnt);
            cnt = 0;
        }
    }
    fclose(op1);

    /********** ref **********/
    ip = fopen("append_ref.txt", "r");
    v.clear();
    avg = std = 0;
    while (fscanf(ip, "%d %lu %d", &t1, &t2, &t3) != EOF) {
        v.push_back(make_tuple(t1, t2, t3));
        avg += t2;
    }
    n = v.size();
    avg /= n;
    fclose(ip);

    begin = v.begin(), end = v.end();
    printf("average cycles: %lf cycles\n", avg);
    for (auto it = begin; it != end; it++) {
        std += pow((double)get<1>(*it) - avg, 2);
    }
    std = sqrt(std / n);
    printf("standard deviation: %lf\n", std);
    printf("confidence interval (95%%): (%lf, %lf)\n\n",
           avg - CRIT_VALUE * std / sqrt(n),
           avg + CRIT_VALUE * std / sqrt(n)
          );

    sort(begin, end, len);
    op1 = fopen("len_ref.txt", "w");
    for (auto it = begin, head = begin; it != end; it++) {
        if (it + 1 == end) {
            fprintf(op1, "%d %ld\n", get<2>(*it), it - head + 1);
            head = it + 1;
        } else if (get<2>(*it) != get<2>(*(it + 1))) {
            fprintf(op1, "%d %ld\n", get<2>(*it), it - head + 1);
            head = it + 1;
        }
    }
    fclose(op1);

    op1 = fopen("data_ref.txt", "w");
    op2 = fopen("ldata_ref.txt", "w");
    for (auto it = begin; it != end; it++) {
        fprintf(get<2>(*it) <= 10 ? op1 : op2, "%d %lu\n", get<0>(*it), get<1>(*it));
    }
    fclose(op2);
    fclose(op1);

    op1 = fopen("high_ref.txt", "w");
    cnt = 0;
    for (auto it = begin; it != end; it++) {
        if (get<1>(*it) > 20000) {
            ++cnt;
        }

        if (it + 1 == end) {
            fprintf(op1, "%d %d\n", get<2>(*it), cnt);
            cnt = 0;
        } else if (get<2>(*it) != get<2>(*(it + 1))) {
            fprintf(op1, "%d %d\n", get<2>(*it), cnt);
            cnt = 0;
        }
    }
    fclose(op1);

    return 0;
}
