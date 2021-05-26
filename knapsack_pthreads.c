#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#define MAX 1500

int max(int a, int b)
{
    return a > b ? a : b;
}

int thread_count;
int current;
int **dp;
int capacity;
int *v;
int *w;

void create_problem(int N, int max_, int weight[N], int value[N])
{
    int i;
    for (i = 0; i < N; i++)
    {
        w[i] = (rand() % max_) + 1;
        weight[i] = w[i];
        v[i] = (rand() % max_) + 1;
        value[i] = v[i];
    }
}

void Pth_mat_vect(void rank)
{
    long my_rank = (long)rank;
    int i, j;
    int local_m = capacity / thread_count;
    int my_first_col = my_rank * local_m;
    int my_last_col = (my_rank + 1) * local_m - 1;
    if (my_first_col == 0)
    {
        my_first_col = 1;
    }
    if (my_rank == thread_count - 1)

    {
        printf("8888\n");
        for (j = my_first_col; j <= capacity - 1; j++)
        {
            dp[current][j] = dp[!current][j];
            if (j - w[i - 1] >= 0)
            {
                dp[current][j] = max(dp[current][j], v[i - 1] + dp[!current][j - w[i - 1]]);
            }
        }
    }
    else
    {
        printf("9999\n");
        for (j = my_first_col; j <= my_last_col; j++)
        {
            dp[current][j] = dp[!current][j];
            if (j - w[i - 1] >= 0)
            {
                dp[current][j] = max(dp[current][j], v[i - 1] + dp[!current][j - w[i - 1]]);
            }
        }
    }

    printf("Hellllllllo %d \n", my_rank);
    return NULL;
}

int solve(int N, int C, int *w, int *v)
{
    long i, j;
    dp = (int **)malloc(2 * sizeof(int *));
    for (i = 0; i < 2; i++)
    {
        dp[i] = (int *)malloc(MAX * sizeof(int));
    }

    for (j = 0; j <= C; j++)
    {
        dp[0][j] = 0;
        dp[1][j] = 0;
    }

    // Start on the second row.
    pthread_t *thread_handles;
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    int current = 1;
    for (i = 1; i <= N; i++)
    {
        printf("\n%d\n", i);
        for (i = 0; i < thread_count; i++)
        {
            pthread_create(&thread_handles[i], NULL, Pth_mat_vect, (void *)i);
        }
        for (j = 0; j < thread_count; j++)
        {
            pthread_join(thread_handles[j], NULL);
        }
        current = !current;
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

    printf("Enter the capacity of the knapsack and number of items\n");
    fflush(stdin);
    scanf("%d %d", &C, &N);

    capacity = C;
    printf("Hello");

    // printf("Enter the number of items:\n");
    // scanf("%d", &N);

    printf("Enter max weight and value of an item\n");
    fflush(stdin);
    scanf("%d", &max_weights);

    printf("Hi");

    int weight[N], value[N];

    v = (int *)malloc(N * sizeof(int));
    w = (int *)malloc(N * sizeof(int));

    printf("Hi2");

    thread_count = 4;

    create_problem(N, max_weights, weight, value);

    printf("Hi3");

    gettimeofday(&tstart, NULL);
    ans = solve(N, C, weight, value);
    gettimeofday(&tend, NULL);

    printf("Hi4");

    t1 = check_time(tstart, tend);
    printf("Create method finished in %lf seconds.\n", t1 / 1000000.0);

    gettimeofday(&tstart, NULL);
    solve2(N, C, weight, value);
    gettimeofday(&tend, NULL);

    t2 = check_time(tstart, tend);

    double s_scaling = t2 / (thread_count * t1);

    printf("Swap finished in %lf seconds.\n", t2 / 1000000.0);

    // printf("weights:\n");
    // for (int i = 0; i < N; i++)
    // {
    //     printf("%d\t", weight[i]);
    // }
    // printf("values:\n");
    // for (int i = 0; i < N; i++)
    // {
    //     printf("%d\t", value[i]);
    // }
    // printf("Max profit with value : %d \n", ans);
    // printf("Total time taken : %lf\n", s_scaling);
}