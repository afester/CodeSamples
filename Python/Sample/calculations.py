#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math

def physical():
    c = 299792458   # m/s
    µ0 = 4 * math.pi * 1e-7
    ε0 = 1 / (µ0 * c**2)

    print("c = %s" % str(c)) 
    print("µ0 = %s" % str(µ0)) 
    print("ε0 = %s" % str(ε0)) 
