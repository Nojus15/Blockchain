main : compile
	g++ -o app *.o -O3
	del *.o
compile :
	g++ -c main.cpp Application/Application.cpp Generator/Generator.cpp File/File.cpp -O3
clean :
	del *.exe *.o

# mingw32-make