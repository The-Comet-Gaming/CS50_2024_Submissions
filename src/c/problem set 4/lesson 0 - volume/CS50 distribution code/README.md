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

### Copy WAV header from input file to output file
Your first TODO is to copy the WAV file header from `input` and write it to `output`. First, though, you’ll need to learn about a few special data types.

So far, we’ve seen a number of different types in C, including `int`, `bool`, `char`, `double`, `float`, and `long`. However, inside a header file called `stdint.h` are the declarations of a number of *other* types that allow us to very precisely define the size (in bits) and sign (signed or unsigned) of an integer. Two types in particular will be useful to us when working with WAV files:

- `uint8_t` is a type that stores an 8-bit (hence 8!) unsigned (i.e., not negative) integer (hence `uint`!). We can treat each byte of a WAV file’s header as a `uint8_t` value. 
- `int16_t` is a type that stores a 16-bit signed (i.e., positive or negative) integer. We can treat each sample of audio in a WAV file as an `int16_t` value. 

You’ll likely want to create an array of bytes to store the data from the WAV file header that you’ll read from the input file. Using the `uint8_t` type to represent a byte, you can create an array of `n` bytes for your header with syntax like
```
uint8_t header[n];
```

replacing `n` with the number of bytes. You can then use `header` as an argument to `fread` or `fwrite` to read into or write from the header.

Recall that a WAV file’s header is always exactly 44 bytes long. Note that `volume.c` already defines a variable for you called `HEADER_SIZE`, equal to the number of bytes in the header.

The below is a pretty big hint, but here’s how you could accomplish this TODO!
```
// Copy header from input file to output file
uint8_t header[HEADER_SIZE];
fread(header, HEADER_SIZE, 1, input);
fwrite(header, HEADER_SIZE, 1, output);
```

### Write updated data to output file
Your next TODO is to read samples from `input`, update those samples, and write the updated samples to `output`. When reading files, it’s common to create a “buffer” in which to temporarily store data. There, you can modify the data and—once it’s ready—write the buffer’s data to a new file.

Recall that we can use the `int16_t` type to represent a sample of a WAV file. To store an audio sample, then, you can create a buffer variable with syntax like:
```c
// Create a buffer for a single sample
int16_t buffer;
```

With a buffer for samples in place, you can now read data into it, one sample at a time. Try using `fread` for this task! You can use `&buffer`, the address of `buffer`, as an argument to `fread` or `fwrite` to read into or write from the buffer. (Recall that the `&` operator is used to get the address of the variable.)
```c
// Create a buffer for a single sample
int16_t buffer;

// Read single sample into buffer
fread(&buffer, sizeof(int16_t), 1, input)
```

Now, to increase (or decrease) the volume of a sample, you need only multiply it by some factor.
```c
// Create a buffer for a single sample
int16_t buffer;

// Read single sample into buffer
fread(&buffer, sizeof(int16_t), 1, input)

// Update volume of sample
buffer *= factor;
```

And finally, you can write that updated sample to `output`:
```c
// Create a buffer for a single sample
int16_t buffer;

// Read single sample from input into buffer
fread(&buffer, sizeof(int16_t), 1, input)

// Update volume of sample
buffer *= factor;

// Write updated sample to new file
fwrite(&buffer, sizeof(int16_t), 1, output);
```

There’s just one problem: you’ll need to continue reading a sample into your buffer, updating its volume, and writing the updated sample to the output file while there are still samples left to read.

- Thankfully, per its documentation, `fread` will return the number of items of data successfully read. You may find this useful to check for when you’ve reached the end of the file!
- Keep in mind there’s no reason you can’t call `fread` inside of a `while` loop’s conditional. You could, for example, make a call to `fread` like the following:
```c
while (fread(...))
{

}
```

It’s quite the hint, but see the below for an efficient way to solve this problem:
```c
// Create a buffer for a single sample
int16_t buffer;

// Read single sample from input into buffer while there are samples left to read
while (fread(&buffer, sizeof(int16_t), 1, input) != 0)
{
    // Update volume of sample
    buffer *= factor;

    // Write updated sample to new file
    fwrite(&buffer, sizeof(int16_t), 1, output);
}
```

Because the version of C you’re using treats non-zero values as `true` and zero values as `false`, you could simplify the above syntax to the following:
```c
// Create a buffer for a single sample
int16_t buffer;

// Read single sample from input into buffer while there are samples left to read
while (fread(&buffer, sizeof(int16_t), 1, input))
{
    // Update volume of sample
    buffer *= factor;

    // Write updated sample to new file
    fwrite(&buffer, sizeof(int16_t), 1, output);
}
```
