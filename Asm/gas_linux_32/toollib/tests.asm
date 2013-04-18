.data

HelloStr:	.word	5
		.ascii 	"Hello"
WorldStr:	.word	6
		.ascii	"World!"
MoonStr:	.string	"Hello Moon!"

.bss

.lcomm	result, 100
.lcomm	buffer, 100

.text

.globl	_start

_start:
		movw	$0, (result)		# length of result string = 0

# result = result + "Hello"
		leal	result, %edi		# destination string
		leal	HelloStr, %esi		# source string
		call	StringAppend

# result = result + ' '
		movb	$' ', %al
		leal	result, %edi		# destination string
		call	StringAppendChar

# result = result + "World!"
		leal	result, %edi		# destination string
		lea	WorldStr, %esi		# source string
		call	StringAppend

# result = result + '\n'
		movb	$10, %al
		leal	result, %edi		# destination string
		call	StringAppendChar

# print result
		leal	result, %esi		# source string
		call	printString
 

		leal	result, %esi		# Source string
		movw	$6, %ax			# Index in source string
		leal	buffer, %edi		# destination string
		call	StringTail

# print result
		leal	buffer, %esi		# source string
		call	printString


#******************************************************************************

# Convert 1234567890 into a string
		movl	$1234567890, %eax
		leal	result, %edi
		call	toDec

# result = result + '\n'
		movb	$10, %al
		leal	result, %edi		# destination string
		call	StringAppendChar

		leal	result, %esi		# source string
		call	printString

# Convert -1234567890 into a string
		movl	$-1234567890, %eax
		leal	result, %edi
		call	toDecSigned

# result = result + '\n'
		movb	$10, %al
		leal	result, %edi		# destination string
		call	StringAppendChar

# print result
		leal	result, %esi		# source string
		call	printString

# Convert 1234567890 into a hex string
		movl	$1234567890, %eax
		leal	result, %edi
		call	toHex

# result = result + '\n'
		movb	$10, %al
		leal	result, %edi		# destination string
		call	StringAppendChar

# print result
		leal	result, %esi		# source string
		call	printString


# Get a String from a null terminated string
		leal	MoonStr, %esi
		leal	result, %edi
		call	StringFromCstr

		movb	$10, %al
		leal	result, %edi		# destination string
		call	StringAppendChar

		leal	result, %esi		# source string
		call	printString

		movl	$1, %eax			# System-call "sys_exit"
		movl 	$0, %ebx			# Exit-code
		int     $0x80
