CC=gcc
FLAGS=-Wall -Werror -O2 -Iinclude
CFILES=$(wildcard src/*.c)

app.exe: $(addprefix out/,$(addsuffix .o,$(notdir $(basename $(CFILES)))))
	$(CC) $(FLAGS) $^ -o app.exe

run: app.exe
	./app.exe test

out/%.o: */%.c
	$(CC) $(FLAGS) -c -MD $< -o out/$(addsuffix .o,$(basename $(notdir $<)))

clean:
	rm *.out *.exe out/*.* out/.


include $(wildcard out/*.d)
