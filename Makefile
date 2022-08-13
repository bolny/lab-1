CC = cc
CFLAGS = -Wall -Wextra -pedantic -std=c99 \
				 -I/opt/homebrew/include \
				 -L/opt/homebrew/lib \
				 -rpath /opt/homebrew/lib \
				 -lglfw \
				 -lMoltenVK \
				 -lvulkan \

VIM_CFLAGS = ${CFLAGS} \
						 -fno-caret-diagnostics \
						 -fno-color-diagnostics

export VK_ICD_FILENAMES="/opt/homebrew/share/vulkan/icd.d/MoltenVK_icd.json"
export VK_LAYER_PATH="/opt/VulkanSDK/1.3.216.0/macOS/share/vulkan/explicit_layer.d"

default: vim

release: src/main.c
	$(CC) src/main.c $(CFLAGS) -o bin/lab

debug: src/main.c
	$(CC) src/main.c -g $(CFLAGS) -o bin/lab

vim: src/main.c
	$(CC) src/main.c -g $(VIM_CFLAGS) -o bin/lab

clean:
	rm -rf tmp/* bin/*
