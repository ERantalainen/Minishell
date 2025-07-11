# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/03 18:36:06 by jpelline          #+#    #+#              #
#    Updated: 2025/07/11 14:57:59 by erantala         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ================================= SETTINGS ================================= #

# Project configuration
PROGRAM_NAME		:= minishell
NAME			:= minishell_standard
CC			:= cc

# Compiler flags
CFLAGS			:=
DEBUG_FLAGS		:= -g3 -fsanitize=address -fsanitize=undefined
OPTFLAGS		:= -O2

# Directory structure
SRC_DIR			:= src
OBJ_DIR			:= obj
DEP_DIR			:= $(OBJ_DIR)/.deps
LIBFT_DIR		:= libft

# Search paths for source files
BUILTIN_DIRS := $(shell find $(SRC_DIR)/built_in -type d | tr '\n' ':' | sed 's/:$$//')

VPATH			:= $(SRC_DIR):$(BUILTIN_DIRS):$(SRC_DIR)/execution \
			:$(SRC_DIR)/here_doc:$(SRC_DIR)/main:$(SRC_DIR)/memory_arena \
			:$(SRC_DIR)/parsing:$(SRC_DIR)/signal:$(SRC_DIR)/utility \
			:$(SRC_DIR)/vector

# Include paths and libraries
INC			:= -I./include -I$(LIBFT_DIR)/include
LIBFT			:= $(LIBFT_DIR)/libft.a
LDFLAGS			:= -L$(LIBFT_DIR) -lft -lreadline

# Dependency generation flags
DEPFLAGS		= -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

# ============================== VISUAL STYLING ============================== #

# Terminal colors for build output
BOLD			:= $(shell tput bold)
GREEN			:= $(shell tput setaf 2)
YELLOW			:= $(shell tput setaf 3)
BLUE			:= $(shell tput setaf 4)
MAGENTA			:= $(shell tput setaf 5)
CYAN			:= $(shell tput setaf 6)
WHITE			:= $(shell tput setaf 7)
RESET			:= $(shell tput sgr0)

# ============================== SOURCE FILES ================================ #

# Parsing and syntax analysis
SRCS_PARSE := \
	parse_input.c \
	syntax_check.c \
	parse_additions.c \
	parse_helpers.c \
	heredoc.c \
	parse_extra.c \
	parse_input_help.c \
	syntax_help.c \
	parsing_token_help.c \
	parse_command_help.c

# Built-in shell commands
SRCS_BUILTIN := \
	echo.c \
	pwd.c \
	cd.c \
	export.c \
	export_help.c \
	export_help_extra.c \
	built_in.c \
	env.c \
	unset.c \
	shlv.c \
	built_helpers.c \
	exit_helper.c \
	echo_helper.c \
	additional_helpers.c

# Utility functions and helpers
SRCS_UTILS := \
	memory_arena.c \
	memory_arena_helpers.c \
	vector.c \
	vector_helpers.c \
	command_help.c \
	mini_split.c \
	helpers.c \
	empty_exp.c \
	cleanup.c \
	multiples.c \
	mini_atoi.c

# Command execution and process management
SRCS_EXEC := \
	exec_parse_utils.c \
	execution.c \
	exec_utility.c \
	exec_utility_extra.c \
	file_handling.c \
	child.c \
	parsing.c

# Main program and entry point
SRCS_OTHER := \
	minishell.c \
	non_interactive.c \
	take_input.c \
	signal.c \
	here_helpers.c

# Combine all source files
SRCS := \
	$(SRCS_OTHER) \
	$(SRCS_UTILS) \
	$(SRCS_PARSE) \
	$(SRCS_BUILTIN) \
	$(SRCS_EXEC)

# ============================== BUILD VARIABLES ============================= #

# Object files and build tracking
OBJS			:= $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))
TOTAL_SRCS		:= $(words $(SRCS))

# Build markers and progress tracking
MARKER_STANDARD		:= .standard_build
PROGRESS_FILE		:= $(OBJ_DIR)/.progress

# Utility variables for build optimization
LATEST_SRC		:= $(shell find src -name "*.c" | xargs ls -t 2>/dev/null | head -1)
OBJ_FILES_EXIST		:= $(shell [ -n "$(wildcard $(OBJ_DIR)/*.o)" ] && echo yes)

# Looking for updated header files
LATEST_HEADER := $(shell find include $(LIBFT_DIR)/include -name "*.h" 2>/dev/null | xargs ls -t 2>/dev/null | head -1)

# Looking for updated libft source files
LATEST_LIBFT_SRC := $(shell find $(LIBFT_DIR)/src -name "*.c" 2>/dev/null | xargs ls -t 2>/dev/null | head -1)

# Check if binary is up to date
is_up_to_date = \
	[ -f $(PROGRAM_NAME) ] && \
	[ "$(PROGRAM_NAME)" -nt $(LATEST_SRC) ] && \
	[ "$(PROGRAM_NAME)" -nt $(LATEST_HEADER) ] && \
	[ "$(PROGRAM_NAME)" -nt $(LIBFT) ] && \
	( [ -z "$(LATEST_LIBFT_SRC)" ] || [ "$(PROGRAM_NAME)" -nt $(LATEST_LIBFT_SRC) ] ) && \
	[ "$(OBJ_FILES_EXIST)" = "yes" ]

# ============================== BUILD TARGETS =============================== #

all:
	@if [ -f $(MARKER_STANDARD) ] && $(is_up_to_date) 2>/dev/null; then \
		echo ">$(BOLD)$(YELLOW)  $(NAME) is already up to date.$(RESET)"; \
	else \
		echo ">$(BOLD)$(WHITE) Starting to build $(NAME)...$(RESET)"; \
		$(MAKE) $(NAME) --no-print-directory; \
		touch $(MARKER_STANDARD); \
		echo ">$(BOLD)$(GREEN)  All components built successfully!$(RESET)"; \
	fi

$(NAME): $(OBJS) libft-check
	@echo ">$(BOLD)$(GREEN)  Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJS) $(LDFLAGS) $(OPTFLAGS)
	@touch $(MARKER_STANDARD)
	@rm -f $(PROGRESS_FILE)
	@echo ">$(BOLD)$(GREEN)  $(NAME) successfully compiled!$(RESET)"

$(OBJ_DIR)/%.o: %.c include/minishell.h | $(OBJ_DIR) $(DEP_DIR)
	@if [ -f $(PROGRESS_FILE) ]; then \
		CURRENT=$$(cat $(PROGRESS_FILE)); \
		NEXT=$$((CURRENT + 1)); \
		echo "$$NEXT" > $(PROGRESS_FILE); \
		printf ">   [%3d%%] $(CYAN)(%d/%d files) Compiling $<... $(RESET)\n" \
			$$((NEXT*100/$(TOTAL_SRCS))) $$((NEXT)) $(TOTAL_SRCS); \
	fi
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(OPTFLAGS) -c $< -o $@ $(INC)

# ============================== ADDITIONAL TARGETS =============================== #

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "0" > $(PROGRESS_FILE)

-include $(wildcard $(DEP_DIR)/*.d)

$(DEP_DIR): | $(OBJ_DIR)
	@mkdir -p $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: OPTFLAGS := -O0
debug: clean $(NAME)
	@echo ">$(BOLD)$(CYAN)  Debug build completed!$(RESET)"

$(LIBFT):
	@echo ">$(MAGENTA)  Entering libft directory...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

libft-check:
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

# ============================== CLEAN TARGETS =============================== #

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		echo "> [ minishell ] $(YELLOW) Cleaning object files...$(RESET)"; \
		rm -rf $(OBJ_DIR); \
		echo "                $(YELLOW) Object files cleaned!$(RESET)"; \
	else \
		echo "> [ minishell ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)clean$(RESET)"; \
	fi

fclean: clean
	@if [ -f $(PROGRAM_NAME) ]; then \
		echo "> [ minishell ] $(YELLOW) Removing $(PROGRAM_NAME)...$(RESET)"; \
		rm -f $(PROGRAM_NAME); \
		rm -f $(MARKER_STANDARD); \
		echo "                $(YELLOW) $(PROGRAM_NAME) removed!$(RESET)"; \
	else \
		echo "> [ minishell ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)fclean$(RESET)"; \
	fi
	@if [ -f $(LIBFT) ]; then \
		$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory; \
	else \
		echo "> [ libft ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)fclean$(RESET)"; \
	fi

re:
	@echo "> [ minishell ] $(BOLD)$(WHITE) Rebuilding from scratch...$(RESET)"
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) $(NAME) --no-print-directory

.SECONDARY: $(OBJS)
.PHONY: all debug clean fclean re libft-check
