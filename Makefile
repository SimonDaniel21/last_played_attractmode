playlistCreator: main.cpp
	g++ -o playlistCreator main.cpp
	
test:
	./playlistCreator /home/simon/Documents/last-played.txt 5 Titel Name Emulator
