import re
import sys
import json
from statistics import median
from collections import defaultdict

TARGET_LABELS = [
    "Total heap",
    "Total stack",
    "Total global",
    "Addresses checked",
    "Distinct Addresses checked",
    "Addresses missed",
]

PATTERN = re.compile(r"^\s*(.*?)\s*:\s*([\d,]+)")

def parse_log_data(log_content):
    data = defaultdict(list)
    blocks = re.split(r"-{54}\nAlloc Stats from softcounters", log_content)

    for block in filter(str.strip, blocks):
        for line in block.splitlines():
            match = PATTERN.match(line)
            if match:
                label, value_str = match.groups()
                if label in TARGET_LABELS:
                    try:
                        value = int(value_str.replace(',', ''))
                        data[label].append(value)
                    except ValueError:
                        print(f"Warning: Couldn't parse value '{value_str}' for label '{label}'")
    results = {}
    # print("\nMedian values:")
    for label in TARGET_LABELS:
        values = data.get(label)
        if values:
            median_value = median(values)
            results[label] = median_value
            # print(f"Median {label}: {median_value}")
        else:
            results[label] = None
            # print(f"Median {label}: No data found")
    return results

def main():
    if len(sys.argv) < 2:
        print(f"Usage: python {sys.argv[0]} <input_log_file> [output_json_file]")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else None

    try:
        with open(input_path, encoding='utf-8') as f:
            log_content = f.read()
        results = parse_log_data(log_content)

        json_output = json.dumps(results, indent=2)
        if output_path:
            with open(output_path, 'w', encoding='utf-8') as out_f:
                out_f.write(json_output)
            # print(f"\nJSON output written to {output_path}")
        else:
            # print("\nJSON output:\n")
            print(json_output)

    except FileNotFoundError:
        print(f"Error: File not found at {input_path}")
        sys.exit(1)
    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
