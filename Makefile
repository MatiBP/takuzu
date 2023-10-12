SRC_DIR = src

.PHONY: all clean help

all:
	@$(MAKE) -C $(SRC_DIR)

clean:
	@$(MAKE) -C $(SRC_DIR) clean

help:
	@echo "Usage of Makefile:"
	@echo "  make         : Build the takuzu binary"
	@echo "  make clean   : Remove temporary files and the binary"
	@echo "  make help    : Display this help message"