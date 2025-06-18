/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unit_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erantala <erantala@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 01:57:21 by erantala          #+#    #+#             */
/*   Updated: 2025/06/18 03:59:21 by erantala         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/minishell.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

int	is_aligned(void *ptr) {
    return ((uintptr_t)ptr % ALIGNMENT) == 0;
}

void test_basic_allocation() {
    void *a = arena_malloc(16);
    void *b = arena_malloc(32);
    assert(a != NULL && b != NULL);
    assert(a != b);
    printf("Basic allocation test passed.\n");
}

void test_alignment() {
    for (int i = 1; i <= 64; i++) {
        void *ptr = arena_malloc(i);
        assert(is_aligned(ptr));
    }
    printf("Alignment test passed.\n");
}

void test_no_overlap() {
    void *a = arena_malloc(24);
    void *b = arena_malloc(24);
    assert((char*)a + 24 <= (char*)b || (char*)b + 24 <= (char*)a);
    printf("No overlap test passed.\n");
}

void test_large_allocation() {
    void *ptr = arena_malloc(ARENA_SIZE / 2);
    assert(ptr != NULL);
    printf("Large allocation test passed.\n");
}

void test_multiple_arenas() {
    size_t block_size = ARENA_SIZE / 4;
    void *blocks[6];

    // Allocate enough blocks to exceed one arena's capacity
    for (int i = 0; i < 6; ++i) {
        blocks[i] = arena_malloc(block_size);
        assert(blocks[i] != NULL);
        assert(is_aligned(blocks[i]));
        // Check that this block does not overlap with any previous block
        for (int j = 0; j < i; ++j) {
            assert((char*)blocks[j] + block_size <= (char*)blocks[i] ||
                   (char*)blocks[i] + block_size <= (char*)blocks[j]);
        }
    }
    printf("Multiple arenas allocation test passed.\n");
}

void test_too_large_allocation() {
    // Try to allocate a block larger than a single arena's capacity
    void *ptr = arena_malloc(ARENA_SIZE + 1024);
    assert(ptr != NULL); // Should succeed if your arena system supports big allocations
    assert(is_aligned(ptr));
    printf("Too large allocation test passed.\n");
}

int main(void) {
    test_basic_allocation();
    test_alignment();
    test_no_overlap();
    test_large_allocation();
    test_multiple_arenas();
    test_too_large_allocation();
    printf("All arena tests passed!\n");
    return 0;
}