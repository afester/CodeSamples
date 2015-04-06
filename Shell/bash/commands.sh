# grep pattern and print pattern space only
grep -o PATTERN file

# print capture group with sed
sed -e 's/PATTERN\(GRPPATTERN\)/\1/'
