
from setuptools import setup, find_packages

setup(
    name = "MynPad",
    version = "0.1",
    packages = find_packages(exclude=["tests"]) + ["."],

    package_data = {
        'data': ['styles.css', 'webpage.css', 'logging.ini'],
    }
)
