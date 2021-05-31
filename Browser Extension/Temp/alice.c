#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int compute(int a, int m, int n)
{
    int r;
    int y = 1;
 
    while (m > 0)
    {
        r = m % 2;
        
        // fast exponention
        if (r == 1) {
            y = (y*a) % n;
        }
        a = a*a % n;
        m = m / 2;
    }
 
    return y;
}

int main()
{
    srand(time(0));
    int p = 997;
    int g = 5;
    int secret_alice = rand();
    
    int public_A = compute(g, secret_alice, p);

    printf("Alice's Public Key: %d\n", public_A);
    int public_B;
    puts("Bob's Public Key: ");
    scanf("%d", &public_B);

    int key_A = compute(public_B, secret_alice, p);
    printf("Alice's Secret Key: %d\n", key_A);
}
