build:
	g++ -o ./out/program -I ./include $(shell find ./src -name "*.cpp" -o -name "*.cc" -o -name "*.cxx" -o -name "*.c++" -o -name "*.C") -L ./lib -lsoci_sqlite3 -lsoci_empty -lfoo -lsoci_tests_common -lsoci_mysql -lsoci_postgresql -lsoci_core -lsqlite3
run:
	./out/program
