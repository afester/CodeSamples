
from setuptools import setup, find_packages

with open('requirements.txt') as f:
    required = f.read().splitlines()

setup(
    name = "MynPad",
    version = "0.1",
    packages = find_packages(exclude=["tests"]) + ["."],

    package_data = {
        'data': ['styles.css', 'webpage.css', 'logging.ini'],
    },

    install_requires = required
#    install_requires = ['cssutils', 'matplotlib', 'PyQt6', 'PyQt6-WebEngine']
)
