
in_name = input("Enter input file name: ")

try:
    in_file = open(in_name, "r")
    out_file = open("output.txt", "w")

    out_file.write(in_file.read()[::-1])

    print("Completed. Output stored in output.txt")

    in_file.close()
    out_file.close()

except FileNotFoundError:
    print("Error: Input file does not exist.")
