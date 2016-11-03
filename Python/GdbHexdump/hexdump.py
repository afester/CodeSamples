#!/usr/bin/python

import gdb, sys

class HexDump(gdb.Command):
    def __init__(self):
        super (HexDump, self).__init__ ('hex-dump', gdb.COMMAND_DATA)
        pythonVersion = "%s.%s.%s (%s)" % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3])
        print('Using python %s' % pythonVersion)

    def groups_of(iterable, size, first=0):
        if first == 0:
            first=size
        chunk, iterable = iterable[:first], iterable[first:]
        while chunk:
            yield chunk
            chunk, iterable = iterable[:size], iterable[size:]

    def invoke(self, arg, from_tty):
        # print('Invoke %s %s' %(arg, from_tty))
        argv = gdb.string_to_argv(arg)
        if len(argv) != 2:
            raise gdb.GdbError('hex-dump takes exactly 2 arguments.')
        addr = gdb.parse_and_eval(argv[0]).cast(
            gdb.lookup_type('void').pointer())
        try:
            bytes = int(gdb.parse_and_eval(argv[1]))
        except ValueError:
            raise gdb.GdbError('Byte count numst be an integer value.')

        inferior = gdb.selected_inferior()

        align = False # gdb.parameter('hex-dump-align')
        width = 16    # gdb.parameter('hex-dump-width')
        if width == 0:
            width = 16

        mem = inferior.read_memory(addr, bytes)
        pr_addr = int(str(addr), 16)
        pr_offset = width

        if align:
            pr_offset = width - (pr_addr % width)
            pr_addr -= pr_addr % width

        for group in groups_of(mem, width, pr_offset):
            print '0x%x: ' % (pr_addr,) + '   '*(width - pr_offset),

            x = 0
            if pr_offset == width:
                 x = width - len(group)
            print ' '.join(['%02X' % (ord(g),) for g in group]) + \
                '   ' * x + ' ',
#                '   ' * (width - len(group) if pr_offset == width else 0) + ' ',

            for g in group:
                if isgraph(g) or g == ' ':
                   l.apppend(g)
                else:
                   l.append('.')
            print ' '*(width - pr_offset) +  ''.join(l)

#                [g if isgraph(g) or g == ' ' else '.' for g in group])
            pr_addr += width
            pr_offset = width

HexDump()
