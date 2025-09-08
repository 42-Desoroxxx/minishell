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
#        )))                    |  $$$$$$/                                    Version 1.6.1  #
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
GREEN = \033[32m
BLUE = \033[34m
RED = \033[31m
RESET = \033[0m

# Compiler
CC = cc
BASE_FLAGS = \
	-Wall -Wextra \
 	-Werror=vla -pedantic-errors -Werror=int-conversion -Werror=incompatible-pointer-types -Werror=implicit-function-declaration -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations \
	-MMD \
	-flto=thin
RELEASE_FLAGS = \
	-Werror \
	-O3 -ffast-math -march=native
DEBUG_FLAGS = \
	-gfull -O0 -fno-builtin -mno-omit-leaf-frame-pointer -fno-omit-frame-pointer \
	-DDEBUG=true
SANE_FLAGS = \
	-fsanitize=address,pointer-compare,pointer-subtract,leak,undefined,shift,shift-exponent,shift-base,integer-divide-by-zero,unreachable,vla-bound,null,signed-integer-overflow,bounds,alignment,float-divide-by-zero,float-cast-overflow,nonnull-attribute,returns-nonnull-attribute,bool,enum,pointer-overflow,builtin -fsanitize-address-use-after-scope

# Libraries
LIB_DIRS = libft
LIB_FILES = libft/libft.a
LDFLAGS = -lreadline

# Directories
SRC = src
OBJ = obj
INCLUDES = -Iincludes $(foreach lib,$(LIB_DIRS),-I$(lib)/includes)

# Sources
SRC_FILES := \
	minishell.c \
	utils/debug_utils.c utils/free_utils.c utils/env_utils.c utils/prompt_utils.c utils/quotes_utils.c \
	lexing/lexer.c lexing/type_lexer.c \
	expanding/expand.c expanding/expand_tokens.c expanding/expand_utils.c \
	parsing/parser.c parsing/check_grammar.c parsing/parse_heredoc.c parsing/parse_n_count.c parsing/parse_redirs.c parsing/pipeline.c \
	executing/path.c executing/exec.c \
	executing/builtin/echo.c executing/builtin/cd.c executing/builtin/cd_utils.c executing/builtin/pwd.c executing/builtin/export.c executing/builtin/export_utils.c executing/builtin/unset.c executing/builtin/env.c executing/builtin/exit.c
SRCS := $(addprefix $(SRC)/,$(SRC_FILES))
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

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

ifeq ($(shell expr $(shell $(CC) -v 2>&1 | grep -oP 'version \K\d+') \>= 15), 1)
    DEBUG_FLAGS += -fstrict-flex-arrays=3
    LDFLAGS += -fuse-ld=lld
endif

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

-include $(OBJS:%.o=%.d)

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
