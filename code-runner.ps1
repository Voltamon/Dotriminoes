gcc -g .\dotriminoes.c .\blocs.c -o .\dotris.exe -O1 -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
.\dotris.exe
del .\dotris.exe
Clear-Host