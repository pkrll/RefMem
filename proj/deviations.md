The project met all requirements, except possibly one.

When freeing in conjunction with allocation, the specification states that we should respect the cascade limit, *unless the amount of memory free'd is less than the amount of bytes requested*. In that case, we should keep going until we have free'd the same amount of bytes as requested.

Our implementation may however exceed that amount, as the function will iterate over the queue of objects and free them in sequence, until it has reached the requested size, instead of finding objects in the queue that together amounts to the exact same size as requested.

For example, if the user has requested 100 bytes and the first 5 objects amounts to a total of 120 bytes, our system will free the 5 objects and thereby exceeding the 100 bytes requested.
