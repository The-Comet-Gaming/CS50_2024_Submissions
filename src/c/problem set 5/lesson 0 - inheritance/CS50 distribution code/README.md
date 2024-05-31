# Inheritance
## Understanding/Distribution Code
Take a look at the distribution code in `inheritance.c`.

Notice the definition of a type called `person`. Each person has an array of two `parents`, each of which is a pointer to another `person` struct. Each person also has an array of two `alleles`, each of which is a `char` (either `'A'`, `'B'`, or `'O'`).
```c
// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;
```

Now, take a look at the `main` function. The function begins by “seeding” (i.e., providing some initial input to) a random number generator, which we’ll use later to generate random alleles.
```c
// Seed random number generator
srand(time(0));
```

The `main` function then calls the `create_family` function to simulate the creation of `person` structs for a family of 3 generations (i.e. a person, their parents, and their grandparents).
```c
// Create a new family with three generations
person *p = create_family(GENERATIONS);
```

We then call `print_family` to print out each of those family members and their blood types.
```c
// Print family tree of blood types
print_family(p, 0);
```

Finally, the function calls `free_family` to `free` any memory that was previously allocated with `malloc`.
```c
// Free memory
free_family(p);
```

The `create_family` and `free_family` functions are left to you to write!
