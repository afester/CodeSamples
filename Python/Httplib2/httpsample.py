#!/usr/bin/python

import sys
import httplib2



'''
Created on Jan 27, 2015

@author: andreas
'''

def main():
    print("Using Python {0}.{1}.{2} ({3})".format(
                              sys.version_info[0],
                              sys.version_info[1],
                              sys.version_info[2],
                              sys.version_info[3]))
    h = httplib2.Http()
    resp, content = h.request("http://www.google.com", "GET")
    h._request(conn, host, absolute_uri, request_uri, method, body, headers, redirections, cachekey)

    print("\nHeaders:")
    print(resp)

    print("\nContent:")
    print(content)

if __name__ == '__main__':
    main()