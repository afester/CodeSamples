.data

digits:		.ascii	"0123456789abcdef"

.bss

.text

.globl	StringAppend
.globl	StringAppendChar
.globl	StringTail
.globl	StringFromCstr
.globl	printString
.globl	toHex
.globl	toDec
.globl	toDecSigned

# Prints a string to stdout
#
# @param esi The source address of the string.
printString:
		movl	$4, %eax	# System-call "sys_write"
		movl	$1, %ebx	# file-descriptor "stdout"
		movl	%esi, %ecx
		addl	$2, %ecx	# skip length information
		xorl	%edx, %edx
		movw	(%esi), %dx	# length of data
		int	$0x80

		ret


# Converts a null terminated string into a length defined string
#
# @param esi The source string (null terminated)
# @param edi The destination string
StringFromCstr:
		leal	2(%edi), %edi	# data portion
		leal	(%edi), %ebx	# save address
1:
		lodsb			# [esi++] => al
		cmpb	$0, %al
		je	2f

		stosb			# al => [edi++]
		jmp	1b
2:
		movl	%edi, %eax
		subl	%ebx, %eax	# number of bytes copied
		movw	%ax, -2(%ebx)

		ret


# Returns the remaining string, starting at a specific index
#
# @param esi The source string
# @param ax  The index in the source string where to start
# @param edi The destination string
StringTail:

# Number of bytes to move
		xorl	%ecx, %ecx
		movw	(%esi), %cx	# length of source string
		subw	%ax, %cx	# number of bytes to move
		jbe	1f		# <= 0, then no bytes to move

# Source address
		leal	2(%esi), %esi	# start of source string data
		xorl	%ebx, %ebx
		movw	%ax, %bx
		addl	%ebx, %esi	# start index

# Adjust length of destination string
		movl	%ecx, (%edi)	# new result string length

# Destination address
		leal	2(%edi), %edi	# start of destination string

# Copy the bytes
		rep movsb

1:
		ret


# Appends one string to another
#
# @param esi The source string
# @param edi The destination string
StringAppend:

# Number of bytes to move
		xorl	%ecx, %ecx
		movw	(%esi), %cx	# length of source string

# Source address
		leal	2(%esi), %esi	# start of source string data

# Adjust length of destination string
		xorl	%ebx, %ebx
		movw	(%edi), %bx	# length of destination string
		addl	%ecx, (%edi)	# new result string length

# Destination address
		leal	2(%edi, %ebx), %edi	# end of destination string

# Copy the bytes
		rep movsb

		ret

# Appends a character to a string
#
# @param al The character to append
# @param edi The destination string
StringAppendChar:
		xorl	%ebx, %ebx
		movw	(%edi), %bx	# length of destination string

		incw	(%edi)		# new length of result string

		leal	2(%edi, %ebx), %edi	# end of destination string
		movb	%al, (%edi)	# append character to string

		ret


# Reverses a given String
#
# @param esi The string to reverse
StringReverse:
		xorl	%ecx, %ecx
		movw	(%esi),	%cx		# get string length
		leal	1(%esi, %ecx), %edi	# points to last character in string
		leal	2(%esi), %esi		# points to first character in string

1:
		cmpl	%esi, %edi
		jbe	2f

		# exchange the two characters
		mov	(%esi), %al
		mov	(%edi), %ah
		mov	%al, (%edi)
		mov	%ah, (%esi)

		dec	%edi
		inc	%esi
		jmp	1b
2:
		ret


# Format a hexadezimal notation of a 32 bit number
#
# @param eax	The 32 bit number to print
# @param edi    The destination buffer to retrieve the result string.
#               Needs to have a size of at least 10 bytes (2x size + 8 characters).
toHex:
		movw	$8, (%edi)		# result string has fixed length
		addl	$2, %edi
		movl	$8, %ecx

1:
		roll	$4, %eax
		movl	%eax, %ebx
		andl	$0x0000000f, %ebx
		movb	digits(,%ebx), %bl	# NOTE: syntactic exception!
		movb	%bl, (%edi)

		incl	%edi
		loop	1b

		ret


# Converts a 32 bit number into decimal
#
# @param eax	The 32 bit number to convert
# @param edi    The destination buffer which receives the string
toDec:
		leal	2(%edi), %edi		# move to data portion of the string
		movl	%edi, %esi		# remember edi - needed later for the string length

1:
# EAX / 10 => EAX, Remainder in EDX
		xorl	%edx, %edx
		movl	$10, %ebx
		divl	%ebx

# Store next character in buffer
		movb	digits(,%edx), %bl	# get character
		mov	%bl, (%edi)		# store character in buffer

		incl	%edi			# next location in buffer -
						# string is reversed first!
		cmpl	$0, %eax
		jne	1b

# Now, esi=start of string, edi=behind last string character

		movl	%edi, %eax
		subl	%esi, %eax		# calculate string length
		movw	%ax, -2(%esi)		# store string length

# Reverse the string
		decl	%edi
2:
		# exchange the two characters
		movb	(%esi), %al
		movb	(%edi), %ah
		movb	%al, (%edi)
		movb	%ah, (%esi)

		decl	%edi
		incl	%esi

		cmpl	%esi, %edi
		ja	2b

		ret


# Convert a signed 32 bit number to a string
#
# @param eax	The signed 32 bit number to print
# @param edi    The destination buffer for the result string
toDecSigned:
		leal	2(%edi), %edi		# move to data portion of the string
		movl	%edi, %esi		# remember edi - needed later for the string length

# check sign flag, store "-" if necessary and adjust value
		testl	$0x80000000, %eax
		jz	1f
		movb	$'-', (%edi)
		incl	%edi

		negl	%eax

1:
# EAX / 10 => EAX, Remainder in EDX
		xorl	%edx, %edx
		movl	$10, %ebx
		divl	%ebx

# Store next character in buffer
		movb	digits(,%edx), %bl	# get character
		movb	%bl, (%edi)		# store character in buffer
		incl	%edi

		cmpl	$0, %eax
		jne	1b

# Now, esi=start of string (first character OR sign), edi=behind last string character
		movl	%edi, %eax
		subl	%esi, %eax		# calculate string length
		movw	%ax, -2(%esi)		# store string length

# Reverse the string
		decl	%edi

		cmpb	$'-', (%esi)
		jne	2f
		incl	%esi

2:
		# exchange two characters
		movb	(%esi), %al
		movb	(%edi), %ah
		movb	%al, (%edi)
		movb	%ah, (%esi)

		decl	%edi
		incl	%esi

		cmpl	%esi, %edi
		ja	2b

		ret
