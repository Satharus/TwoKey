
const getPrimes = (min, max) => {
   const result = Array(max + 1)
   .fill(0)
   .map((_, i) => i);
   for (let i = 2; i <= Math.sqrt(max + 1); i++) {
      for (let j = i ** 2; j < max + 1; j += i) delete result[j];
   }
   return Object.values(result.slice(min));
};
const getRandomNum = (min, max) => {
   return Math.floor(Math.random() * (max - min + 1) + min);
};
const getRandomPrime = ([min, max]) => {
   const primes = getPrimes(min, max);
   return primes[getRandomNum(0, primes.length - 1)];
};


// This program calculates the Key for two persons
// using the Diffie-Hellman Key exchange algorithm

// Power function to return value of a ^ b mod P
function power(a, b, p)
{
	if (b == 1)
		return a;
	else
		return((Math.pow(a, b)) % p);
}

// Driver code
var P, G, x, a, y, b, ka, kb;

// Both the persons will be agreed upon the
// public keys G and P

// A prime number P is taken
const range = [1, 100];
P=getRandomPrime(range);
document.write("The value of P:" + P + "<br>");

// A primitve root for P, G is taken
G = 9;
document.write("The value of G:" + G + "<br>");

// Alice will choose the private key a
// a is the chosen private key
a = getRandomNum(range);
document.write("The private key a for Alice:" +
			a + "<br>");

// Gets the generated key
x = power(G, a, P);

// Bob will choose the private key b
// b is the chosen private key
b = getRandomNum(range);
document.write("The private key b for Bob:" +
			b + "<br>");

// Gets the generated key
y = power(G, b, P);

// Generating the secret key after the exchange
// of keys
ka = power(y, a, P); // Secret key for Alice
kb = power(x, b, P); // Secret key for Bob

document.write("Secret key for the Alice is:" +
			ka + "<br>");
document.write("Secret key for the Bob is:" +
			kb + "<br>");




