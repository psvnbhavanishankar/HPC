
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#define MAX 1500

int max(int a, int b)
{
    return a > b ? a : b;
}

int thread_count;

void create_problem(int N, int max_, int w[N], int v[N])
{
    int i;

    for (i = 0; i < N; i++)
    {
        w[i] = (rand() % max_) + 1;
        v[i] = (rand() % max_) + 1;
    }
}

int solve(int N, int C, int *w, int *v)
{
    int i, j;
    int dp[2][MAX];
    for (j = 0; j <= C; j++)
    {
        dp[0][j] = 0;
        dp[1][j] = 0;
    }

    // Start on the second row.

    int current = 1;
    for (i = 1; i <= N; i++)
    {
#pragma omp parallel for num_threads(thread_count)
        for (j = 1; j <= C; j++)
        {
            dp[current][j] = dp[!current][j];
            if (j - w[i - 1] >= 0)
            {
                dp[current][j] = max(dp[current][j], v[i - 1] + dp[!current][j - w[i - 1]]);
            }
        }
        current = !current;

        // * After the computation for whole j is done then at the end current is replaced with the maximum current value.
    }

    return dp[!current][C];
}

int solve2(int n, int c, int weight[MAX], int profit[MAX])
{
    int i, j;
    int total[2][MAX];
    int current = 0;
    /* Max profit, if it fits */
    for (j = 0; j < c; j++)
    {
        if (weight[0] > j)
        {
            total[current][j] = 0;
        }
        else
        {
            total[current][j] = profit[0];
        }
    }

    for (i = 1; i < n; i++)
    {
        for (j = 0; j < c; j++)
        {
            if ((j < weight[i]) ||
                (total[!current][j] >= total[!current][j - weight[i]] + profit[i]))
            {
                total[current][j] = total[!current][j];
            }
            else
            {
                total[current][j] = total[!current][j - weight[i]] + profit[i];
            }
        }
        current = !current;
    }

    return total[!current][c - 1];
}

double check_time(struct timeval tstart, struct timeval tend)
{
    unsigned long long usec;
    if (tend.tv_usec > tstart.tv_usec)
    {
        usec = (tend.tv_sec - tstart.tv_sec) * 1000000 + tend.tv_usec - tstart.tv_usec;
    }
    else
    {
        usec = (tend.tv_sec - (tstart.tv_sec + 1)) * 1000000 + (1000000 + tend.tv_usec - tstart.tv_usec);
    }
    return (double)usec;
}

int main(int argc, char *argv[])
{
    int N, C, ans = 0;
    int max_weights;

    struct timeval tstart, tend;
    double t1, t2;

    printf("Enter the capacity of the knapsack\n");
    scanf("%d", &C);

    printf("Enter the number of items:\n");
    scanf("%d", &N);

    printf("Enter max weight and value of an item\n");
    scanf("%d", &max_weights);

    int weight[N], value[N];

    thread_count = 6;

    // N = strtol(argv[1], NULL, 10);
    // C = strtol(argv[2], NULL, 10);
    // max_weights = strtol(argv[3], NULL, 10);

    create_problem(N, max_weights, weight, value);

    gettimeofday(&tstart, NULL);
    ans = solve(N, C, weight, value);
    gettimeofday(&tend, NULL);

    t1 = check_time(tstart, tend);
    printf("Create method finished in %lf seconds.\n", t1 / 1000000.0);

    gettimeofday(&tstart, NULL);
    solve2(N, C, weight, value);
    gettimeofday(&tend, NULL);

    t2 = check_time(tstart, tend);

    double s_scaling = t2 / (thread_count * t1);

    printf("Swap finished in %lf seconds.\n", t2 / 1000000.0);

    printf("weights:\n");
    for (int i = 0; i < N; i++)
    {
        printf("%d\t", weight[i]);
    }
    printf("values:\n");
    for (int i = 0; i < N; i++)
    {
        printf("%d\t", value[i]);
    }
    printf("Max profit with value : %d \n", ans);
    printf("Total time taken : %lf\n", s_scaling);
}