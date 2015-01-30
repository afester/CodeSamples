#!/usr/bin/python

import os
import sys
import httplib2
import socks
import urlparse
from launchpadlib.launchpad import Launchpad

def no_credential():
    print "Can't proceed without Launchpad credential."
    sys.exit()

def main():
    cachedir = ".lpcache"

    # get http proxy from environment
    # proxyInfo = None
    # httpProxy = os.environ.get('http_proxy')
    # if httpProxy is not None:
    #    o = urlparse.urlparse(httpProxy)
    #    proxyInfo = httplib2.ProxyInfo(socks.PROXY_TYPE_HTTP, o.hostname, o.port)

    # launchpad = Launchpad.login_anonymously('just testing', 'production', cachedir, proxy_info = proxyInfo)
    launchpad = Launchpad.login_anonymously('just testing', 'production', cachedir)
    bug_one = launchpad.bugs[1]
    print bug_one.title

if __name__ == '__main__':
    main()
