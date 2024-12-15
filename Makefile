build:
	g++ -std=c++11 -o project2.out src/*.cpp 
run:
	./project2.out	
clean:
	rm output/*.tga
	rm project2.out
compare:
	fc.exe output\part1.tga examples\EXAMPLE_part1.tga
	fc.exe output\part2.tga examples\EXAMPLE_part2.tga
	fc.exe output\part3.tga examples\EXAMPLE_part3.tga
	fc.exe output\part4.tga examples\EXAMPLE_part4.tga
	fc.exe output\part5.tga examples\EXAMPLE_part5.tga
	fc.exe output\part6.tga examples\EXAMPLE_part6.tga
	fc.exe output\part7.tga examples\EXAMPLE_part7.tga
	fc.exe output\part8_b.tga examples\EXAMPLE_part8_b.tga
	fc.exe output\part8_g.tga examples\EXAMPLE_part8_g.tga
	fc.exe output\part8_r.tga examples\EXAMPLE_part8_r.tga
	fc.exe output\part9.tga examples\EXAMPLE_part9.tga
	fc.exe output\part10.tga examples\EXAMPLE_part10.tga