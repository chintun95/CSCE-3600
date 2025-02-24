if [ "$1" == "yesterday" ]; then
    date -d "yesterday" '+%A'
elif [ "$1" == "today" ]; then
    date '+%A'
elif [ "$1" == "tomorrow" ]; then
    date -d "tomorrow" '+%A'
else
    echo "Invalid option. Use \"yesterday\", \"today\", or \"tomorrow\""
fi
