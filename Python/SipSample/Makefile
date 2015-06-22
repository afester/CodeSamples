all: lib sipwrapper

lib:
	mkdir build
	( cd build; cmake .. ; make )

sipwrapper:
	( cd sip ; python configure.py ; make )

clean:
	rm -rf build
