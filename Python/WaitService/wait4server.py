#!/usr/bin/env python3

import sys
import requests
from time import sleep
import datetime

def ping(url):
    """
    Checks if the given URL can be successfully retrieved.

    :param url: The URL to check.
    :return: True if a GET request to the given URL succeeds, False otherwise.
    """

    try:
        response = requests.get(url)
        return response.status_code == 200
    except requests.ConnectionError as e:
        return False

def getTimestamp():
    return datetime.datetime.now().isoformat(timespec='milliseconds')

def log(text):
    with open ("/tmp/waiter.log", "a") as logfile:
        logfile.write(f"[{getTimestamp()}] {text}\n")

def wait_url(url):
    """
    Waits until the given URL can be successfully retrieved.

    :param url:
    """

    while not ping(url):
        log(".")
        sleep(0.5)
        log("X")
        sleep(0.5)

    log("O")

def main(argv):
    dest_host = argv[1] if len(argv) > 1 else "http://localhost"

    log(f"Starting Apache Pinger for {dest_host} ...")
    wait_url(dest_host)
    log("Apache Pinger done.")

if __name__ == "__main__":
    main(sys.argv)
