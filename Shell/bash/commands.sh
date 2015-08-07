# grep pattern and print pattern space only
grep -o PATTERN file

# print capture group with sed
sed -e 's/PATTERN\(GRPPATTERN\)/\1/'

# Inline-replace ("-i") a date in log files (WARNING! Changes the given file!)
sed -i 's/\([0-9]*-[0-9]*-[0-9]*\) \([0-9]*:[0-9]*:[0-9]*\.[0-9]*\) \(Thread-[0-9]*\)/XXXX-XX-XX XX:XX:XX Thread-XXX/g' file.log

# The capture groups allow access to the date, time and thread part - e.g. to switch time and date:
sed 's/\([0-9]*-[0-9]*-[0-9]*\) \([0-9]*:[0-9]*:[0-9]*\.[0-9]*\) \(Thread-[0-9]*\)/\2 \1 \3/g' file.log
