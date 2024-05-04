# Volume
## Understand/Distribution Code
Notice first that `volume.c` is already set up to take three command-line arguments, `input`, `output`, and `factor`.

- `main` takes both an `int`, `argc`, and an array of `char *s` (strings!), `argv`.
- If `argc`, the number of arguments at the command-line including the program itself, is not equal to 4, the program will print its proper usage and exit with status code 1.
```c
int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // ...
}
```

Next, `volume.c` uses `fopen` to open the two files provided as command-line arguments.
- It’s best practice to check if the result of calling `fopen` is `NULL`. If it is, the file wasn’t found or wasn’t able to be opened.
```c
// Open files and determine scaling factor
FILE *input = fopen(argv[1], "r");
if (input == NULL)
{
    printf("Could not open file.\n");
    return 1;
}

FILE *output = fopen(argv[2], "w");
if (output == NULL)
{
    printf("Could not open file.\n");
    return 1;
}
```

Later, these files are closed with `fclose`. Whenever you call `fopen`, you should later call `fclose`!
```c
// Close files
fclose(input);
fclose(output);
```

Before closing the files, though, notice that we have a few TODOs.
```
// TODO: Copy header from input file to output file

// TODO: Read samples from input file and write updated data to output file
```

Odds are you’ll need to know the factor by which to scale the volume, hence why `volume.c` already converts the third command-line argument to a `float` for you!
```c
float factor = atof(argv[3]);
```
