
make
hyperfine "./distances -t5" --export-json output.json
mean_time=$(jq ".results[0].mean" output.json)
echo "Mean execution time: $mean_time seconds"