# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jpelline <jpelline@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/08 21:55:46 by jpelline          #+#    #+#              #
#    Updated: 2025/06/09 20:14:38 by jpelline         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========================== CONFIGURATION ============================ #

NAME		:= libft.a
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror
AR			:= ar rcs

SRC_DIR		:= src
OBJ_DIR		:= obj
INC_DIR		:= include
DEP_DIR		:= $(OBJ_DIR)/.deps

MARKER		:= .standard_build
DEPFLAGS	= -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d
INC			:= -I./$(INC_DIR)

# ========================== VISUAL STYLING =========================== #

BOLD		:= $(shell tput bold)
GREEN		:= $(shell tput setaf 2)
YELLOW		:= $(shell tput setaf 3)
BLUE		:= $(shell tput setaf 4)
MAGENTA		:= $(shell tput setaf 5)
CYAN		:= $(shell tput setaf 6)
WHITE		:= $(shell tput setaf 7)
RESET		:= $(shell tput sgr0)

# ========================== SOURCE FILES ============================== #

# Character classification and conversion functions
CHAR_SRCS	:= ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
			ft_isprint.c ft_toupper.c ft_tolower.c

# String manipulation functions
STR_SRCS	:= ft_strlen.c ft_strlcpy.c ft_strlcat.c ft_strchr.c \
			ft_strrchr.c ft_strncmp.c ft_strnstr.c ft_strdup.c \
			ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
			ft_strmapi.c ft_striteri.c ft_reverse_string.c

# Memory management functions
MEM_SRCS	:= ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
			ft_memchr.c ft_memcmp.c ft_calloc.c

# File descriptor operations
FD_SRCS		:= get_next_line.c ft_putchar_fd.c ft_putstr_fd.c \
			ft_putendl_fd.c get_next_line_utils.c ft_putnbr_fd.c \
			ft_uputnbr_fd.c

# Conversion functions
CONV_SRCS	:= ft_atoi.c ft_atof.c ft_atoib.c ft_itoa.c ft_printf.c

# Linked list functions (bonus)
LST_SRCS	:= ft_lstnew_bonus.c ft_lstadd_front_bonus.c \
			ft_lstsize_bonus.c ft_lstlast_bonus.c \
			ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
			ft_lstclear_bonus.c ft_lstiter_bonus.c \
			ft_lstmap_bonus.c

# ========================== OBJECT FILES ============================== #

SRCS		:= $(CHAR_SRCS) $(STR_SRCS) $(MEM_SRCS) $(FD_SRCS) \
			$(CONV_SRCS) $(LST_SRCS)

OBJS		:= $(addprefix $(OBJ_DIR)/,$(SRCS:.c=.o))
TOTAL_SRCS	:= $(words $(SRCS))

# ========================== PROGRESS TRACKING ========================= #

LAST_PERCENTAGE		:= 0
COMPILED_COUNT		:= 0

# ========================== UTILITY FUNCTIONS ========================= #

# Check if program is up to date
is_up_to_date	= [ -f $(NAME) ] && \
				[ -z "$$(find $(SRC_DIR) -name '*.c' -newer $(NAME) 2>/dev/null)" ]

# ========================== BUILD TARGETS ============================== #

.PHONY: all clean fclean re

# Default target with intelligent rebuild detection
all:
	@if [ -f $(NAME) ] && [ -f $(MARKER) ] && $(is_up_to_date) 2>/dev/null; then \
		echo ">$(BOLD)$(YELLOW)  $(NAME) is already up to date.$(RESET)"; \
	else \
		echo ">$(BOLD)$(WHITE) Starting to build $(NAME)...$(RESET)"; \
		$(MAKE) $(NAME) --no-print-directory; \
		touch $(MARKER); \
		echo ">$(BOLD)$(GREEN)  All components built successfully!$(RESET)"; \
	fi

# Library creation target
$(NAME): $(OBJS)
	@echo ">$(BOLD)$(GREEN)  All files compiled successfully!$(RESET)"
	@echo ">$(BOLD)$(GREEN)  Linking $(NAME)...$(RESET)"
	@$(AR) $@ $^
	@touch $(MARKER)
	@echo ">$(BOLD)$(GREEN)  $(NAME) successfully compiled!$(RESET)"

# ========================== DIRECTORY CREATION ======================= #

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	@mkdir -p $@

# ========================== COMPILATION RULES ========================= #

# Object file compilation with progress tracking
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	$(eval COMPILED_COUNT := $(shell echo $$(($(COMPILED_COUNT)+1))))
	$(eval PROGRESS := $(shell echo $$(($(COMPILED_COUNT)*100/$(TOTAL_SRCS)))))
	@if [ $$(($(PROGRESS) % 25)) -eq 0 ] || [ $(PROGRESS) -eq 100 ]; then \
		printf ">   [%3d%%] $(CYAN)Compiling... (%d/%d files)$(RESET)\n" \
			$(PROGRESS) $(COMPILED_COUNT) $(TOTAL_SRCS); \
	fi
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@ $(INC)

# ========================== DEPENDENCY HANDLING ======================= #

# Include auto-generated dependency files
-include $(wildcard $(DEP_DIR)/*.d)

# ========================== CLEANUP TARGETS =========================== #

# Remove object files and dependency files
clean:
	@if [ -d $(OBJ_DIR) ]; then \
		echo "> [ libft ] $(YELLOW) Cleaning object files...$(RESET)"; \
		rm -rf $(OBJ_DIR); \
		echo "            $(YELLOW) Object files cleaned!$(RESET)"; \
	else \
		echo "> [ libft ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)clean$(RESET)"; \
	fi

# Remove everything including the library
fclean: clean
	@if [ -f $(NAME) ]; then \
		echo "> [ libft ] $(YELLOW) Removing $(NAME)...$(RESET)"; \
		rm -rf $(NAME); \
		rm -f $(MARKER); \
		echo "            $(YELLOW) $(NAME) removed!$(RESET)"; \
	else \
		echo "> [ libft ] $(BOLD)$(YELLOW) Nothing to be done with $(RESET)$(WHITE)fclean$(RESET)"; \
	fi

# Full rebuild from scratch
re: fclean
	@echo "> [ libft ] $(BOLD)$(WHITE) Rebuilding from scratch...$(RESET)"
	@$(MAKE) all --no-print-directory

# ========================== BUILD OPTIMIZATION ======================= #

# Prevent intermediate files from being deleted
.SECONDARY: $(OBJS)
