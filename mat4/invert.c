
#include <debug.h>

#include "invert.h"

struct mat4 mat4_invert(
	struct mat4 z)
{
	#define a (z.a)
	#define b (z.b)
	#define c (z.c)
	#define d (z.d)
	#define e (z.e)
	#define f (z.f)
	#define g (z.g)
	#define h (z.h)
	#define i (z.i)
	#define j (z.j)
	#define k (z.k)
	#define l (z.l)
	#define m (z.m)
	#define n (z.n)
	#define o (z.o)
	#define p (z.p)
	
	double det =
		+ a * (+ f * (k * p - l * o)
		       - g * (j * p - l * n)
		       + h * (j * o - k * n))
		- b * (+ e * (k * p - l * o)
		       - g * (i * p - l * m)
		       + h * (i * o - k * m))
		+ c * (+ e * (j * p - l * n)
		       - f * (i * p - l * m)
		       + h * (i * n - j * m))
		- d * (+ e * (j * o - k * n)
		       - f * (i * o - k * m)
		       + g * (i * n - j * m));
	
	return (struct mat4) {
		(+ f * (k * p - l * o) - g * (j * p - l * n) + h * (j * o - k * n)) / det,
		(- b * (k * p - l * o) + c * (j * p - l * n) - d * (j * o - k * n)) / det,
		(+ b * (g * p - h * o) - c * (f * p - h * n) + d * (f * o - g * n)) / det,
		(- b * (g * l - h * k) + c * (f * l - h * j) - d * (f * k - g * j)) / det,
		(- e * (k * p - l * o) + g * (i * p - l * m) - h * (i * o - k * m)) / det,
		(+ a * (k * p - l * o) - c * (i * p - l * m) + d * (i * o - k * m)) / det,
		(- a * (g * p - h * o) + c * (e * p - h * m) - d * (e * o - g * m)) / det,
		(+ a * (g * l - h * k) - c * (e * l - h * i) + d * (e * k - g * i)) / det,
		(+ e * (j * p - l * n) - f * (i * p - l * m) + h * (i * n - j * m)) / det,
		(- a * (j * p - l * n) + b * (i * p - l * m) - d * (i * n - j * m)) / det,
		(+ a * (f * p - h * n) - b * (e * p - h * m) + d * (e * n - f * m)) / det,
		(- a * (f * l - h * j) + b * (e * l - h * i) - d * (e * j - f * i)) / det,
		(- e * (j * o - k * n) + f * (i * o - k * m) - g * (i * n - j * m)) / det,
		(+ a * (j * o - k * n) - b * (i * o - k * m) + c * (i * n - j * m)) / det,
		(- a * (f * o - g * n) + b * (e * o - g * m) - c * (e * n - f * m)) / det,
		(+ a * (f * k - g * j) - b * (e * k - g * i) + c * (e * j - f * i)) / det,
	};
}









