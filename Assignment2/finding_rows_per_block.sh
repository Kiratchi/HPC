#!/bin/bash

# Initial range for x
x_min=9000
x_max=11000
tolerance=100  # How close we want to get to the solution
best_x=0
best_time=999999  # Initialize to a large value

# Function to compile and evaluate the time for a given x
evaluate_time() {
    local x=$1

    # Rebuild the code with the new value of x (assumes x is passed to the makefile or source code)
    make #X=$x

    # Run the hyperfine benchmark and extract the mean time
    hyperfine './distances -t4 -x' --export-json output.json > /dev/null
    local mean_time=$(jq '.results[0].mean' output.json)
    
    # Return the time
    echo $mean_time
}

# Binary search loop
while (( x_max - x_min > tolerance )); do
    # Midpoint
    x_mid=$(( (x_min + x_max) / 2 ))
    
    echo "Evaluating x = $x_mid"
    
    # Evaluate time at midpoint
    time_mid=$(evaluate_time $x_mid)

    echo "Time at x = $x_mid is $time_mid seconds"

    # Update the best x if this is the best time so far
    if (( $(echo "$time_mid < $best_time" | bc -l) )); then
        best_time=$time_mid
        best_x=$x_mid
    fi

    # Adjust the range based on the result
    if (( $(echo "$time_mid > $best_time" | bc -l) )); then
        x_min=$x_mid
    else
        x_max=$x_mid
    fi
done

# Final result
echo "Best x found: $best_x"
echo "Best execution time: $best_time seconds"
