launch:	compil exe dot

compil:
	g++ dif_main.cpp dif.cpp -o dif

exe:
	./dif

dot:
	dot -Tpng graph.txt -o pict.png