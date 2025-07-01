# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/03 18:36:06 by jpelline          #+#    #+#              #
#    Updated: 2025/07/02 01:27:33 by erantala         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================== CONFIGURATION =============================== #

ifeq ($(MAKECMDGOALS),bonus)
NAME_BONUS	:= minishell_bonus
else
NAME		:= minishell_standard
endif

PROGRAM_NAME	:= minishell
CC		:= cc
CFLAGS		:= -Wall -Wextra -Werror
DEBUG_FLAGS	:= -g3 -fsanitize=address -fsanitize=undefined
OPTFLAGS	:= -O2

VPATH		:= src:src/builtin:src/tokenizer:src/execution

ifeq ($(MAKECMDGOALS),bonus)
SRC_DIR		:= src_bonus
else
SRC_DIR		:= src
endif

OBJ_DIR		:= obj

DEP_DIR		:= $(OBJ_DIR)/.deps
DEPFLAGS	= -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d

LIBFT_DIR	:= libft
LIBFT		:= $(LIBFT_DIR)/libft.a
INC		:= -I./include -I$(LIBFT_DIR)/include -I./src/pipe/include
INC_BONUS	:= -I./include_bonus -I$(LIBFT_DIR)/include

LDFLAGS		:= -L$(LIBFT_DIR) -lft -lreadline

# ============================== VISUAL STYLING ============================== #

BOLD		:= $(shell tput bold)
GREEN		:= $(shell tput setaf 2)
YELLOW		:= $(shell tput setaf 3)
BLUE		:= $(shell tput setaf 4)
MAGENTA		:= $(shell tput setaf 5)
CYAN		:= $(shell tput setaf 6)
WHITE		:= $(shell tput setaf 7)
RESET		:= $(shell tput sgr0)

# ============================== SOURCE FILES ================================ #

SRCS_MAIN	:= main.c memory_arena.c vector.c parse_input.c \
		syntax_check.c take_input.c export.c helpers.c heredoc.c \
		parse_additions.c parse_helpers.c signal.c echo.c pwd.c cd.c \
		exec_parse_utils.c built_in.c env.c unset.c shlv.c \
		vector_helpers.c mini_split.c non_interactive.c cleanup.c \
		command_help.c execution.c empty_exp.c

SRCS		:= $(SRCS_MAIN)

# ============================== PROGRESS TRACKING =========================== #

ifeq ($(MAKECMDGOALS),bonus)
OBJS_BONUS	:= $(addprefix $(OBJ_DIR)/,$(SRCS_BONUS:.c=.o))
TOTAL_SRCS	:= $(words $(SRCS_BONUS))
else
OBJS		:= $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))
TOTAL_SRCS	:= $(words $(SRCS))
endif

MARKER_STANDARD := .standard_build
MARKER_BONUS	:= .bonus_build
PROGRESS_FILE	:= $(OBJ_DIR)/.progress
WAS_BONUS	:= $(shell [ -f "$(MARKER_BONUS)" ] && echo yes)
LATEST_SRC	:= $(shell find src -name "*.c" | xargs ls -t 2>/dev/null | head -1)
OBJ_FILES_EXIST := $(shell [ -n "$(wildcard $(OBJ_DIR)/*.o)" ] && echo yes)

is_up_to_date = \
    [ -f $(PROGRAM_NAME) ] && \
    [ "$(PROGRAM_NAME)" -nt $(LATEST_SRC) ] && \
    [ "$(PROGRAM_NAME)" -nt $(LIBFT) ] && \
    [ "$(OBJ_FILES_EXIST)" = "yes" ]

# ============================== BUILD TARGETS =============================== #

all:
	@if [ -f $(MARKER_BONUS) ] && $(is_up_to_date); then \
		echo ">$(BOLD)$(WHITE) Cleaning bonus build...$(RESET)"; \
		$(MAKE) -s clean; \
	fi; \
	if [ -f $(MARKER_STANDARD) ] && $(is_up_to_date) 2>/dev/null; then \
		echo ">$(BOLD)$(YELLOW)  $(NAME) is already up to date.$(RESET)"; \
	else \
		echo ">$(BOLD)$(WHITE) Starting to build $(NAME)...$(RESET)"; \
		$(MAKE) $(NAME) --no-print-directory; \
		touch $(MARKER_STANDARD); \
		rm -f $(MARKER_BONUS) ; \
		echo ">$(BOLD)$(GREEN)  All components built successfully!$(RESET)"; \
	fi

$(NAME): $(OBJS) $(LIBFT)
	@echo ">$(BOLD)$(GREEN)  Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJS) $(LDFLAGS) $(OPTFLAGS)
	@touch $(MARKER_STANDARD)
	@rm -f $(MARKER_BONUS)
	@rm -f $(PROGRESS_FILE)
	@echo ">$(BOLD)$(GREEN)  $(NAME) successfully compiled!$(RESET)"

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR) $(DEP_DIR)
	@if [ -f $(PROGRESS_FILE) ]; then \
		CURRENT=$$(cat $(PROGRESS_FILE)); \
		NEXT=$$((CURRENT + 1)); \
		echo "$$NEXT" > $(PROGRESS_FILE); \
		printf ">   [%3d%%] $(CYAN)(%d/%d files) Compiling $<... $(RESET)\n" \
			$$((NEXT*100/$(TOTAL_SRCS))) $$((NEXT)) $(TOTAL_SRCS); \
	fi
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(OPTFLAGS) -c $< -o $@ $(INC)

# ============================== BONUS TARGETS =============================== #

bonus:
	@if [ -f $(MARKER_STANDARD) ] && $(is_up_to_date); then \
		echo ">$(BOLD)$(WHITE) Cleaning standard build...$(RESET)"; \
		$(MAKE) -s clean MAKECMDGOALS=bonus; \
	fi; \
	if [ -f "$(MARKER_BONUS)" ] && $(is_up_to_date) 2>/dev/null; then \
		echo ">$(BOLD)$(YELLOW)  $(NAME_BONUS) is already up to date.$(RESET)"; \
	else \
		echo ">$(BOLD)$(WHITE) Starting to build $(NAME_BONUS)...$(RESET)"; \
		$(MAKE) $(NAME_BONUS) MAKECMDGOALS=bonus --no-print-directory; \
		touch $(MARKER_BONUS); \
		rm -f $(MARKER_STANDARD); \
		echo ">$(BOLD)$(GREEN)  All components built successfully!$(RESET)"; \
	fi

$(NAME_BONUS): $(OBJS_BONUS) $(LIBFT)
	@echo ">$(BOLD)$(GREEN)  Linking $(NAME_BONUS)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJS_BONUS) $(LDFLAGS) $(OPTFLAGS)
	@touch $(MARKER_BONUS)
	@rm -f $(MARKER_STANDARD)
	@echo ">$(BOLD)$(GREEN)  $(NAME_BONUS) successfully compiled!$(RESET)"
	@rm -f $(PROGRESS_FILE)

ifeq ($(MAKECMDGOALS),bonus)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	@touch $(PROGRESS_FILE)
	@if [ -f $(PROGRESS_FILE) ]; then \
		CURRENT=$$(cat $(PROGRESS_FILE)); \
		NEXT=$$((CURRENT + 1)); \
		echo "$$NEXT" > $(PROGRESS_FILE); \
		printf ">   [%3d%%] $(CYAN)(%d/%d files) Compiling $<... $(RESET)\n" \
			$$((NEXT*100/$(TOTAL_SRCS))) $$((NEXT)) $(TOTAL_SRCS); \
	fi
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(OPTFLAGS) -c $< -o $@ $(INC_BONUS)
endif

# ============================== ADDITIONAL TARGETS =============================== #

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "0" > $(PROGRESS_FILE)

$(DEP_DIR): | $(OBJ_DIR)
	@mkdir -p $@

-include $(wildcard $(DEP_DIR)/*.d)

debug: CFLAGS += $(DEBUG_FLAGS)
debug: OPTFLAGS := -O0
debug: clean $(NAME)
	@echo ">$(BOLD)$(CYAN)  Debug build completed!$(RESET)"

$(LIBFT):
	@echo ">$(MAGENTA)  Entering libft directory...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR) --no-print-directory

# ============================== CLEAN TARGETS =============================== #

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		echo "> [ pipex ] $(YELLOW) Cleaning object files...$(RESET)"; \
		rm -rf $(OBJ_DIR); \
		echo "            $(YELLOW) Object files cleaned!$(RESET)"; \
	else \
		echo "> [ pipex ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)clean$(RESET)"; \
	fi
	@if [ "$(MAKECMDGOALS)" != "bonus" ] && [ "$(WAS_BONUS)" != "yes" ]; then \
		if [ -d $(LIBFT_DIR)/$(OBJ_DIR) ]; then \
			$(MAKE) -C $(LIBFT_DIR) clean --no-print-directory; \
		else \
			echo "> [ libft ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)clean$(RESET)"; \
		fi; \
	fi

fclean: clean
	@if [ -f $(PROGRAM_NAME) ]; then \
		echo "> [ pipex ] $(YELLOW) Removing $(PROGRAM_NAME)...$(RESET)"; \
		rm -f $(PROGRAM_NAME); \
		rm -f $(MARKER_BONUS); \
		rm -f $(MARKER_STANDARD); \
		echo "            $(YELLOW) $(PROGRAM_NAME) removed!$(RESET)"; \
	else \
		echo "> [ pipex ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)fclean$(RESET)"; \
	fi
	@if [ -f $(LIBFT) ]; then \
		$(MAKE) -C $(LIBFT_DIR) fclean --no-print-directory; \
	else \
		echo "> [ libft ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)fclean$(RESET)"; \
	fi

ifeq ($(WAS_BONUS),yes)
re:
	@echo "> [ pipex ] $(BOLD)$(WHITE) Rebuilding from scratch...$(RESET)"
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) bonus --no-print-directory
else
re:
	@echo "> [ pipex ] $(BOLD)$(WHITE) Rebuilding from scratch...$(RESET)"
	@$(MAKE) fclean --no-print-directory
	@$(MAKE) $(NAME) --no-print-directory
endif

help:
	@echo "$(BOLD)$(CYAN)Available targets:$(RESET)"
	@echo "  $(GREEN)all$(RESET)     - Build the project (default)"
	@echo "  $(GREEN)debug$(RESET)   - Build with debug flags and sanitizers"
	@echo "  $(GREEN)clean$(RESET)   - Remove object files"
	@echo "  $(GREEN)fclean$(RESET)  - Remove all generated files"
	@echo "  $(GREEN)re$(RESET)      - Rebuild from scratch"
	@echo "  $(GREEN)help$(RESET)    - Show this help message"

.SECONDARY: $(OBJS) $(OBJS_BONUS)
.PHONY: all debug clean fclean re help bonus
