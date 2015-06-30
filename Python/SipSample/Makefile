all: lib sipwrapper run

lib:
	mkdir build
	( cd build; cmake .. ; make )

sipwrapper:
	( cd sip ; python3 configure.py ; make )

run:
	( export PYTHONPATH=sip; export LD_LIBRARY_PATH=build/cpp/lib; python/sipSample.py )

clean:
	rm -rf build
	( cd sip ; make clean ; rm helloworld.sbf sipAPIhelloworld.h siphelloworldHelloWorld.cpp siphelloworldcmodule.cpp Makefile )
