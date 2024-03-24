#!/bin/bash

# Check if the number of arguments is not equal to 2
if [ $# -ne 2 ]; then
	echo "Please provide 2 arguments."
	exit 1
fi

input=$1
output=$2

# Check if the input file exists
if [ ! -f $input ]; then
    echo "Input file '$input' does not exist."
    exit 1
fi

# Create output file
touch $output

echo ------------------ > $output

# Get unique cities from the data file
unique_cities=$(sed 's/,  */,/g' "$input" | awk -F',' 'NR>1 {print $3}' | sort | uniq)
echo "Unique cities in the given data file: " >> $output
echo "$unique_cities" >> $output

echo ------------------ >> $output

# Highest paid individuals by sorting and outputting the top 3
echo "Details of top 3 individuals with the highest salary: " >> $output
awk -F',' 'NR>1 {print $0}' $input | sort -t',' -k4,4nr | head -n 3 >> $output

echo ------------------ >> $output

# Calculating average salary of each city by calculating total salary and count of individuals
echo "Details of average salary of each city: " >> $output
sed 's/, /,/g' "$input" | awk -F',' 'NR > 1 {
    total[$3] += $4
    count[$3]++
}
END {
    for (city in total) {
        average = total[city] / count[city]
        format = (average % 1 == 0) ? "%.0f" : "%.1f"
        printf "City: %s, Salary: " format "\n", city, average
    }
}' >> $output

echo ------------------ >> $output

# Calculating overall average salary
overall_avg=$(awk -F', *' 'NR>1 {sum+=$4; count++} END {print sum/count}' "$input")

# Individuals with a salary above the overall average salary
echo "Details of individuals with a salary above the overall average salary: " >> $output
awk -F',' -v overall_avg="$overall_avg" 'NR>1 && $4 > overall_avg {
    printf "%s %s %s %s\n", $1, $2, $3, $4
}' "$input" >> $output

echo ------------------	>> $output

echo "Output file '$output' created."
echo "It contains the following information:"
echo "1. Unique cities in the given data file."
echo "2. Details of top 3 individuals with the highest salary."
echo "3. Details of average salary of each city."
echo "4. Details of individuals with a salary above the overall average salary."

exit 0