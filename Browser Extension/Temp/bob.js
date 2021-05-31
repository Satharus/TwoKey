function compute(a, m, n)
{
    var r;
    var y = 1;
 
    while (m > 0)
    {
        r = m % 2;
        
        // fast exponention
        if (r === 1) {
            y = (y*a) % n;
        }
        a = a*a % n;
        m = m / 2;
    }
 
 if (y !== g && y != a) return y;
 else return compute(a, m, n);
}


var p = 997;
var g = 5;

var secret_bob =  Math.floor(Math.random()*Number.MAX_SAFE_INTEGER); 

var public_B = compute(g, secret_bob, p);

console.log(public_B);

var public_A = 514

var Key_B = compute(public_A, secret_bob, p);

console.log(Key_B)