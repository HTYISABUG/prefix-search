#include <cstdio>
#include <tuple>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

#define CRIT_VALUE 1.96
#define INTERVAL 50

typedef tuple<int, uint64_t, int> tt;

bool num(tt a, tt b)
{
    return get<2>(a) < get<2>(b);
}

int main(void)
{
    /********** cpy **********/
    vector<tt> v;
    FILE *ip = fopen("prefix_search_cpy.txt", "r"), *op;
    int t1, t3;
    uint64_t t2, n;
    double avg = 0, std = 0;

    // calcute avg & std
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

    // calcute avg & std per 50 found data
    sort(begin, end, num);
    op = fopen("prefix_search_avg_cpy.txt", "w");
    avg = std = 0;
    int cnt = 0;
    printf("|interval|data amount|average cycles|standard deviation|confidence interval (95%%)|\n");
    printf("|-|-|-|-|-|\n");
    for (auto it = begin, head = begin; it != end; it++) {
        if (get<1>(*it) < 700000) {
            avg += get<1>(*it);
        } else {
            ++cnt;
        }
        int lower = get<2>(*it) / INTERVAL * INTERVAL,
            upper = get<2>(*(it + 1)) / INTERVAL * INTERVAL;

        if (it + 1 == end || lower != upper) {
            cnt = it - head + 1 - cnt;
            if (cnt) {
                avg /= cnt;
                for (auto iter = head; iter != it + 1; iter++) {
                    std += pow((double)get<1>(*iter) - avg, 2);
                }
                std = sqrt(std / cnt);
                printf("|%d~%d", lower, lower + 50);
                printf("|%d", cnt);
                printf("|%lu", (uint64_t)avg);
                printf("|%lu", (uint64_t)std);
                printf("|(%ld, %ld)|\n",
                       (int64_t)(avg - CRIT_VALUE * std / sqrt(cnt)),
                       (int64_t)(avg + CRIT_VALUE * std / sqrt(cnt))
                      );
                fprintf(op, "%d %lf %lf %lf\n",
                        (lower * 2 + 50) / 2,
                        avg,
                        avg - CRIT_VALUE * std / sqrt(cnt),
                        avg + CRIT_VALUE * std / sqrt(cnt)
                       );
            } else {
                printf("|%d~%d", lower, lower + 50);
                printf("|%d", cnt);
                printf("|0");
                printf("|0");
                printf("|(0, 0)|\n");
                fprintf(op, "%d %lf %lf %lf\n", (lower * 2 + 50) / 2, 0.0, 0.0, 0.0);
            }
            head = it + 1;
            avg = cnt = 0;
        }
    }
    fclose(op);

    /********** ref **********/
    // calcute avg & std
    ip = fopen("prefix_search_ref.txt", "r");
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

    // calcute avg & std per 50 found data
    sort(begin, end, num);
    op = fopen("prefix_search_avg_ref.txt", "w");
    avg = std = 0;
    cnt = 0;
    printf("|interval|data amount|average cycles|standard deviation|confidence interval (95%%)|\n");
    printf("|-|-|-|-|-|\n");
    for (auto it = begin, head = begin; it != end; it++) {
        if (get<1>(*it) < 700000) {
            avg += get<1>(*it);
        } else {
            ++cnt;
        }
        int lower = get<2>(*it) / INTERVAL * INTERVAL,
            upper = get<2>(*(it + 1)) / INTERVAL * INTERVAL;

        if (it + 1 == end || lower != upper) {
            cnt = it - head + 1 - cnt;
            if (cnt) {
                avg /= cnt;
                for (auto iter = head; iter != it + 1; iter++) {
                    std += pow((double)get<1>(*iter) - avg, 2);
                }
                std = sqrt(std / cnt);
                printf("|%d~%d", lower, lower + 50);
                printf("|%d", cnt);
                printf("|%lu", (uint64_t)avg);
                printf("|%lu", (uint64_t)std);
                printf("|(%ld, %ld)|\n\n",
                       (int64_t)(avg - CRIT_VALUE * std / sqrt(cnt)),
                       (int64_t)(avg + CRIT_VALUE * std / sqrt(cnt))
                      );
                fprintf(op, "%d %lf %lf %lf\n",
                        (lower * 2 + 50) / 2,
                        avg,
                        avg - CRIT_VALUE * std / sqrt(cnt),
                        avg + CRIT_VALUE * std / sqrt(cnt)
                       );
            } else {
                printf("|%d~%d", lower, lower + 50);
                printf("|%d", cnt);
                printf("|0");
                printf("|0");
                printf("|(0, 0)|\n\n");
                fprintf(op, "%d %lf %lf %lf\n", (lower * 2 + 50) / 2, 0.0, 0.0, 0.0);
            }
            head = it + 1;
            avg = cnt = 0;
        }
    }
    fclose(op);

    return 0;
}
