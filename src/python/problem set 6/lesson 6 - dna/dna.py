import csv
import sys


def main():
    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py FILE.csv FILE.txt")
        exit(1)

    # Read database file into a variable
    # Read DNA sequence file into a variable
    rows = []
    # open csv and txt files to read into dicts
    with open(sys.argv[1]) as csvFile, open(sys.argv[2]) as txtFile:
        reader = csv.DictReader(csvFile)
        for row in reader:
            rows.append(row)
        sequence = txtFile.read()
    # intialize a variable length list of keys for the STRs
    keys = []
    for i in range(len(reader.fieldnames)):
        if i < 1:
            continue
        keys.append(str(reader.fieldnames[i]))

    # Find longest match of each STR in DNA sequence
    longestSTRs = {}
    for i in range(len(keys)):
        longestSTRs.update({keys[i]: longest_match(sequence, keys[i])})

    # Check database for matching profiles
    # loop i iterates through rows, loop j iterates through STRs to compare lengths
    # a name is printed if the number of matches found is equal to the number of STR keys
    for i in range(len(rows)):
        matchesFound = 0
        for j in range(len(keys)):
            if int(rows[i][keys[j]]) == int(longestSTRs[keys[j]]):
                matchesFound += 1
            if matchesFound == len(keys):
                print(rows[i]["name"])
                exit(0)

    print("No Match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
