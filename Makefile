build:
	g++ -g -o calc.exe main.cpp tokenizer.cpp parser.cpp

run: build
	./calc