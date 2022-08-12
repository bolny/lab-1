CC = cc
CFLAGS = -Wall -Wextra -std=c99 \
				 -I/opt/homebrew/include \
				 -L/opt/homebrew/lib \
				 -rpath /opt/homebrew/lib \
				 -lglfw \
				 -lMoltenVK \
				 -lvulkan \

export VK_ICD_FILENAMES="/opt/homebrew/share/vulkan/icd.d/MoltenVK_icd.json"
export VK_LAYER_PATH="/opt/VulkanSDK/1.3.216.0/macOS/share/vulkan/explicit_layer.d"

all: lab

lab: src/main.c
	mkdir -p bin
	$(CC) src/main.c $(CFLAGS) -o bin/lab

debug: src/main.c
	mkdir -p bin
	$(CC) -g  src/main.c $(CFLAGS) -o bin/lab

clean:
	rm -rf tmp/* bin/*
