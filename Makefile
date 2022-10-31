# main : compile
# 	g++ -o app *.o -O3
# 	del *.o
# compile :
# 	g++ -c main.cpp Application/Application.cpp Generator/Generator.cpp File/File.cpp Hasher/Hasher.cpp Transaction/Transaction.cpp User/User.cpp Client/Client.cpp -fopenmp MerkleTree/MerkleTree.cpp Block/Block.cpp Timer/Timer.cpp -O3
# clean :
# 	del *.exe *.o

main :
	g++ -o app main.cpp Application/Application.cpp Generator/Generator.cpp File/File.cpp Hasher/Hasher.cpp Transaction/Transaction.cpp User/User.cpp Client/Client.cpp -fopenmp MerkleTree/MerkleTree.cpp Block/Block.cpp Timer/Timer.cpp


# mingw32-make