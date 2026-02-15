#include <iostream>
#include <random>
#include <vector>

#ifndef incCmp
#define incCmp ((*cmp)++ || true)
// this header is to help me track comparisons using a pointer
#endif

#include "main.hpp"

using namespace std;

std::random_device rd;
std::mt19937 gen(rd());

/**
 * Shuffles an array.
 * @tparam T Generic type
 * @param len Length of the array to be shuffled
 * @param arr Array to be shuffled
 * @param cmp Comparison counter pointer. not incremented, but passed through to show i would increment if i needed to
 * @remark O(N) complexity
 */
template<typename T> void shuffleArr(const size_t len, T arr[], size_t *cmp) {
    std::uniform_int_distribution<size_t> next(0, len - 1);
    // algorithm for shuffle taken from C++ documentation
    for (size_t i = 0; len - i; i++) {
        // get random to swap with
        const size_t index = next(gen);
        // swap
        T buf = arr[i];
        arr[i] = arr[index];
        arr[index] = buf;
    }
}

/**
 * Linear search implementation.
 * @tparam T Generic type for generic reasons
 * @param vec Vector that is being searched
 * @param item Item that is being searched for
 * @param cmp Pointer to size_t for tracking comparisons.
 * @return Whether the item was found.
 * @remark Best case: hit O(1), miss O(n)
 * @remark Average case: hit O(n), miss O(n)
 * @remark Worst case: hit O(n), miss O(n)
 */
template<typename T> bool linearSearch(const vector<T> vec, const T item, size_t *cmp) {
    const size_t len = vec.size();
    for (size_t i = 0; len - i; i++) {
        if (incCmp && vec[i] == item) {return true;}
    }
    return false;
}
/*
 * As for time complexity: we could represent this by doing a step-by-step haskell function.
 * linSearchCplx :: Int -> Int
 * linSearchCplx 1 = 1
 * linSearchCplx n = 1 + linSearchCplx (n - 1)
 * This strongly resembles a linear function.
 */
// Break for autodoc
/**
 * Binary search.
 * @tparam T Type of the collection. Must implement operator<, operator>
 * @param arr SORTED array/vector to be searched
 * @param item Item that is being searched for
 * @param cmp pointer to comparison hold
 * @return whether the item was in the collection
 * @remark All case hit/miss: O(log n).
 */
template<typename T> bool binarySearch(const vector<T> arr, const T item, size_t *cmp) {
    const size_t len = arr.size();
    if (arr.empty()) return false; // because integer underflow
    // i didn't use an == operator, so i'm not counting it. sue me
    size_t start = 0;
    size_t end = len - 1;
    // while (incCmp && start < end) {
    // this wastes a comparison, we can do better
    while (end - start) {
        // i don't see an equals sign... i'm not counting it
        const size_t mid = (start + end) / 2; // rounds down
        if (incCmp && arr[mid] < item) { // we're too *low*
            start = mid+1; // pushes up
            continue;
        }
        end = mid; // pushes up
    }
    return incCmp && arr[end] == item;
}
/*
 * As for the time complexity being similar to O(log N):
 * In each step, the amount of elements you search is approximately halved.
 * In haskell, we could write the following function to represent this complexity:
 * binSearchCplx :: Int -> Int
 * binSearchCplx 1 = 1
 * binSearchCplx n = 1 + binSearchCplx (n/2)
 * This fairly straightforwardly resembles a rounded-up log_2 function, thus being proportional to log(N).
 */


/**
 * Idea 1: Basic implementation using vector
 * bool bogoSearch(arr, item)
 * | vec checked = empty // O(1)
 * | while(length of checked < len) // TIMES N
 * | | int index // O(1)
 * | | do // TIMES len/(len-L)
 * | | | index = randint(0, len-1)
 * | | while index in checked // O(N)
 * | | checked.push(index) // O(1)
 * | | if arr[index] == item // O(1)
 * | | | return true // O(1)
 * | return false
 * // Time complexity:
 * Best O(1) hit / O(n) miss
 * Average O(n^3 log N) hit / O(n^3 log N) miss. Yikes!
 * Worst O(infinite) hit/miss. well that's to be expected tbh
 * // That's pretty bad, bogosort tier. at least it doesn't check everything
 * // I don't even know how to take the average properly jeez
 * // Post coding mindset here: we cheated by using a non-self unordered search algo. oops
**/

/**
 * Idea 2: Sorted vector to use binary search
 * bool betterBogoSearch(arr, item)
 * | vec checked = empty // O(1)
 * | indices = empty
 * | while(length of checked < len) // TIMES 1|N|N
 * | | int idx // O(1)
 * | | do // TIMES len/(len-L)
 * | | | idx = randint(0, len-1)
 * | | while binSearch(len, idx) // TIMES sum[2..n] ln(k) --> n log n, binsearch = log n -> n log^2 n
 * | | checked.insertSorted(idx) // O(N)
 * | | if arr[index] == item // we're doing a search, but it's not a search on the *array*
 * | | | return true // it's only for internals
 * | return false
 *
 * Time complexity:
 * Best: O(1) hit / O(n) miss
 * Average: O(n^2 log^2 n) hit miss
 * Worst: O(infinity) because runtime RNG
 *
**/

/**
 * Idea 3: Ok what if we ONLY used bogoSearch as our search algorithm
 * because then we aren't cheating by using a different search algorithm
 * call that bogoBogoSearch for bogoSearching in the bogoSearch
 * bool bogoBogoSearch(arr, item)
 * | vec checked = empty
 * | while(length of checked < length of arr)
 * | | int idx
 * | | do
 * | | | idx = randint(0, length of arr - 1)
 * | | while bogoBogoSearch(checked, idx)
 * | | checked.push(idx)
 * | | if arr[index] == item
 * | | | return true
 * | return false
 * Best = O(1)
 * Average = (half iterations)
 * Well this one's a doozy due to the recursion
 * Given a particular iteration n, we will have to successfully generate k random numbers
 * BUT because we always have to assume we went through and didn't find anything (worst case)
 * this actually requires us to take the WORST case of bogoBogoSort(N)
 * so it becomes sum[k = 1 .. n/2] WORST(k) * sum[j = 1..k] k/j (for reasons that are explained below)
 * Worst = (ALL iterations)
 * so let's see how bad this is.
 * Every time we're checking to see if a number isn't in there, we roll a die.
 * If it rolls a number we've already seen, we already got that one. roll again.
 * My theory for this is that we sum a binomial distribution --
 * for a six sided die, 6/6 + 6/5 + 6/4 + 6/3 + 6/2 + 6/1
 * this will be the average number of rolls we need to get all values
 * so we create a series -- sum[k = 1..max] max/k.
 * ignoring runtime RNG, which is necessary if the program is to terminate,
 * this works surprisingly well as an estimation.
 * So... that's it. let's make the sum.
 * WORST(n) = sum[k = 1..n] WORST(n-1) * sum[j = 1..k] k/j
 * having done the first hundred terms in python, this function seems similar to having logarithmic growth
 * and as i was doing this and chatting with the math club, i realized i could find the base of this logarithm
 * by checking values that were around 1 difference apart. so i selected 84 (>6) and 32(>5), 84/32 = ... 2.625 wait a minute
 * so i had missed that this was an exponential growth problem in disguise
 * and the growth between terms is 1 + ln(e)
 * which gives us... sum[k = 1..n] WORST(n-1) * (k + sum[j = 1..k] ln j)
 * and because the recursion is just multiplication, we can get...
 * sum[k = 1..j] prod[m = 1..k] m + sum[p = 1..m] ln p
 * integrating the innermost for limit behavior, we get
 * sum[k = 1..j] prod[m = 1..k] m + m (log m - 1). huh, that's m log m
 * sum[k = 1..j] prod[m = 2..k] m log m
 * let's separate this into sum[k = 1..j] prod[m = 1..k] m * prod[m = 1..k] log m
 * then we get sum[k = 1..j] m! * prod[m = 2..k] ln m
 * if we want to compress the latter into a sequence we can try
 * ln (2 ^ ln (3 ^ ln (4 ^ ln(5 ^ ln(...))))) which gets us nowhere
 * so i think that the best definition is sum[k = 1..j] prod[m = 1..k] m ln m
 * note that this MUST be ln for correct growth patterns.
 * i think this earns the name of bogoSearch fairly well given that it sees n! and scoffs
 * now this was a fun thought exercise, but all of this only applies to the average case
 * so the AVERAGE time complexity is O(sum[k=1..n/2] prod[m=1..k](m log m)),
 * which i cannot break the factorial out of and have any meaningful way
 * Average: O(sum[k = 1..n/2] prod[m=1..k](m log m)) due to non-determinism
 * Oh, we didn't do the memory complexity. fortunately, you can only have k vectors of decreasing size,
 * so the memory complexity is average O(n^2) wait a minute how did we write quadratic memory complexity
 * I will, for the purposes of this, do an exploration of the worst DETERMINISTIC case
 * which should look like t(n) = sum[k = 1..n-1] t(k) due to recursive bogosort
 * let's start expanding
 * t(n) = t(n-1) + sum[k = 1..n-2] t(k)
 * = sum[k=1..n-2] t(k) + sum[k=1..n-2] t(k)
 * ...that worked out quickly. we got 2^n.
 * so, the worst DETERMINISTIC case is O(2^n), with non-deterministic obviously being unbounded.
 * this is an interesting result as it definitely grows slower than our previous one,
 * which managed to pass even factorials!
 * oh, and memory complexity too -- this one's also O(n^2) due to it needing all layers.
 * technically the other one is O(n^2/8) and this is O(n^2/2) but we don't notate constants.
 * I could figure it out exactly but i don't think that's in spec.
 * let's write the actual algorithm. DO NOT RUN THIS BECAUSE OF HOW SLOW IT IS.
**/
// break for autodoc
/**
 * The least efficient search algorithm you have ever seen.
 * @tparam T type of the array
 * @param vec vector we are searching
 * @param item the item you are searching for
 * @param cmp Pointer to comparison counter
 * @return whether the item exists or not
 * @remark upgrading gentoo feels like using this function
 * @remark Best case: O(1) hit / O(2^n) miss. WOW that is terrible good lord
 * @remark Average case: Uncertain hit/miss. Hit > O((n/2)!), miss > O(n!). How did we cross factorial complexity
 * @remark Worst case: O(infinity) hit/miss. (non-deterministic).
**/
template<typename T> bool bogoBogoSearch(const vector<T> vec, const T item, size_t *cmp) {
    const size_t len = vec.size();
    if (vec.empty()) return false;
    vector<size_t> indices;
    indices.resize(vec.len); // because this is the amount of memory we will need
    std::uniform_int_distribution<size_t> next(0, len-1);
    for (size_t i = 0; (len - i); i++) {
        size_t idx;
        do {
            idx = next(gen);
        } while (bogoBogoSearch(indices, item, cmp));
        if (incCmp && vec[idx] == item) return true;
        indices[i] = idx;
    }
    return false;
}


/**
 * Alright, that was awful. I don't think I ever want to intentionally code a bad algorithm again,
 * but I had an idea on Thursday.
 * We know the valid range of indices we need to check against, right?
 * Since it's a continuous discrete range, we can just use indexing.
 * For search misses, this shouldn't actually invoke additional runtime.
 * Pseudocode, then!
 * bool quickRandSearch(vec, item)
 * | len = vec.length
 * | indices = bool[len]
 * | loop len:
 * | | index
 * | | do // TIMES N log N
 * | | | index = random(0, len-1) // O(1)
 * | | while indices[index] // O(1)
 * | | if arr[len] == item
 * | | | return true
 * | | indices[len] = true
 * | return false
 *
 * Time complexity:
 * Best: O(1) hit / O(n) miss
 * Average: O(n^2 log n) hit / miss
 * Worst: O(infinity) due to runtime RNG
 */

// break for autodoc

/**
 * Even faster runtime than previous implementations.
 * @tparam T Type of the vector/item
 * @param vec Vector to be searched
 * @param item Item to be searched for
 * @param cmp Reference to comparison counter
 * @returns Whether the item was in the vector
 * @remark Best: O(N) hit / miss (array fill)
 * @remark Average: O(N^2 log N) hit / miss
 * @remark Worst: O(infinite) hit / miss
 */
template<typename T> bool constLookupRandSearch(const vector<T> vec, T item, size_t *cmp) {
    const size_t len = vec.size();
    bool checked[len];
    for (size_t i = 0; (len - i); i++) {
        checked[i] = false;
    }
    for (size_t i = 0; (len - i); i++) {
        size_t idx;
        std::uniform_int_distribution<size_t> next(0, len-1);
        do {
            idx = next(gen);
        } while (checked[idx]);
        if (incCmp && vec[idx] == item) {
            return true;
        }
        checked[idx] = true;
    }
    return false;
}

/*
 * But while implementing the above, I had an idea.
 * What if I had a way to not regenerate random numbers?
 * This would be like resolving a hash collision in a hash table.
 * I've never actually done this manually before, but given I'm not actually doing it,
 * I figured it shouldn't be too terrible.
 * Something like...
 * bool constLookupSearch(vec, item)
 * | for i = 0..len-1
 * | | index = rng(0, len-i-1)
 * | | while has checked vec[index] // constant lookup time
 * | | | index++
 * | | // 1/sqrt N/N collision handling
 * | | // vs 1/N log N/infinity handling with regeneration
 * | | if vec[index] is item
 * | | | return true
 * | return false
 *
 */

/**
 * The fastest search I can make using runtime RNG without going extremely overboard.
 * @tparam T Type of the vector
 * @param vec Vector to be searched
 * @param item Item to be searched for
 * @param cmp Comparison counter reference
 * @return Whether the item was contained
 * @remark Best: O(N)      hit / miss
 * @remark Average: O(N^2) hit / miss
 * @remark Worst: O(N^2)   hit / miss
 */
template<typename T> bool incGenMemRandSearch(const vector<T> vec, T item, size_t *cmp) {
    const size_t len = vec.size();
    size_t min_nCh = 0;
    // moved the declaration here to avoid cache miss by loading checked
    bool checked[len];
    for (size_t i = 0; (len - i); i++) {
        checked[i] = false;
    }
    for (size_t i = 0; (len - i); i++) {
        std::uniform_int_distribution<size_t> next(0, len-1-i);
        // only generates valid ranges
        size_t idx = next(gen) + min_nCh;
        // skips ones we've already checked

        // O(sqrt n) runtime
        // see below for what happens if you 'reroll' idx a certain amount of times
        for (size_t search = min_nCh; idx + 1 - search; search++) {
            if (checked[search]) {
                ++idx; // somehow this is supposed to be faster?
            }
        }
        // this is what i want to optimize
        while (checked[min_nCh]) {
            ++min_nCh;
        }
        // this prevents "slogging" by pushing towards the end of the checked indices
        // if the first indices are checked
        // because this only invokes O(1) runtime it's free
        // I think the savings are only proportional to lookup time
        // and don't change operation, because i only generate one time
        // generating more times would be slower, though
        if (incCmp && vec[idx] == item) {
            return true;
        }
        checked[idx] = true;
    }
    return false;
}

/*
 * The idea behind adding min_nCh is that it prevents "slogging",
 * where we would always find we had checked [0] after checking it.
 * Instead, we ignore it for the future, saving us a LOT of checks.
 * Despite making this optimization, we've still managed to limit ourselves to only one check/roll per loop.
 * This will cost us N checks, but will save on average... N checks.
 * Let this be a lesson in pointless optimization for everyone.
 * This leaves us at:
 * Best: O(1) hit / O(n) miss
 * Average: O(n^2) hit / miss
 * Worst: O(n^2) hit / miss
 *
 * Whoa, we don't have an infinite case anymore,
 * because we can't re-generate the same number infinite times!
 *
 * Though... what if we didn't try to generate random numbers as we went?
 * Getting a random ordering of a sequence should be possible in O(N) time.
 * Let's give that a shot, why don't we?
 */

// break for autodoc

/**
 * Searches randomly for an item.
 *
 * @tparam T type of the element
 * @param arr array to be searched
 * @param item item to be searched for
 * @param cmp comparison counter
 * @return Whether the item existed
 * @remark Best case: O(N) hit / O(n) miss
 * @remark Average case: O(n) / O(n) miss
 * @remark Worst case: O(n) / O(n) miss
**/
template<typename T> bool randomSearch(const vector<T> arr, const T item, size_t *cmp) {
    const size_t len = arr.size();
    size_t indices[len];
    for (size_t i = 0; (len - i); i++) {
        // cheating by using != -> ! for integers
        indices[i] = i;
    }
    shuffleArr(len, indices, cmp);
    for (int i = 0; (len - i); i++) {
        // cheating by using != -> ! for integers
        size_t idx = indices[i];
        if (incCmp && arr[idx] == item)  {
            return true;
        }
    }
    return false;
}
/*
 * One interesting thing about this function is that,
 * due to the time complexity of the 'search' internals being identical to linear search,
 * we only need to worry about if the overhead increases it.
 * Fortunately, the overhead is a straightforwards linear loop, so it's clearly O(n) complexity.
 * This is an inferior algorithm on average, but is less prone to adversarial design
 * (given that == comparison on T is a remarkably expensive operation, more so than filling an array)
 */

// break for autodoc

/**
 * That's pretty decent, but what if it's already sorted?
 * Well, in that case, the index we check can tell us where to look.
 * If ==, stop. If >, below, and if <, above.
 * Pseudocode time!
 *
 * bool notBinarySearch(len, arr, item)
 * | start = 0
 * | end = len - 1
 * | while end >= start
 * | | idx = randint(start, end)
 * | | if arr[idx] < item
 * | | | start = idx + 1
 * | | | continue
 * | | if arr[idx] > item
 * | | | end = idx - 1
 * | | | continue
 * | | return true
 * | return false
 *
 * wait a minute this is just binary search
 * I can't actually check time complexity on this one due to calculus being hard
 * But trust me it makes sense
 */
// break for autodoc
/**
 * Searches randomly, assuming the array is sorted. Will tend to fail on unsorted arrays.
 * This is just binary search in clown makeup.
 * @tparam T Type of the array that is being sorted. Only checked for type width reasons.
 * @param arr vector that is being searched
 * @param item item that is being searched for
 * @param cmp comparison counter
 * @return whether the item was found
 * @remark Best case: O(1) hit  miss
 * @remark Average case: O(log n) hit / miss
 * @remark Worst case: O(n) hit / miss
 */
template<typename T> bool quickSearch(const vector<T> arr, const T item, size_t *cmp) {
    const size_t len = arr.size();
    size_t start = 0;
    if (arr.empty()) return false;
    // saved a comparison here
    size_t end = len-1;
    while (incCmp && end >= start) {
        // using > and having a ternary or something similar in the return
        // doesn't actually save comparison operations so >= is correct here

        std::uniform_int_distribution<size_t> next(start, end);
        const size_t rnd = next(gen);
        if (incCmp && arr[rnd] < item) {
            start = rnd + 1;
            continue;
        }
        if (incCmp && arr[rnd] > item) {
            if (!rnd) return false;
            // checks for zero without using == so it doesn't count
            // it'll only be true when the following would underflow
            end = rnd - 1;
            continue;
        }
        return true;
        // i tried to rewrite this but wasn't actually able to save any operations
        // this is the minimum i can squeeze out while ensuring terminability
    }
    return false;
}
/*
 * I would love to actually analyze the time complexity of this one,
 * so I'll give it a shot.
 * Let's assume the item is equally likely to be at any position in the vector.
 * Then, our time complexity resembles the following:
 * qSearchCplx 1 = 1
 * qSearchCplx n = do {
 *  rnd = rng(0,n-1) # inclusive
 *  up = n - rnd
 *  dn = rnd
 *  return up / n * qSearchCplx up + dn / n * qSearchCplx dn
 * }
 * let's convert this into an interval sum average
 * qSrchCplx = 1/n sum[k=0..n] (k-n)/n * qSrchCplx (k-n) + k/n qSrchCplx k
 * = 1/n^2 sum[k=0..n] (k-n) qSrchCplx(k-n) + k qSrchCplx k
 * = 2/n^2 sum[k=0..n/2] k qSrchCplx k
 * = 2/n^2 sum[k=0..n] k qSrchCplx k
 * I suspect this is O(log^2 n) average time tbh but i don't know how to be sure about it
 * though i can argue that we have a k/n -> log n
 * and recur(n) -> n log n
 * which results in 2/n * log n * n log n = 2 log^2 n
 * which is good for hand waving but that's about it
 */
// break for autodoc
int main() {
    // do nothing
    auto *cmp = new size_t;
    *cmp = 0;


    delete cmp;
}