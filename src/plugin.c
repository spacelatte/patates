/*bin/echo
FILE="$0.exe"
trap "rm -f '$FILE'" EXIT
cc -I../inc -o "$FILE" -c "$0"
exit
*/