all: hello helloMulticore

hello:
	ghc -o hello hello.hs

helloMulticore:
	ghc -O2 --make helloMulticore.hs -threaded -rtsopts
