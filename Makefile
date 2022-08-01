CC = cc
CFLAGS = -Wall -Wextra -std=c99 \
				 -Iinclude \
				 -Llib \
				 -rpath lib \
				 -lglfw \
				 -lMoltenVK \
				 -lvulkan \

export VK_ICD_FILENAMES="share/vulkan/icd.d/MoltenVK_icd.json"
export VK_LAYER_PATH="share/vulkan/explicit_layer.d"

all: lab

lab: src/main.c
	mkdir -p bin
	$(CC) src/main.c $(CFLAGS) -o bin/lab

debug: src/main.c
	mkdir -p bin
	$(CC) -g  src/main.c $(CFLAGS) -o bin/lab

clean:
	rm -f tmp/* bin/*
