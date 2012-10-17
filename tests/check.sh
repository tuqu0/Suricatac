#! /bin/sh

# colors
CK='\033[30m'
RED='\033[31m'
GREEN='\033[32m'
YELLOW='\033[33m'
BLUE='\033[34m'
PURPLE='\033[35m'
CYAN='\033[36m'
WHITE='\033[37m'

# config
CONFIG="unit_tests.cfg"
EXEC="./Suricatac"
CMD=""
FILE_OUTPUT=""
COLS="80"

# check if the output is equal between binary and file test
printf "$YELLOW %50s\n" "---   Checking project   ---"

while read line; do

	FILE=$(echo $line | cut -d\; -f1)
	ARGS=$(echo $line | cut -d\; -f2)
	FILE_OUTPUT=`cat $FILE`
	CMD=`$EXEC $ARGS 2>&1`
	COL=$(($COLS - ${#FILE}))
	
	printf "$YELLOW $FILE  : "
	if [ "$CMD" == "$FILE_OUTPUT" ]; then
		printf "$GREEN %$COL""s\n" "OK"	
	else
		printf "$RED %$COL""s\n" "KO"
	fi
done < $CONFIG
printf $GREEN
printf "\n"

exit 0
