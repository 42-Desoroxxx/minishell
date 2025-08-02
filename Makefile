##############################################################################################
#                                                                                            #
#   /$$$$$$                                      /$$      /$$           /$$                  #
#  /$$__  $$                                    | $$$    /$$$          | $$                  #
# | $$  \__/  /$$$$$$  /$$$$$$$$ /$$   /$$      | $$$$  /$$$$  /$$$$$$ | $$   /$$  /$$$$$$   #
# | $$       /$$__  $$|____ /$$/| $$  | $$      | $$ $$/$$ $$ |____  $$| $$  /$$/ /$$__  $$  #
# | $$      | $$  \ $$   /$$$$/ | $$  | $$      | $$  $$$| $$  /$$$$$$$| $$$$$$/ | $$$$$$$$  #
# | $$    $$| $$  | $$  /$$__/  | $$  | $$      | $$\  $ | $$ /$$__  $$| $$_  $$ | $$_____/  #
# |  $$$$$$/|  $$$$$$/ /$$$$$$$$|  $$$$$$$      | $$ \/  | $$|  $$$$$$$| $$ \  $$|  $$$$$$$  #
#  \______/  \______/ |________/ \____  $$      |__/     |__/ \_______/|__/  \__/ \_______/  #
#                                /$$  | $$                                                   #
#        )))                    |  $$$$$$/                                    Version 1.1    #
#       (((                      \______/                                                    #
#     +-----+                                   __..--''``---....___   _..._    __           #
#     |     |]      /    //    // //  /// //_.-'    .-/";  `        ``<._  ``.''_ `. / // /  #
#     `-----'   //  //  ///  ///     ///_.-' _..--.'_    \                    `( ) ) // //   #
#         /   / // / ///  /      /// / (_..-' // (< _     ;_..__               ; `' / ///    #
#     ///  //  /  /       // ///  /   / // // //  `-._,_)' // / ``--...____..-' /// / //     #
#    / // / /// //  /// / / // //   //  /// //  /  ///  //  // /// / /  ///   /   / ///  //  #
##############################################################################################

.PHONY: all debug clean clean_self fclean re re_debug sane re_sane bonus

NAME = minishell
MAKE_MODE ?= release

# Colors
GREEN = \033[1;32m
BLUE = \033[1;34m
RED = \033[1;31m
RESET = \033[0m

# Compiler
CC = cc
BASE_FLAGS = -Wall -Wextra -Werror=vla
RELEASE_FLAGS = -Werror -O3 -ffast-math -march=native -flto
DEBUG_FLAGS =  -g -O0 -fno-builtin -mno-omit-leaf-frame-pointer -fno-omit-frame-pointer -fstrict-flex-arrays=3
SANE_FLAGS = -fsanitize=address,pointer-compare,pointer-subtract,leak,undefined,shift,shift-exponent,shift-base,integer-divide-by-zero,unreachable,vla-bound,null,signed-integer-overflow,bounds,alignment,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,pointer-overflow,builtin -fsanitize-address-use-after-scope
ifeq ($(MAKE_MODE),release)
	CFLAGS = $(BASE_FLAGS) $(RELEASE_FLAGS)
    LIB_TARGET = all
else
	ifeq ($(MAKE_MODE),debug)
		CFLAGS = $(BASE_FLAGS) $(DEBUG_FLAGS)
		LIB_TARGET = debug
	else
	 ifeq ($(MAKE_MODE),sane)
	 	CFLAGS = $(BASE_FLAGS) $(DEBUG_FLAGS) $(SANE_FLAGS)
	 	LIB_TARGET = sane
	 else
		$(error Unknown mode)
		endif
	endif
endif

# Libraries
LIB_DIRS = libft
LIB_FILES = libft/libft.a
LDFLAGS = -lreadline

# Directories
SRC = src
OBJ = obj
INCLUDES = -Iincludes $(foreach lib,$(LIB_DIRS),-I$(lib)/includes)

# Sources
SRC_FILES := minishell.c lexer.c lexer_utils.c type_lexer.c
SRCS := $(addprefix $(SRC)/,$(SRC_FILES))
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

all:
	@$(MAKE) -j --no-print-directory $(NAME)

debug:
	@echo "$(GREEN)Building in debug mode$(RESET)"
	@$(MAKE) -j --no-print-directory MAKE_MODE=debug

sane:
	@echo "$(GREEN)Building in sane mode$(RESET)"
	@$(MAKE) -j --no-print-directory MAKE_MODE=sane

$(OBJ):
	@mkdir -p $@ $(dir $(OBJS))

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	@echo "$(BLUE)Compiling$(RESET) $<..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libs:
	@echo "$(BLUE)Building libraries in $(MAKE_MODE) mode...$(RESET)"
	@for lib in $(LIB_DIRS); do \
		if [ "$(LIB_TARGET)" = "sane" ] && [ "$$lib" != "libft" ]; then \
			$(MAKE) -j --no-print-directory -C $$lib debug; \
		else \
			$(MAKE) -j --no-print-directory -C $$lib $(LIB_TARGET); \
		fi; \
	done

$(NAME): $(OBJS) | libs
	@echo "$(GREEN)Linking$(RESET) $@..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIB_FILES) -o $@ $(LDFLAGS)
	@echo "$(GREEN)Successfully compiled $(NAME)!$(RESET)"

clean_self:
	@echo "$(RED)Cleaning$(RESET) $(NAME)'s object files..."
	@rm -rf $(OBJ)

clean: clean_self
	@for lib in $(LIB_DIRS); do \
		$(MAKE) --no-print-directory -s -C $$lib clean; \
	done

fclean: clean_self
	@echo "$(RED)Removing$(RESET) $(NAME)..."
	@rm -f $(NAME)
	@for lib in $(LIB_DIRS); do \
		$(MAKE) --no-print-directory -s -C $$lib fclean; \
	done

re: fclean
	@$(MAKE) -j --no-print-directory all

re_debug: fclean
	@$(MAKE) -j --no-print-directory debug

re_sane: fclean
	@$(MAKE) -j --no-print-directory sane

bonus: all
