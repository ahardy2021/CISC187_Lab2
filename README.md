1. How many steps would it take to perform a linear search for the number 8 in the ordered array, `[2, 4, 6, 8, 10, 12, 13]`?
4 steps:
	1. Is `arr[0] (2) == 8`? X
	2. Is `arr[1] (4) == 8`? X
	3. Is `arr[2] (6) == 8`? X
	4. Is `arr[3] (8) == 8`? √ (STOP)

2. How many steps would binary search take for the previous example?
Also 4 steps:
	1. `mid = (start+end)/2 (3)`. Is `arr[mid] (8) < 8`? X, `end = mid (3)`.
	2. `mid = (start+end)/2 (1)`. Is `arr[mid] (4) < 8`? √, `start = mid + 1 (2)`.
	3. `mid = (start+end)/2 (2)`. Is `arr[mid] (6) < 8`? √, `start = mid + 1 (3)`.
	4. Escape, since `start == end`. Only possible value is `start`, so return `arr[start] == 8`.

3. What is the maximum number of steps it would take to perform a binary search on an array of size 100,000?
Should be 18 as a result of being the solution to `1 + ceil(log_2 100,000)`
	1.  100,000 elements -> 50,000 max.
	2.  50,000 elements -> 25,000 max.
	3.  25,000 elements -> 12,500 max.
	4.  12,500 elements -> 6,250 max.
	5.  6,250 elements -> 3,125 max.
	6.  3,125 elements -> 1,563 max.
	7.  1,563 elements -> 782 max.
	8.  782 elements -> 391 max.
	9.  391 elements -> 196 max
	10. 196 elements -> 98 max.
	11. 98 elements -> 49 max.
	12. 49 elements -> 25 max.
	13. 25 elements -> 13 max.
	14. 13 elements -> 7 max.
	15. 7 elements -> 4 max.
	16. 4 elements -> 2 max.
	17. 2 elements -> 1 max.
	18. 1 element -> Check for equality.

4. See comments.
5. See comments.
