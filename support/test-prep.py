import os
import argparse

common_header = """// RUN: %typeart-mpicc -O2 -g %s -o %ditests_test_dir/%basename_t.exe
// RUN: %must-mpiexec -n 2 %ditests_test_dir/%basename_t.exe
// RUN: cat %must-output-json | %filecheck %s
"""

header_correct = common_header + """

// CHECK-NOT: MUST_ERROR_TYPEMATCH_MISMATCH
"""

header_incorrect = common_header + """

// CHECK: MUST_ERROR_TYPEMATCH_MISMATCH
"""

def process_file(filepath, header):
    with open(filepath, "r", encoding="utf-8") as f:
        content = f.read()
    
    # Only add header if not already present
    if common_header.splitlines()[0] not in content:
        with open(filepath, "w", encoding="utf-8") as f:
            f.write(header + "\n" + content)

def main():
    parser = argparse.ArgumentParser(description="Add test headers to .c files in a directory.")
    parser.add_argument("target_dir", help="Target directory containing .c files.")
    args = parser.parse_args()

    target_dir = args.target_dir

    if not os.path.isdir(target_dir):
        print(f"Error: {target_dir} is not a valid directory.")
        return

    for filename in os.listdir(target_dir):
        if filename.endswith(".c"):
            filepath = os.path.join(target_dir, filename)
            if filename.startswith("Correct"):
                process_file(filepath, header_correct)
            else:
                process_file(filepath, header_incorrect)

if __name__ == "__main__":
    main()

